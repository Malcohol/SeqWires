/**
 * Write a Standard MIDI File based on the contents of a tree of Features.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <BabelWiresPlugins/Smf/Plugin/smfWriter.hpp>

#include <BabelWiresPlugins/Smf/Plugin/gmSpec.hpp>

#include <SeqWiresLib/Features/trackFeature.hpp>
#include <SeqWiresLib/Utilities/filteredTrackIterator.hpp>
#include <SeqWiresLib/Utilities/musicUtilities.hpp>
#include <SeqWiresLib/Utilities/trackTraverser.hpp>
#include <SeqWiresLib/Tracks/percussionEvents.hpp>

#include <BabelWiresLib/Project/projectContext.hpp>
#include <BabelWiresLib/TypeSystem/typeSystem.hpp>

#include <algorithm>
#include <sstream>

smf::SmfWriter::SmfWriter(const babelwires::ProjectContext& projectContext, babelwires::UserLogger& userLogger,
                          const target::SmfFormatFeature& sequence, std::ostream& ostream)
    : m_projectContext(projectContext)
    , m_smfFormatFeature(sequence)
    , m_ostream(ostream)
    , m_os(&m_ostream)
    , m_division(256) {
    const seqwires::PercussionKit& gmKit =
        projectContext.m_typeSystem.getRegisteredEntry(seqwires::GMPercussionKit::getThisIdentifier())
            .is<seqwires::GMPercussionKit>();
    const seqwires::PercussionKit& gm2StandardKit =
        projectContext.m_typeSystem.getRegisteredEntry(seqwires::GM2StandardPercussionKit::getThisIdentifier())
            .is<seqwires::GM2StandardPercussionKit>();
    m_knownKits[GM_PERCUSSION_KIT] = &gmKit;
    m_knownKits[GM2_STANDARD_PERCUSSION_KIT] = &gm2StandardKit;
}

void smf::SmfWriter::writeUint16(std::uint16_t i) {
    m_os->put(i >> 8);
    m_os->put(i & 255);
}

void smf::SmfWriter::writeUint24(std::uint32_t i) {
    assert((i < (1 << 24)) && "Value cannot be represented in 24 bits");
    m_os->put(i >> 16);
    m_os->put((i >> 8) & 255);
    m_os->put(i & 255);
}

void smf::SmfWriter::writeUint32(std::uint32_t i) {
    m_os->put(i >> 24);
    m_os->put((i >> 16) & 255);
    m_os->put((i >> 8) & 255);
    m_os->put(i & 255);
}

void smf::SmfWriter::writeVariableLengthQuantity(std::uint32_t i) {
    assert((i <= 0x0fffffff) && "Value is too big for a variable-lengths quantity");

    if (const std::uint32_t b0 = ((i >> 21) & 0x7f)) {
        m_os->put(b0 | 0x80);
    }
    if (const std::uint32_t b1 = ((i >> 14) & 0x7f)) {
        m_os->put(b1 | 0x80);
    }
    if (const std::uint32_t b2 = ((i >> 7) & 0x7f)) {
        m_os->put(b2 | 0x80);
    }
    const std::uint32_t b3 = i & 0x7f;
    m_os->put(b3);
}

void smf::SmfWriter::writeModelDuration(const seqwires::ModelDuration& d) {
    // TODO: Quantize if this isn't exact.
    const int numDivisions = (d.getNumerator() * m_division * 4) / (d.getDenominator());
    writeVariableLengthQuantity(numDivisions);
}

void smf::SmfWriter::writeTempoEvent(int bpm) {
    m_os->put(0x00u);
    m_os->put(0xffu);
    m_os->put(0x51u);
    m_os->put(0x03u);

    const int d = 60'000'000 / bpm;

    writeUint24(d);
}

void smf::SmfWriter::writeTextMetaEvent(int type, std::string text) {
    assert((0 <= type) && (type <= 15) && "Type is out-of-range.");
    babelwires::Byte t = type;
    m_os->put(0x00u);
    m_os->put(0xffu);
    m_os->put(t);
    writeVariableLengthQuantity(text.length());

    // TODO assert text is ASCII.
    *m_os << text;
}

void smf::SmfWriter::writeHeaderChunk() {
    const int numTracks = m_smfFormatFeature.getNumMidiTracks();

    assert((m_division < (2 << 15)) && "division is too large");

    m_os->write("MThd", 4);
    writeUint32(6);
    writeUint16(m_smfFormatFeature.getSelectedTagIndex());
    writeUint16(numTracks);

    {
        int division = 1;
        for (int i = 0; i < numTracks; ++i) {
            const target::ChannelGroup& channelGroup = m_smfFormatFeature.getMidiTrack(i);
            for (int j = 0; j < channelGroup.getNumTracks(); ++j) {
                const target::ChannelTrackFeature& entry = channelGroup.getTrack(j);
                division = babelwires::lcm(division, seqwires::getMinimumDenominator(entry.m_trackFeature->get()));
            }
        }
        m_division = division;
    }

    // 0 in high-bit implies metrical.
    writeUint16(m_division);
}

void smf::SmfWriter::writeNoteEvent(int channel, seqwires::ModelDuration timeSinceLastEvent,
                                    const seqwires::TrackEvent& e) {
    assert(channel >= 0);
    assert(channel <= 15);

    writeModelDuration(timeSinceLastEvent);
    if (const seqwires::NoteOnEvent* noteOn = e.as<seqwires::NoteOnEvent>()) {
        m_os->put(0b10010000 | channel);
        m_os->put(noteOn->m_pitch);
        m_os->put(noteOn->m_velocity);
    } else if (const seqwires::NoteOffEvent* noteOff = e.as<seqwires::NoteOffEvent>()) {
        m_os->put(0b10000000 | channel);
        m_os->put(noteOff->m_pitch);
        m_os->put(noteOff->m_velocity);
    } else {
        // TODO
        assert(!"Unrecognized note event encountered");
    }
}

namespace {
    struct NoteTrackIterator : seqwires::FilteredTrackIterator<> {
        bool isEventOfInterest(const seqwires::TrackEvent& event) const { return event.as<seqwires::NoteEvent>(); }
    };

    struct PercussionTrackIterator : seqwires::FilteredTrackIterator<> {
        bool isEventOfInterest(const seqwires::TrackEvent& event) const { return event.as<seqwires::PercussionEvent>(); }
    };

} // namespace

void smf::SmfWriter::writeNotes(const target::ChannelGroup& channelGroup) {
    const int numChannels = channelGroup.getNumTracks();

    seqwires::ModelDuration trackDuration = 0;

    // TODO Use the mergeFunction to multiplex the channels.

    std::vector<seqwires::TrackTraverser<seqwires::FilteredTrackIterator<seqwires::TrackEvent>>> traversers;

    for (int i = 0; i < channelGroup.getNumTracks(); ++i) {
        const smf::target::ChannelTrackFeature& channelTrack = channelGroup.getTrack(i);
        const seqwires::Track& track = channelTrack.m_trackFeature->get();
        const int channelNumber = channelTrack.m_channelNum->get();
        traversers.emplace_back(track, seqwires::iterateOver<seqwires::TrackEvent>(track));
        traversers.back().leastUpperBoundDuration(trackDuration);
    }

    seqwires::ModelDuration timeSinceStart = 0;
    seqwires::ModelDuration timeOfLastEvent = 0;
    while (timeSinceStart < trackDuration) {
        seqwires::ModelDuration timeToNextEvent = trackDuration - timeSinceStart;
        for (int i = 0; i < channelGroup.getNumTracks(); ++i) {
            traversers[i].greatestLowerBoundNextEvent(timeToNextEvent);
        }

        bool isFirstEvent = true;
        for (int i = 0; i < channelGroup.getNumTracks(); ++i) {
            const smf::target::ChannelTrackFeature& channelTrack = channelGroup.getTrack(i);
            traversers[i].advance(timeToNextEvent, [this, &isFirstEvent, &timeToNextEvent, &timeOfLastEvent,
                                                    &timeSinceStart, &channelTrack](const seqwires::TrackEvent& event) {
                seqwires::ModelDuration timeToThisEvent = 0;
                if (isFirstEvent) {
                    timeToThisEvent = timeToNextEvent;
                    timeOfLastEvent = timeSinceStart + timeToNextEvent;
                    isFirstEvent = false;
                }
                writeNoteEvent(channelTrack.m_channelNum->get(), timeToThisEvent, event);
            });
        }

        timeSinceStart += timeToNextEvent;
    }

    // End of track event.
    writeModelDuration(trackDuration - timeOfLastEvent);
}

void smf::SmfWriter::writeTrack(const target::ChannelGroup* channelGroup, const MidiMetadata& metadata) {
    std::ostream* oldStream = m_os;
    std::ostringstream tempStream;
    m_os = &tempStream;

    if (const auto* copyright = metadata.getCopyright()) {
        std::string ctext = copyright->get();
        if (!ctext.empty()) {
            writeTextMetaEvent(2, ctext);
        }
    }
    if (const auto* sequenceOrTrackName = metadata.getSequenceName()) {
        std::string ntext = sequenceOrTrackName->get();
        if (!ntext.empty()) {
            writeTextMetaEvent(3, ntext);
        }
    }
    if (const auto* tempo = metadata.getTempoFeature()) {
        writeTempoEvent(tempo->get());
    }
    if (channelGroup) {
        writeNotes(*channelGroup);
    } else {
        writeModelDuration(0);
    }

    // End of track.
    m_os->put(0xffu);
    m_os->put(0x2Fu);
    m_os->put(0x00u);

    m_os = oldStream;
    m_os->write("MTrk", 4);
    writeUint32(static_cast<std::uint32_t>(tempStream.tellp()));
    m_os->write(tempStream.str().data(), tempStream.tellp());
}

smf::SmfWriter::KnownPercussionKits smf::SmfWriter::getPercussionKit(int channel) {
    const GMSpecType::Value spec = m_smfFormatFeature.getMidiMetadata().getSpecFeature()->getAsValue();
    switch (spec)
    {
    case GMSpecType::Value::GM:
        if (channel == 9) {
            return GM_PERCUSSION_KIT;
        }
        break;
    case GMSpecType::Value::GM2:
        if (channel == 9) {
            return GM2_STANDARD_PERCUSSION_KIT;
        }
        break;
    case GMSpecType::Value::NONE:
    default:
        break;
    }
    return NOT_PERCUSSION;
}

void smf::writeToSmf(const babelwires::ProjectContext& projectContext, babelwires::UserLogger& userLogger,
                     const target::SmfFormatFeature& smfFormatFeature, std::ostream& output) {
    const int numTracks = smfFormatFeature.getNumMidiTracks();
    smf::SmfWriter writer(projectContext, userLogger, smfFormatFeature, output);
    writer.writeHeaderChunk();
    if (smfFormatFeature.getSelectedTagIndex() == 0) {
        writer.writeTrack(&smfFormatFeature.getMidiTrack(0), smfFormatFeature.getMidiMetadata());
    } else {
        writer.writeTrack(&smfFormatFeature.getMidiTrack(0), smfFormatFeature.getMidiMetadata());
        for (int i = 1; i < numTracks; ++i) {
            MidiMetadata dummyMetadata;
            writer.writeTrack(&smfFormatFeature.getMidiTrack(i), dummyMetadata);
        }
    }
}

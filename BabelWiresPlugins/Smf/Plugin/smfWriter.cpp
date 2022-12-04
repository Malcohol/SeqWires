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
#include <SeqWiresLib/Tracks/percussionEvents.hpp>
#include <SeqWiresLib/Utilities/filteredTrackIterator.hpp>
#include <SeqWiresLib/Utilities/musicUtilities.hpp>
#include <SeqWiresLib/Utilities/trackTraverser.hpp>

#include <BabelWiresLib/Project/projectContext.hpp>
#include <BabelWiresLib/TypeSystem/typeSystem.hpp>

#include <Common/Log/userLogger.hpp>

#include <algorithm>
#include <sstream>
#include <set>

smf::SmfWriter::SmfWriter(const babelwires::ProjectContext& projectContext, babelwires::UserLogger& userLogger,
                          const target::SmfFormatFeature& sequence, std::ostream& ostream)
    : m_projectContext(projectContext)
    , m_userLogger(userLogger)
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

smf::SmfWriter::WriteTrackEventResult smf::SmfWriter::writeTrackEvent(int channelNumber, seqwires::ModelDuration timeSinceLastEvent,
                                     const seqwires::TrackEvent& e) {
    assert(channelNumber >= 0);
    assert(channelNumber <= 15);

    if (const seqwires::PercussionKit *const kitIfPercussion = m_channelSetup[channelNumber].m_kitIfPercussion) {
        if (const seqwires::PercussionOnEvent* percussionOn = e.as<seqwires::PercussionOnEvent>()) {
            writeModelDuration(timeSinceLastEvent);
            m_os->put(0b10010000 | channelNumber);
            m_os->put(percussionOn->getPitch());
            m_os->put(percussionOn->getVelocity());
            return WriteTrackEventResult::Written;
        } else if (const seqwires::PercussionOffEvent* percussionOff = e.as<seqwires::PercussionOffEvent>()) {
            writeModelDuration(timeSinceLastEvent);
            m_os->put(0b10000000 | channelNumber);
            m_os->put(percussionOff->getPitch());
            m_os->put(percussionOff->getVelocity());
            return WriteTrackEventResult::Written;
        } 
    } else {
        if (const seqwires::NoteOnEvent* noteOn = e.as<seqwires::NoteOnEvent>()) {
            writeModelDuration(timeSinceLastEvent);
            m_os->put(0b10010000 | channelNumber);
            m_os->put(noteOn->m_pitch);
            m_os->put(noteOn->m_velocity);
            return WriteTrackEventResult::Written;
        } else if (const seqwires::NoteOffEvent* noteOff = e.as<seqwires::NoteOffEvent>()) {
            writeModelDuration(timeSinceLastEvent);
            m_os->put(0b10000000 | channelNumber);
            m_os->put(noteOff->m_pitch);
            m_os->put(noteOff->m_velocity);
            return WriteTrackEventResult::Written;
        }
    }
    return WriteTrackEventResult::WrongCategory;
}

namespace {
    struct NoteTrackIterator : seqwires::FilteredTrackIterator<> {
        bool isEventOfInterest(const seqwires::TrackEvent& event) const { return event.as<seqwires::NoteEvent>(); }
    };

    struct PercussionTrackIterator : seqwires::FilteredTrackIterator<> {
        bool isEventOfInterest(const seqwires::TrackEvent& event) const {
            return event.as<seqwires::PercussionEvent>();
        }
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

        bool isFirstEventAtThisTime = true;
        for (int i = 0; i < channelGroup.getNumTracks(); ++i) {
            const smf::target::ChannelTrackFeature& channelTrack = channelGroup.getTrack(i);
            traversers[i].advance(timeToNextEvent, [this, &isFirstEventAtThisTime, &timeToNextEvent, &timeOfLastEvent,
                                                    &timeSinceStart, &channelTrack](const seqwires::TrackEvent& event) {
                const seqwires::ModelDuration timeToThisEvent = isFirstEventAtThisTime ? timeToNextEvent : 0;
                const WriteTrackEventResult result = writeTrackEvent(channelTrack.m_channelNum->get(), timeToThisEvent, event);
                if (result == WriteTrackEventResult::Written) {
                    timeOfLastEvent = timeSinceStart + timeToNextEvent;
                    isFirstEventAtThisTime = false;
                } else {
                    // TODO Warn user about events which could not be written.
                    m_userLogger.logWarning() << "Event could not be written";
                }
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

void smf::SmfWriter::setUpPercussionKit(std::vector<const seqwires::Track*> tracks, int channelNumber) {
    if (channelNumber == 9) {
        // TODO - Use track contents. Select from set of kits.
        const GMSpecType::Value spec = m_smfFormatFeature.getMidiMetadata().getSpecFeature()->getAsValue();
        switch (spec) {
            case GMSpecType::Value::GM:
                m_channelSetup[9].m_kitIfPercussion = m_knownKits[GM_PERCUSSION_KIT];
                break;
            case GMSpecType::Value::GM2:
                m_channelSetup[9].m_kitIfPercussion = m_knownKits[GM2_STANDARD_PERCUSSION_KIT];
                break;
            case GMSpecType::Value::NONE:
            default:
                break;
        }
    }
}

void smf::SmfWriter::setUpPercussionKits() {
    std::array<std::vector<const seqwires::Track*>, 16> tracksForChannel;
    const int numChannelGroups = m_smfFormatFeature.getNumMidiTracks();
    for (int i = 0; i < numChannelGroups; ++i) {
        const smf::target::ChannelGroup& channelGroup = m_smfFormatFeature.getMidiTrack(i);
        const int numTracks = channelGroup.getNumTracks();
        for (int j = 0; j < numTracks; ++j) {
            const smf::target::ChannelTrackFeature& channelAndTrack = channelGroup.getTrack(j);
            tracksForChannel[channelAndTrack.getChannelNumber()].emplace_back(&channelAndTrack.getTrack());
        }
    }
    for (int i = 0; i < 16; ++i) {
        setUpPercussionKit(tracksForChannel[i], i);
    }
}

void smf::SmfWriter::write() {
    writeHeaderChunk();
    setUpPercussionKits();

    const int numTracks = m_smfFormatFeature.getNumMidiTracks();
    if (m_smfFormatFeature.getSelectedTagIndex() == 0) {
        writeTrack(&m_smfFormatFeature.getMidiTrack(0), m_smfFormatFeature.getMidiMetadata());
    } else {
        writeTrack(&m_smfFormatFeature.getMidiTrack(0), m_smfFormatFeature.getMidiMetadata());
        for (int i = 1; i < numTracks; ++i) {
            MidiMetadata dummyMetadata;
            writeTrack(&m_smfFormatFeature.getMidiTrack(i), dummyMetadata);
        }
    }
}

void smf::writeToSmf(const babelwires::ProjectContext& projectContext, babelwires::UserLogger& userLogger,
                     const target::SmfFormatFeature& smfFormatFeature, std::ostream& output) {
    smf::SmfWriter writer(projectContext, userLogger, smfFormatFeature, output);
    writer.write();
}

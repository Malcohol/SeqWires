/**
 * Write a Standard MIDI File based on the contents of a tree of Features.
 * 
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include "BabelWiresPlugins/Smf/smfWriter.hpp"

#include "SeqWiresLib/Features/trackFeature.hpp"
#include "SeqWiresLib/Utilities/filteredTrackIterator.hpp"
#include "SeqWiresLib/Utilities/musicUtilities.hpp"
#include "SeqWiresLib/Utilities/trackTraverser.hpp"

#include <algorithm>
#include <sstream>

smf::SmfWriter::SmfWriter(std::ostream& ostream)
    : m_ostream(ostream)
    , m_os(&m_ostream)
    , m_division(256) {}

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
    m_os->put(0x00);
    m_os->put(0xff);
    m_os->put(0x51);
    m_os->put(0x03);

    const int d = 60'000'000 / bpm;

    writeUint24(d);
}

void smf::SmfWriter::writeTextMetaEvent(int type, std::string text) {
    assert((0 >= type) && (type <= 15) && "Type is out-of-range.");
    babelwires::Byte t = type;
    m_os->put(0x00);
    m_os->put(0xff);
    m_os->put(t);
    writeVariableLengthQuantity(text.length());

    // TODO assert text is ASCII.
    *m_os << text;
}

void smf::SmfWriter::writeHeaderChunk(const target::SmfFeature& sequence) {
    int numTracks = sequence.getNumMidiTracks();
    if (sequence.getFormat() == smf::target::SmfFeature::SMF_FORMAT_0) {
        // Track 0 holds meta-data.
        ++numTracks;
    }

    assert((numTracks < 16) && "Midi spec allows only 16 channels");
    assert((m_division < (2 << 15)) && "division is too large");

    m_os->write("MThd", 4);
    writeUint32(6);
    writeUint16(int(sequence.getFormat()));
    writeUint16(numTracks);

    {
        int division = 1;
        for (int i = 0; i < sequence.getNumMidiTracks(); ++i) {
            const target::ChannelGroup& channelGroup = sequence.getMidiTrack(i);
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
    if (const seqwires::NoteOnEvent* noteOn = e.asA<seqwires::NoteOnEvent>()) {
        m_os->put(0b10010000 | channel);
        m_os->put(noteOn->m_pitch);
        m_os->put(noteOn->m_velocity);
    } else if (const seqwires::NoteOffEvent* noteOff = e.asA<seqwires::NoteOffEvent>()) {
        m_os->put(0b10000000 | channel);
        m_os->put(noteOff->m_pitch);
        m_os->put(noteOff->m_velocity);
    } else {
        // TODO
        assert(!"Unrecognized note event encountered");
    }
}

namespace {

    /// Only noteOn and noteOff events are understood.
    bool noteOnOffFilter(const seqwires::TrackEvent& event) {
        if (event.asA<seqwires::NoteOnEvent>()) {
            return true;
        } else if (event.asA<seqwires::NoteOffEvent>()) {
            return true;
        } else {
            return false;
        }
    }

} // namespace

void smf::SmfWriter::writeNotes(const target::ChannelGroup& channelGroup) {
    const int numChannels = channelGroup.getNumTracks();

    seqwires::ModelDuration trackDuration = 0;
    std::vector<seqwires::TrackTraverser<seqwires::FilteredTrackIterator<seqwires::NoteEvent>>> traversers;

    for (int i = 0; i < channelGroup.getNumTracks(); ++i) {
        const smf::target::ChannelTrackFeature& channelTrack = channelGroup.getTrack(i);
        const seqwires::Track& track = channelTrack.m_trackFeature->get();
        traversers.emplace_back(track, seqwires::iterateOver<seqwires::NoteEvent>(track));
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
                                                    &timeSinceStart, &channelTrack](const seqwires::NoteEvent& event) {
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

void smf::SmfWriter::writeTrack(const target::ChannelGroup* channelGroup, const seqwires::TempoFeature* tempo,
                                    const babelwires::StringFeature* copyright,
                                    const babelwires::StringFeature* sequenceOrTrackName) {
    std::ostream* oldStream = m_os;
    std::ostringstream tempStream;
    m_os = &tempStream;

    if (copyright) {
        std::string ctext = copyright->get();
        if (!ctext.empty()) {
            writeTextMetaEvent(2, ctext);
        }
    }
    if (sequenceOrTrackName) {
        std::string ntext = sequenceOrTrackName->get();
        if (!ntext.empty()) {
            writeTextMetaEvent(3, ntext);
        }
    }
    if (tempo) {
        writeTempoEvent(tempo->get());
    }
    if (channelGroup) {
        writeNotes(*channelGroup);
    } else {
        writeModelDuration(0);
    }

    // End of track.
    m_os->put(0xff);
    m_os->put(0x2F);
    m_os->put(0x00);

    m_os = oldStream;
    m_os->write("MTrk", 4);
    writeUint32(tempStream.tellp());
    m_os->write(tempStream.str().data(), tempStream.tellp());
}

void smf::writeToSmfFormat0(std::ostream& output, const smf::target::Format0SmfFeature& sequence) {
    const int numTracks = sequence.getNumMidiTracks();
    smf::SmfWriter writer(output);
    writer.writeHeaderChunk(sequence);
    writer.writeTrack(&sequence.getMidiTrack(0), sequence.getTempoFeature(), sequence.getCopyright(),
                            sequence.getSequenceName());
}

void smf::writeToSmfFormat1(std::ostream& output, const smf::target::Format1SmfFeature& sequence) {
    const int numTracks = sequence.getNumMidiTracks();
    smf::SmfWriter writer(output);
    writer.writeHeaderChunk(sequence);
    writer.writeTrack(nullptr, sequence.getTempoFeature(), sequence.getCopyright(),
                            sequence.getSequenceName());
    for (int i = 0; i < numTracks; ++i) {
        writer.writeTrack(&sequence.getMidiTrack(i), nullptr, nullptr, nullptr);
    }
}

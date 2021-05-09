/**
 * Parse a Standard MIDI File into a tree of Features.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include "BabelWiresPlugins/Smf/smfParser.hpp"
#include "Common/exceptions.hpp"
#include "SeqWiresLib/Features/trackFeature.hpp"
#include "SeqWiresLib/Tracks/noteEvents.hpp"

#include <cassert>

smf::SmfParser::SmfParser(babelwires::DataSource& dataSource)
    : m_dataSource(dataSource)
    , m_sequenceType(source::SmfFeature::SMF_UNKNOWN_FORMAT)
    , m_numTracks(-1)
    , m_division(-1) {}

babelwires::Byte smf::SmfParser::getNext() {
    try {
        return m_dataSource.getNextByte();
    } catch (std::exception& e) {
        throw babelwires::ParseException() << "Stream is truncated";
    }
}

babelwires::Byte smf::SmfParser::peekNext() {
    try {
        return m_dataSource.peekNextByte();
    } catch (std::exception& e) {
        throw babelwires::ParseException() << "Stream is truncated";
    }
}

void smf::SmfParser::readByteSequence(const char* seq) {
    assert(seq);
    while (*seq) {
        const babelwires::Byte c = getNext();
        if (c != *seq) {
            throw babelwires::ParseException()
                << "Expected " << *seq << " at index " << m_dataSource.getAbsolutePosition() << " but found " << c
                << " instead";
        }
        ++seq;
    }
}

void smf::SmfParser::skipBytes(int numBytes) {
    for (int i = 0; i < numBytes; ++i) {
        getNext();
    }
}

std::uint16_t smf::SmfParser::readU16() {
    const std::uint32_t b0 = getNext();
    const std::uint32_t b1 = getNext();
    return (b0 << 8) | b1;
}

std::uint32_t smf::SmfParser::readU24() {
    const std::uint32_t b0 = getNext();
    const std::uint32_t b1 = getNext();
    const std::uint32_t b2 = getNext();
    return (b0 << 16) | (b1 << 8) | b2;
}

std::uint32_t smf::SmfParser::readU32() {
    const std::uint32_t b0 = getNext();
    const std::uint32_t b1 = getNext();
    const std::uint32_t b2 = getNext();
    const std::uint32_t b3 = getNext();
    return (b0 << 24) | (b1 << 16) | (b2 << 8) | b3;
}

std::uint32_t smf::SmfParser::readVariableLengthQuantity() {
    std::uint32_t result = 0;
    babelwires::Byte b;
    int numBytes = 0;
    do {
        if (numBytes == 4) {
            throw babelwires::ParseException() << "Variable Length Quantity too big";
        }
        ++numBytes;

        b = getNext();
        result = (result << 7) + (b & 0x7f);
    } while (b & 0x80);
    return result;
}

seqwires::ModelDuration smf::SmfParser::readModelDuration() {
    const int numDivisions = readVariableLengthQuantity();
    return seqwires::ModelDuration(numDivisions) * seqwires::ModelDuration(1, m_division * 4);
}

std::string smf::SmfParser::readTextMetaEvent(int length) {
    std::vector<char> text;
    for (int i = 0; i < length; ++i) {
        text.push_back(getNext());
    }
    return std::string(text.data(), length);
}

void smf::SmfParser::readHeaderChunk() {
    readByteSequence("MThd");
    if (readU32() != 6) {
        throw babelwires::ParseException() << "Header chunk not expected length";
    }
    m_sequenceType = source::SmfFeature::Format(readU16());
    if (m_sequenceType > 2) {
        throw babelwires::ParseException() << "Not a known type of Standard MIDI File";
    }
    m_numTracks = readU16();
    m_division = readU16();
    if (m_division & (1 << 15)) {
        throw babelwires::ParseException() << "SMPTE format durations not supported";
    }
}

smf::source::SmfFeature::Format smf::SmfParser::getSequenceType(babelwires::DataSource& dataSource) {
    try {
        dataSource.setRewindPoint(16);
        SmfParser parser(dataSource);
        parser.readHeaderChunk();
        dataSource.rewind();
        return parser.m_sequenceType;
    } catch (const std::exception& e) {
        return source::SmfFeature::SMF_UNKNOWN_FORMAT;
    }
}

void smf::SmfParser::parse() {
    readHeaderChunk();
    switch (m_sequenceType) {
        case source::SmfFeature::SMF_FORMAT_0: {
            auto seqPtr = std::make_unique<source::Format0SmfFeature>();
            source::Format0SmfFeature& seq = *seqPtr;
            m_result = std::move(seqPtr);
            readFormat0Sequence(seq);
            break;
        }
        case source::SmfFeature::SMF_FORMAT_1: {
            auto seqPtr = std::make_unique<source::Format1SmfFeature>();
            source::Format1SmfFeature& seq = *seqPtr;
            m_result = std::move(seqPtr);
            readFormat1Sequence(seq);
            break;
        }
        case source::SmfFeature::SMF_FORMAT_2: {
            // TODO
        }
        case source::SmfFeature::SMF_UNKNOWN_FORMAT:
        default: {
            throw babelwires::ParseException() << "The data is not in one of the understood sequence types";
        }
    }
}

void smf::SmfParser::readTempoEvent(seqwires::TempoFeature& tempo) {
    const int d = readU24();
    const int bpm = 60'000'000 / d;
    tempo.set(bpm);
}

namespace {
    class TrackSplitter {
      public:
        TrackSplitter()
            : m_channels{} {}

        template <typename EVENT_TYPE>
        void addToChannel(int c, seqwires::ModelDuration timeSinceLastTrackEvent, EVENT_TYPE& event) {
            seqwires::Track* channel = getChannel(c);

            m_timeSinceStart += timeSinceLastTrackEvent;
            event.setTimeSinceLastEvent(m_timeSinceStart - m_timeOfLastEvent[c]);
            m_timeOfLastEvent[c] = m_timeSinceStart;

            channel->addEvent(event);
        }

        /// All channels share the duration of the MIDI track.
        void setDurationsForAllChannels(seqwires::ModelDuration timeToEndOfTrackEvent) {
            seqwires::ModelDuration duration = m_timeSinceStart + timeToEndOfTrackEvent;
            for (int c = 0; c < MAX_CHANNELS; ++c) {
                if (m_channels[c] != nullptr) {
                    m_channels[c]->setDuration(duration);
                }
            }
        }

        void assignChannelsToChannelGroup(smf::source::ChannelGroup& channels) {
            // If this is a format 1 track with multiple channels, then privilege the
            // channel with the most events.
            int privilegedTrack = -1;
            int maxNumEvents = 0;
            for (int c = 0; c < MAX_CHANNELS; ++c) {
                if ((m_channels[c] != nullptr) && (m_channels[c]->getNumEvents() > maxNumEvents)) {
                    privilegedTrack = c;
                    maxNumEvents = m_channels[c]->getNumEvents();
                }
            }
            if (privilegedTrack != -1) {
                channels.setPrivilegedTrack(privilegedTrack);
                for (int c = 0; c < MAX_CHANNELS; ++c) {
                    if (m_channels[c] != nullptr) {
                        channels.addTrack(c)->set(std::move(m_channels[c]));
                    }
                }
            }
        }

      private:
        seqwires::Track* getChannel(int c) {
            assert(0 <= c);
            assert(c < MAX_CHANNELS);

            if (m_channels[c] == nullptr) {
                m_channels[c] = std::make_unique<seqwires::Track>();
            }
            return m_channels[c].get();
        }

      private:
        static const int MAX_CHANNELS = 16;

        seqwires::ModelDuration m_timeSinceStart;
        seqwires::ModelDuration m_timeOfLastEvent[MAX_CHANNELS];
        std::unique_ptr<seqwires::Track> m_channels[MAX_CHANNELS];
    };
} // namespace

void smf::SmfParser::readTrack(int i, source::ChannelGroup& channels, source::MidiMetadata& metadata) {
    readByteSequence("MTrk");
    const std::uint32_t trackLength = readU32();
    const int currentIndex = m_dataSource.getAbsolutePosition();

    TrackSplitter tracks;

    seqwires::ModelDuration timeSinceLastNoteEvent = 0;
    babelwires::Byte lastStatusByte = 0;
    while ((m_dataSource.getAbsolutePosition() - currentIndex) < trackLength) {
        timeSinceLastNoteEvent += readModelDuration();

        // Peek in case running status should be used.
        babelwires::Byte statusByte = peekNext();
        if ((statusByte & 0x80) || (lastStatusByte == 0)) {
            // A new status byte, so consume the status byte.
            getNext();
        } else {
            // Running status.
            statusByte = lastStatusByte;
        }
        lastStatusByte = statusByte;

        const babelwires::Byte statusHi = statusByte >> 4;
        const babelwires::Byte statusLo = statusByte & 0xf;
        switch (statusHi) {
            case 0b1111: {
                // No running status allowed.
                lastStatusByte = 0;
                if ((statusLo == 0) || (statusLo == 7)) {
                    // SysEx event.
                    skipBytes(readVariableLengthQuantity());
                } else if (statusLo == 0xf) {
                    // Meta-event.
                    const babelwires::Byte type = getNext();
                    const std::uint32_t length = readVariableLengthQuantity();
                    switch (type) {
                        case 0x51: // Set tempo
                        {
                            readTempoEvent(metadata.getActivatedTempoFeature());
                            break;
                        }
                        case 0x2F: // End of track.
                        {
                            // Finished.
                            if ((m_dataSource.getAbsolutePosition() - currentIndex) != trackLength) {
                                throw babelwires::ParseException()
                                    << "MIDI track " << i << " had an unexpected end-of-track event";
                            }
                            tracks.setDurationsForAllChannels(timeSinceLastNoteEvent);
                            tracks.assignChannelsToChannelGroup(channels);
                            return;
                        }
                        case 0x02: // Copyright
                        {
                            metadata.getActivatedCopyright().set(readTextMetaEvent(length));
                            break;
                        }
                        case 0x03: // Sequence or track name.
                        {
                            metadata.getActivatedSequenceName().set(readTextMetaEvent(length));
                            break;
                        }
                        default: // Ignored.
                            skipBytes(length);
                            break;
                    }
                } else {
                    throw babelwires::ParseException()
                        << "Unrecognized MIDI message with status byte " << static_cast<int>(statusByte);
                }
                break;
            }
            case 0b1000: // Note off.
            {
                seqwires::NoteOffEvent noteOff;
                noteOff.m_pitch = getNext();
                noteOff.m_velocity = getNext();
                tracks.addToChannel(statusLo, timeSinceLastNoteEvent, noteOff);
                timeSinceLastNoteEvent = 0;
                break;
            }
            case 0b1001: // Note on.
            {
                seqwires::NoteOnEvent noteOn;
                noteOn.m_pitch = getNext();
                noteOn.m_velocity = getNext();
                tracks.addToChannel(statusLo, timeSinceLastNoteEvent, noteOn);
                timeSinceLastNoteEvent = 0;
                break;
            }
            case 0b1010: // Polyphonic key pressure Aftertouch.
            case 0b1011: // Control change.
            case 0b1110: // Pitch wheel
            {
                skipBytes(2);
                break;
            }
            case 0b1100: // Program change
            case 0b1101: // Channel pressure
            {
                skipBytes(1);
                break;
            }
            default: {
                throw babelwires::ParseException()
                    << "Unrecognized MIDI message with status byte " << static_cast<int>(statusByte);
            }
        }
    }

    // We've read too far.
    { throw babelwires::ParseException() << "Read all of track " << i << " without finding an end-of-track event"; }
}

void smf::SmfParser::readFormat0Sequence(source::Format0SmfFeature& sequence) {
    if (m_numTracks != 1) {
        throw babelwires::ParseException()
            << "A format 0 claims to have " << m_numTracks << " tracks but it should only have 1";
    }
    source::ChannelGroup* midiTrack = sequence.getMidiTrack0();
    readTrack(0, *midiTrack, sequence.getMidiMetadata());
}

void smf::SmfParser::readFormat1Sequence(source::Format1SmfFeature& sequence) {
    source::ChannelGroup* midiTrack = sequence.addMidiTrack();
    readTrack(0, *midiTrack, sequence.getMidiMetadata());
    for (int i = 1; i < m_numTracks; ++i) {
        source::ChannelGroup* midiTrack = sequence.addMidiTrack();
        source::MidiMetadata dummyMetadata;
        readTrack(i, *midiTrack, dummyMetadata);
    }
}

std::unique_ptr<babelwires::FileFeature> smf::parseSmfSequence(babelwires::DataSource& dataSource) {
    SmfParser parser(dataSource);
    parser.parse();
    return parser.getResult();
}

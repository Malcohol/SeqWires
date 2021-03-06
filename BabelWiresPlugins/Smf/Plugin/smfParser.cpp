/**
 * Parse a Standard MIDI File into a tree of Features.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <BabelWiresPlugins/Smf/Plugin/smfParser.hpp>

#include <SeqWiresLib/Features/trackFeature.hpp>
#include <SeqWiresLib/Tracks/noteEvents.hpp>

#include <Common/Log/debugLogger.hpp>
#include <Common/exceptions.hpp>

#include <cassert>
#include <cmath>
#include <iomanip>

smf::SmfParser::SmfParser(babelwires::DataSource& dataSource, const babelwires::ProjectContext& projectContext,
                          babelwires::UserLogger& userLogger)
    : m_projectContext(projectContext)
    , m_dataSource(dataSource)
    , m_userLogger(userLogger)
    , m_sequenceType(source::SmfFeature::SMF_UNKNOWN_FORMAT)
    , m_numTracks(-1)
    , m_division(-1) {}

babelwires::Byte smf::SmfParser::getNext() {
    try {
        return m_dataSource.getNextByte();
    } catch (std::exception&) {
        throw babelwires::ParseException() << "Stream is truncated";
    }
}

babelwires::Byte smf::SmfParser::peekNext() {
    try {
        return m_dataSource.peekNextByte();
    } catch (std::exception&) {
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

void smf::SmfParser::parse() {
    readHeaderChunk();
    switch (m_sequenceType) {
        case source::SmfFeature::SMF_FORMAT_0: {
            auto seqPtr = std::make_unique<source::Format0SmfFeature>(m_projectContext);
            source::Format0SmfFeature& seq = *seqPtr;
            m_result = std::move(seqPtr);
            readFormat0Sequence(seq);
            break;
        }
        case source::SmfFeature::SMF_FORMAT_1: {
            auto seqPtr = std::make_unique<source::Format1SmfFeature>(m_projectContext);
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
    const double d = readU24();
    const double bpm = 60'000'000 / d;
    tempo.set(std::round(bpm));
}

namespace {
    class TrackSplitter {
      public:
        TrackSplitter()
            : m_channels{} {}

        template <typename EVENT_TYPE>
        void addToChannel(int c, seqwires::ModelDuration timeSinceLastTrackEvent, EVENT_TYPE&& event) {
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

template <typename STREAMLIKE> void smf::SmfParser::logByteSequence(STREAMLIKE log, int length) {
    log << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(getNext());
    for (auto i = 0; i < length - 1; ++i) {
        log << ", " << std::setfill('0') << std::setw(2) << static_cast<int>(getNext());
    }
}

void smf::SmfParser::readSequencerSpecificEvent(int length) {
    if (length <= 1) {
        logByteSequence(m_userLogger.logWarning() << "Skipping sequencer specific event with invalid length: ", length);
        return;
    }
    assert(length <= 255 && "Length was expected to be held in one byte");
    std::array<babelwires::Byte, 255> eventBytes;
    for (int i = 0; i < length; ++i) {
        eventBytes[i] = getNext();
    }
    auto log = babelwires::logDebug();
    int byteIndex = 0;
    if (eventBytes[0] == 0x43) {
        if ((length >= 3) && (eventBytes[1] == 0x7B)) {
            // XF Events
            byteIndex = 3;
            switch (eventBytes[2]) {
                case 0x00:
                    log << "Ignored XF Version ID: ";
                    break;
                case 0x01:
                    log << "Ignored XF Chord Event: ";
                    break;
                case 0x02:
                    log << "Ignored XF Rehearsal Mark: ";
                    break;
                case 0x03:
                    log << "Ignored XF Phrase Mark: ";
                    break;
                case 0x04:
                    log << "Ignored XF Max Phrase Mark: ";
                    break;
                case 0x05:
                    log << "Ignored XF Fingered Number: ";
                    break;
                case 0x0C:
                    log << "Ignored XF Guide Track Flag: ";
                    break;
                case 0x10:
                    log << "Ignored XF Information Flag for Guitar: ";
                    break;
                case 0x12:
                    log << "Ignored XF Chord Voicing for Guitar: ";
                    break;
                case 0x7F:
                    log << "Ignored XF Song Data Number: ";
                    break;
                default:
                    log << "Ignored unrecognized XF event: ";
                    byteIndex = 2;
                    break;
            }
        } else if ((length >= 5) && (eventBytes[1] == 0x73)) {
            // Yamaha META event: See CVP900 Data List manual.
            byteIndex = 5;
            if (eventBytes[2] == 0x0A) {
                switch (static_cast<unsigned int>(eventBytes[3] << 8) + eventBytes[4]) {
                    case 0x0004:
                        log << "Ignored Yamaha Meta-event Start Measure Number: ";
                        break;
                    case 0x0005:
                        log << "Ignored Yamaha Meta-event Track Information: ";
                        break;
                    case 0x0006:
                        log << "Ignored Yamaha Meta-event Offset Volume: ";
                        break;
                    case 0x0007:
                        log << "Ignored Yamaha Meta-event Song Offset Measure: ";
                        break;
                    default:
                        log << "Ignored Yamaha Meta-event XF event: ";
                        byteIndex = 2;
                        break;
                }                
            } else if (eventBytes[2] == 0x0C) {
                log << "Ignored Yamaha Meta-event Style Name: ";
                byteIndex = 3;
            } else if (eventBytes[2] == 0x0D) {
                log << "Ignored Yamaha Meta-event Song OTS: ";
                byteIndex = 3;
            } else {
                byteIndex = 2;
                log << "Ignored unrecognized Yamaha Meta-event: ";
            }
        } else {
            log << "Ignored Yamaha sequencer specific meta-event: ";
            byteIndex = 2;
        }
    } else {
        log << "Ignored sequencer specific meta-event: ";
    }
    log << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(eventBytes[byteIndex]);
    for (auto i = byteIndex + 1; i < length; ++i) {
        log << ", " << std::setfill('0') << std::setw(2) << static_cast<int>(eventBytes[i]);
    }
}

void smf::SmfParser::readTrack(int i, source::ChannelGroup& channels, MidiMetadata& metadata) {
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

        // Implementation note:
        // I use the debug log where an event is valid, but I haven't implemented support for it yet.
        // I use the user log where an event is invalid, so it could never be parsed.

        const babelwires::Byte statusHi = statusByte >> 4;
        const babelwires::Byte statusLo = statusByte & 0xf;
        switch (statusHi) {
            case 0b1111: {
                // No running status allowed.
                lastStatusByte = 0;
                if ((statusLo == 0) || (statusLo == 7)) {
                    // SysEx event.
                    auto length = readVariableLengthQuantity();
                    logByteSequence(babelwires::logDebug() << "Ignored sysex event! ", length);
                } else if (statusLo == 0xf) {
                    // Meta-event.
                    const babelwires::Byte type = getNext();
                    const std::uint32_t length = readVariableLengthQuantity();
                    switch (type) {
                        case 0x00: // Sequence number
                        {
                            if (length != 2) {
                                logByteSequence(m_userLogger.logWarning()
                                                    << "Skipping sequence number meta-event with incorrect length: ",
                                                length);
                            } else {
                                babelwires::logDebug() << "Ignored meta-event! Sequence number: " << readU16();
                            }
                            break;
                        }
                        case 0x01: // Text event
                        {
                            babelwires::logDebug() << "Ignored meta-event! Text event: " << readTextMetaEvent(length);
                            break;
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
                        case 0x04: // Instrument name
                        {
                            babelwires::logDebug()
                                << "Ignored MIDI Event! Instrument name: " << readTextMetaEvent(length);
                            break;
                        }
                        case 0x05: // Lyric
                        {
                            babelwires::logDebug() << "Ignored meta-event! Lyric: " << readTextMetaEvent(length);
                            break;
                        }
                        case 0x06: // Marker
                        {
                            babelwires::logDebug() << "Ignored meta-event! Marker: " << readTextMetaEvent(length);
                            break;
                        }
                        case 0x07: // Cue point
                        {
                            babelwires::logDebug() << "Ignored meta-event! Cue point: " << readTextMetaEvent(length);
                            break;
                        }
                        case 0x20: // Channel prefix
                        {
                            if (length != 1) {
                                logByteSequence(m_userLogger.logWarning()
                                                    << "Skipping channel prefix meta-event with incorrect length",
                                                length);
                            } else {
                                logByteSequence(babelwires::logDebug() << "Ignored meta-event! Channel prefix: ",
                                                length);
                            }
                            break;
                        }
                        case 0x2F: // End of track.
                        {
                            // Finished.
                            if ((m_dataSource.getAbsolutePosition() - currentIndex) != trackLength) {
                                throw babelwires::ParseException()
                                    << "MIDI track " << i << " had an unexpected end-of-track event";
                            }
                            if (length != 0) {
                                // Not a good idea to skip end of track events.
                                logByteSequence(
                                    m_userLogger.logWarning()
                                        << "End of Track meta-event has incorrect length. Will try use it anyway.",
                                    length);
                            }
                            tracks.setDurationsForAllChannels(timeSinceLastNoteEvent);
                            tracks.assignChannelsToChannelGroup(channels);
                            return;
                        }
                        case 0x51: // Set tempo
                        {
                            if (length != 3) {
                                logByteSequence(m_userLogger.logWarning()
                                                    << "Skipping Tempo meta-event with incorrect length",
                                                length);
                            } else {
                                readTempoEvent(metadata.getActivatedTempoFeature());
                            }
                            break;
                        }
                        case 0x54: // SMPTE offset
                        {
                            if (length != 5) {
                                logByteSequence(m_userLogger.logWarning()
                                                    << "Skipping SMPTE Offset meta-event with incorrect length",
                                                length);
                            } else {
                                logByteSequence(babelwires::logDebug() << "Ignored meta-event! SMPTE Offset: ", length);
                            }
                            break;
                        }
                        case 0x58: // Time signature
                        {
                            if (length != 4) {
                                logByteSequence(m_userLogger.logWarning()
                                                    << "Skipping Time Signature meta-event with incorrect length",
                                                length);
                            } else {
                                logByteSequence(babelwires::logDebug() << "Ignored meta-event! Time signature: ",
                                                length);
                            }
                            break;
                        }
                        case 0x59: // Key signature
                        {
                            if (length != 2) {
                                logByteSequence(m_userLogger.logWarning()
                                                    << "Skipping Key Signature event with incorrect length",
                                                length);
                            } else {
                                logByteSequence(babelwires::logDebug() << "Ignored meta-event! Key signature: ",
                                                length);
                            }
                            break;
                        }
                        case 0x7F: // Sequence specific event
                        {
                            readSequencerSpecificEvent(length);
                            break;
                        }
                        default: // Unknown meta-event type
                        {
                            // This isn't in the spec, so warn: Perhaps SeqWires is out-of-date.
                            logByteSequence(m_userLogger.logWarning()
                                                << "Skipping unknown meta-event of type " << std::hex << type << ": ",
                                            length);
                            break;
                        }
                    }
                } else {
                    throw babelwires::ParseException()
                        << "Unrecognized MIDI message with status byte " << static_cast<int>(statusByte);
                }
                break;
            }
            case 0b1000: // Note off.
            {
                const seqwires::Pitch pitch = getNext();
                const seqwires::Velocity velocity = getNext();
                tracks.addToChannel(statusLo, timeSinceLastNoteEvent, seqwires::NoteOffEvent{0, pitch, velocity});
                timeSinceLastNoteEvent = 0;
                break;
            }
            case 0b1001: // Note on.
            {
                const seqwires::Pitch pitch = getNext();
                const seqwires::Velocity velocity = getNext();
                if (velocity != 0) {
                    tracks.addToChannel(statusLo, timeSinceLastNoteEvent, seqwires::NoteOnEvent{0, pitch, velocity});
                } else {
                    tracks.addToChannel(statusLo, timeSinceLastNoteEvent, seqwires::NoteOffEvent{0, pitch});
                }
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
        MidiMetadata dummyMetadata;
        readTrack(i, *midiTrack, dummyMetadata);
    }
}

std::unique_ptr<babelwires::FileFeature> smf::parseSmfSequence(babelwires::DataSource& dataSource,
                                                               const babelwires::ProjectContext& projectContext,
                                                               babelwires::UserLogger& userLogger) {
    SmfParser parser(dataSource, projectContext, userLogger);
    parser.parse();
    return parser.getResult();
}

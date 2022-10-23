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
#include <SeqWiresLib/percussion.hpp>

#include <BabelWiresLib/Project/projectContext.hpp>
#include <BabelWiresLib/TypeSystem/typeSystem.hpp>

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
    , m_division(-1) {
    const seqwires::PercussionKit& gmKit =
        projectContext.m_typeSystem.getRegisteredEntry(seqwires::GMPercussion::getThisIdentifier())
            .is<seqwires::GMPercussion>();
    const seqwires::PercussionKit& gm2StandardKit =
        projectContext.m_typeSystem.getRegisteredEntry(seqwires::GM2Percussion::getThisIdentifier())
            .is<seqwires::GM2Percussion>();
    m_knownKits[GM_PERCUSSION_KIT] = &gmKit;
    m_knownKits[GM2_STANDARD_PERCUSSION_KIT] = &gm2StandardKit;
}

smf::SmfParser::~SmfParser() = default;

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

class smf::SmfParser::TrackSplitter {
  public:
    TrackSplitter(const std::array<ChannelSetup, 16>& channelSetup)
        : m_channels{}
        , m_channelSetup(channelSetup) {}

    void addNoteOn(unsigned int channelNumber, seqwires::ModelDuration timeSinceLastTrackEvent, seqwires::Pitch pitch, seqwires::Velocity velocity) {
        //if (m_channelSetup[channelNumber].m_kitIfPercussion) { TODO }
        addToChannel<seqwires::NoteOnEvent>(channelNumber, timeSinceLastTrackEvent, pitch, velocity);
    }

    void addNoteOff(unsigned int channelNumber, seqwires::ModelDuration timeSinceLastTrackEvent, seqwires::Pitch pitch, seqwires::Velocity velocity) {
        //if (m_channelSetup[channelNumber].m_kitIfPercussion) { TODO }
        addToChannel<seqwires::NoteOffEvent>(channelNumber, timeSinceLastTrackEvent, pitch, velocity);
    }

    /// All channels share the duration of the MIDI track.
    void setDurationsForAllChannels(seqwires::ModelDuration timeToEndOfTrackEvent) {
        seqwires::ModelDuration duration = m_timeSinceStart + timeToEndOfTrackEvent;
        for (int channelNumber = 0; channelNumber < MAX_CHANNELS; ++channelNumber) {
            if (m_channels[channelNumber] != nullptr) {
                m_channels[channelNumber]->m_track.setDuration(duration);
            }
        }
    }

    void assignChannelsToChannelGroup(smf::source::ChannelGroup& channels) {
        // If this is a format 1 track with multiple channels, then privilege the
        // channel with the most events.
        int privilegedTrack = -1;
        int maxNumEvents = 0;
        for (int channelNumber = 0; channelNumber < MAX_CHANNELS; ++channelNumber) {
            if ((m_channels[channelNumber] != nullptr) &&
                (m_channels[channelNumber]->m_track.getNumEvents() > maxNumEvents)) {
                privilegedTrack = channelNumber;
                maxNumEvents = m_channels[channelNumber]->m_track.getNumEvents();
            }
        }
        if (privilegedTrack != -1) {
            channels.setPrivilegedTrack(privilegedTrack);
            for (int channelNumber = 0; channelNumber < MAX_CHANNELS; ++channelNumber) {
                if (m_channels[channelNumber] != nullptr) {
                    channels.addTrack(channelNumber)->set(std::move(m_channels[channelNumber]->m_track));
                }
            }
        }
    }

  private:
    struct PerChannelInfo {
        seqwires::ModelDuration m_timeOfLastEvent;
        seqwires::Track m_track;
    };

    PerChannelInfo* getChannel(unsigned int channelNumber) {
        assert(0 <= channelNumber);
        assert(channelNumber < MAX_CHANNELS);
        auto& channel = m_channels[channelNumber];

        if (channel == nullptr) {
            channel = std::make_unique<PerChannelInfo>();
        }
        return channel.get();
    }

    template <typename EVENT_TYPE, typename... ARGS>
    void addToChannel(unsigned int channelNumber, seqwires::ModelDuration timeSinceLastTrackEvent, ARGS&&... args) {
        PerChannelInfo* channel = getChannel(channelNumber);

        m_timeSinceStart += timeSinceLastTrackEvent;
        channel->m_track.addEvent(EVENT_TYPE{m_timeSinceStart - channel->m_timeOfLastEvent, std::forward<ARGS>(args)...});
        channel->m_timeOfLastEvent = m_timeSinceStart;
    }
  private:
    static const int MAX_CHANNELS = 16;

    std::array<const seqwires::PercussionKit*, NUM_KNOWN_PERCUSSION_KITS> m_knownKits;
    GMSpecType::Value m_gmSpec;

    seqwires::ModelDuration m_timeSinceStart;

    std::array<std::unique_ptr<PerChannelInfo>, MAX_CHANNELS> m_channels;

    const std::array<ChannelSetup, 16>& m_channelSetup;
};

template <typename STREAMLIKE> void smf::SmfParser::logByteSequence(STREAMLIKE log, int length) {
    log << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(getNext());
    for (auto i = 0; i < length - 1; ++i) {
        log << ", " << std::setfill('0') << std::setw(2) << static_cast<int>(getNext());
    }
}

void smf::SmfParser::readFullMessageIntoBuffer(std::uint32_t length) {
    m_messageBuffer.resize(length);
    for (int i = 0; i < length; ++i) {
        m_messageBuffer[i] = getNext();
    }
}

template <std::size_t N> bool smf::SmfParser::isMessageBufferMessage(const std::array<std::int16_t, N>& message) const {
    if (m_messageBuffer.size() != message.size()) {
        return false;
    }
    for (int i = 0; i < message.size(); ++i) {
        if (message[i] != -1) {
            if (m_messageBuffer[i] != message[i]) {
                return false;
            }
        }
    }
    return true;
}

template <typename STREAMLIKE> void smf::SmfParser::logMessageBuffer(STREAMLIKE log) const {
    log << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(m_messageBuffer[0]);
    for (auto i = 1; i < m_messageBuffer.size(); ++i) {
        log << ", " << std::setfill('0') << std::setw(2) << static_cast<int>(m_messageBuffer[i]);
    }
}

void smf::SmfParser::readSysExEvent() {
    auto length = readVariableLengthQuantity();
    if (length < 1) {
        logByteSequence(m_userLogger.logWarning() << "Skipping SysEx message with invalid length: ", length);
        return;
    }
    readFullMessageIntoBuffer(length);
    const babelwires::Byte headerId = m_messageBuffer[0];
    if (headerId == 0x7E) {
        // Universal SysEx
        if (length < 4) {
            logByteSequence(m_userLogger.logWarning() << "Skipping Universal SysEx message with invalid length: ",
                            length);
            return;
        }
        // Universal Non-Real Time SysEx
        /*const babelwires::Byte deviceId = m_messageBuffer[1]*/
        const babelwires::Byte subId1 = m_messageBuffer[2];
        const babelwires::Byte subId2 = m_messageBuffer[3];
        if (subId1 == 0x09) {
            // General MIDI message
            if (subId2 == 0x01) {
                // General MIDI On
                setGMSpec(GMSpecType::Value::GM);
                babelwires::logDebug() << "General MIDI On";
            } else if (subId2 == 0x02) {
                // General MIDI Off
                babelwires::logDebug() << "General MIDI Off";
            } else if (subId2 == 0x03) {
                // General MIDI 2 On
                setGMSpec(GMSpecType::Value::GM2);
                babelwires::logDebug() << "General MIDI 2 On";
            } else {
                babelwires::logDebug() << "Ignoring unrecognized General MIDI SysEx message";
            }
            if (m_messageBuffer[4] != 0xF7) {
                m_userLogger.logWarning() << "Improperly terminated General MIDI SysEx message";
            }
            return;
        }
    } else if (headerId == 0x41) {
        // Roland SysEx
        const unsigned int messageSize = m_messageBuffer.size();
        if (m_messageBuffer[messageSize - 1] != 0xF7) {
            m_userLogger.logWarning() << "Improperly terminated Roland SysEx message";
        }
        // Checksum
        babelwires::Byte checkSum = 0;
        for (int i = 4; i < messageSize - 2; ++i) {
            // This can overflow without problems.
            checkSum += m_messageBuffer[i];
        }
        if (m_messageBuffer[messageSize - 2] != (0x80 - (checkSum % 0x80) % 0x80)) {
            m_userLogger.logWarning() << "Ignoring Roland SysEx message with invalid checksum";
            return;
        }
        if (isMessageBufferMessage(
                std::array<std::int16_t, 10>{0x41, -1, 0x42, 0x12, 0x40, 0x00, 0x7F, 0x00, 0x41, 0xF7})) {
            setGMSpec(GMSpecType::Value::GS);
            babelwires::logDebug() << "Roland GS Reset";
            return;
        }
        if (isMessageBufferMessage(std::array<std::int16_t, 10>{0x41, -1, 0x42, 0x12, 0x40, -1, 0x15, -1, -1, 0xF7}) &&
            ((m_messageBuffer[5] & 0xf0) == 0x10)) {
            // Use for rhythm part.
            const babelwires::Byte blockNumber = m_messageBuffer[5] & 0x0f;
            const babelwires::Byte value = m_messageBuffer[7];
            if (value > 2) {
                m_userLogger.logWarning()
                    << "Ignoring Roland SysEx use for rhythm part message with out of range value";
            } else {
                setGsPartMode(blockNumber, value);
            }
            return;
        }
    } else if (headerId == 0x43) {
        // Yamaha SysEx
        if (isMessageBufferMessage(std::array<std::int16_t, 8>{0x43, -1, 0x4C, 0x00, 0x00, 0x7E, 0x00, 0xF7})) {
            setGMSpec(GMSpecType::Value::XG);
            babelwires::logDebug() << "Yamaha XG Reset";
            return;
        }
    }
    logMessageBuffer(babelwires::logDebug() << "Ignoring unrecognized SysEx event! ");
}

void smf::SmfParser::readSysExEventContinuation() {
    auto length = readVariableLengthQuantity();
    logByteSequence(babelwires::logDebug() << "Ignoring continued SysEx message", length);
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

void smf::SmfParser::readControlChange(unsigned int channelNumber) {
    const babelwires::Byte controllerNumber = getNext();
    const babelwires::Byte value = getNext();
    switch (controllerNumber) {
        case 0x00:
            // bank select MSB
            setBankMSB(channelNumber, value);
            break;
        case 0x20:
            // bank select LSB
            setBankLSB(channelNumber, value);
            break;
        default:
            break;
    }
}

void smf::SmfParser::readProgramChange(unsigned int channelNumber) {
    const babelwires::Byte newProgram = getNext();
    setProgram(channelNumber, newProgram);
}

void smf::SmfParser::readTrack(int trackIndex, source::ChannelGroup& channels, MidiMetadata& metadata) {
    readByteSequence("MTrk");
    const std::uint32_t trackLength = readU32();
    const int currentIndex = m_dataSource.getAbsolutePosition();

    TrackSplitter tracks(m_channelSetup);

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
        // I use the user log where an event is invalid, so it could never be parsed correctly.

        const babelwires::Byte statusHi = statusByte >> 4;
        const babelwires::Byte statusLo = statusByte & 0xf;
        switch (statusHi) {
            case 0b1111: {
                // No running status allowed.
                lastStatusByte = 0;
                if (statusLo == 0x00) {
                    readSysExEvent();
                } else if (statusLo == 0x07) {
                    readSysExEventContinuation();
                } else if (statusLo == 0x0f) {
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
                                    << "MIDI track " << trackIndex << " had an unexpected end-of-track event";
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
                tracks.addNoteOff(statusLo, timeSinceLastNoteEvent, pitch, velocity);
                timeSinceLastNoteEvent = 0;
                break;
            }
            case 0b1001: // Note on.
            {
                const seqwires::Pitch pitch = getNext();
                const seqwires::Velocity velocity = getNext();
                if (velocity != 0) {
                    tracks.addNoteOn(statusLo, timeSinceLastNoteEvent, pitch, velocity);
                } else {
                    tracks.addNoteOff(statusLo, timeSinceLastNoteEvent, pitch, velocity);
                }
                timeSinceLastNoteEvent = 0;
                break;
            }
            case 0b1010: // Polyphonic key pressure Aftertouch.
            {
                skipBytes(2);
                break;
            }
            case 0b1011: // Control change.
            {
                readControlChange(statusLo);
                break;
            }
            case 0b1110: // Pitch wheel
            {
                skipBytes(2);
                break;
            }
            case 0b1100: // Program change
            {
                readProgramChange(statusLo);
                break;
            }
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
    throw babelwires::ParseException() << "Read all of track " << trackIndex
                                       << " without finding an end-of-track event";
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

void smf::SmfParser::setGMSpec(GMSpecType::Value spec) {
    switch (spec) {
        case GMSpecType::Value::GM:
        case GMSpecType::Value::GS:
        case GMSpecType::Value::XG:
            m_channelSetup[9].m_kitIfPercussion = m_knownKits[GM_PERCUSSION_KIT];
            break;
        case GMSpecType::Value::GM2:
            m_channelSetup[9].m_kitIfPercussion = m_knownKits[GM2_STANDARD_PERCUSSION_KIT];
            break;
        case GMSpecType::Value::NONE:
        default:
            break;
    }
    m_gmSpec = spec;
}

void smf::SmfParser::setBankMSB(unsigned int channelNumber, const babelwires::Byte msbValue) {
    m_channelSetup[channelNumber].m_bankMSB = msbValue;
    onChangeProgram(channelNumber);
}

void smf::SmfParser::setBankLSB(unsigned int channelNumber, const babelwires::Byte lsbValue) {
    m_channelSetup[channelNumber].m_bankLSB = lsbValue;
    onChangeProgram(channelNumber);
}

void smf::SmfParser::setProgram(unsigned int channelNumber, const babelwires::Byte value) {
    m_channelSetup[channelNumber].m_program = value;
    onChangeProgram(channelNumber);
}

void smf::SmfParser::setGsPartMode(unsigned int blockNumber, babelwires::Byte value) {
    // See page 197 of the SC-88 Pro manual
    const std::array<unsigned int, 16> blockToPartMapping{10, 1, 2, 3, 4, 5, 6, 7, 8, 9, 11, 12, 13, 14, 15, 16};
    // For now, assume the midi channels for each part are unchanged.
    // I'm indexing midi channels from 0.
    const unsigned int channelNumber = blockToPartMapping[blockNumber] - 1;
    m_channelSetup[channelNumber].m_gsPartMode = value;
    onChangeProgram(channelNumber);
}

/// Right now, just trying to determine which percussionKit is in use if any.
void smf::SmfParser::onChangeProgram(unsigned int channelNumber) {
    ChannelSetup& channelSetup = m_channelSetup[channelNumber];

    if (m_gmSpec == GMSpecType::Value::GM2) {
        if (channelSetup.m_bankMSB == 0x78) {
            // Percussion
            // LSB not used
            // (program == 1) Standard
            // (program == 9) Room
            // (program == 17) Power
            // (program == 25) Electronic
            // (program == 26) Analog
            // (program == 25) Jazz
            // (program == 41) Brush
            // (program == 49) Orchestra
            // (program == 57) SFX
            channelSetup.m_kitIfPercussion = m_knownKits[GM2_STANDARD_PERCUSSION_KIT];
        } else if (channelSetup.m_bankMSB == 0x79) {
            // Melody
            channelSetup.m_kitIfPercussion = nullptr;
        } else {
            // Not specified by GM2
        }
    } else if (m_gmSpec == GMSpecType::Value::XG) {
        if (channelSetup.m_bankMSB == 0x7f) {
            // Percussion
            channelSetup.m_kitIfPercussion = m_knownKits[GM_PERCUSSION_KIT];
        } else if (channelSetup.m_bankMSB == 0x7e) {
            // SFX Percussion
        } else if (channelSetup.m_bankMSB == 0x00) {
            // Voice
            channelSetup.m_kitIfPercussion = nullptr;
        } else if (channelSetup.m_bankMSB == 0x40) {
            // SFX
            channelSetup.m_kitIfPercussion = nullptr;
        } else {
            // Not specified by XG
        }
    } else if (m_gmSpec == GMSpecType::Value::GS) {
        if (channelSetup.m_gsPartMode == 1) {
            // Percussion mode 1
            channelSetup.m_kitIfPercussion = m_knownKits[GM_PERCUSSION_KIT];
        } else if (channelSetup.m_gsPartMode == 2) {
            // Percussion mode 2
            channelSetup.m_kitIfPercussion = m_knownKits[GM_PERCUSSION_KIT];
        } else {
            // Not a percussion voice
            channelSetup.m_kitIfPercussion = nullptr;
        }
    }
}

std::unique_ptr<babelwires::FileFeature> smf::parseSmfSequence(babelwires::DataSource& dataSource,
                                                               const babelwires::ProjectContext& projectContext,
                                                               babelwires::UserLogger& userLogger) {
    SmfParser parser(dataSource, projectContext, userLogger);
    parser.parse();
    return parser.getResult();
}

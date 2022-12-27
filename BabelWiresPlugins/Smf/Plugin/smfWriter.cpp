/**
 * Write a Standard MIDI File based on the contents of a tree of Features.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <BabelWiresPlugins/Smf/Plugin/smfWriter.hpp>

#include <BabelWiresPlugins/Smf/Plugin/Percussion/gm2StandardPercussionSet.hpp>
#include <BabelWiresPlugins/Smf/Plugin/Percussion/gmPercussionSet.hpp>
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
#include <set>
#include <sstream>

namespace {
    // See page 237 of the SC-8850 English manual for the part to block conversion.
    // We will always use the default part mapping, where parts correspond to midi channels.
    const std::array<unsigned int, 16> s_gsChannelToBlockMapping{1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 11, 12, 13, 14, 15};
} // namespace

smf::SmfWriter::SmfWriter(const babelwires::ProjectContext& projectContext, babelwires::UserLogger& userLogger,
                          const target::SmfFormatFeature& sequence, std::ostream& ostream)
    : m_projectContext(projectContext)
    , m_userLogger(userLogger)
    , m_smfFormatFeature(sequence)
    , m_ostream(ostream)
    , m_os(&m_ostream)
    , m_division(256)
    , m_standardPercussionSets(projectContext) {}

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
    writeUint16((m_smfFormatFeature.getSelectedTagIndex() == 0) ? 1 : numTracks);

    {
        int division = 1;
        for (int i = 0; i < numTracks; ++i) {
            const target::ChannelTrackFeature& entry = m_smfFormatFeature.getMidiTrack(i);
            division = babelwires::lcm(division, seqwires::getMinimumDenominator(entry.m_trackFeature->get()));
        }
        m_division = division;
    }

    // 0 in high-bit implies metrical.
    writeUint16(m_division);
}

smf::SmfWriter::WriteTrackEventResult smf::SmfWriter::writeTrackEvent(int channelNumber,
                                                                      seqwires::ModelDuration timeSinceLastEvent,
                                                                      const seqwires::TrackEvent& e) {
    assert(channelNumber >= 0);
    assert(channelNumber <= 15);

    if (const smf::PercussionSet* const kitIfPercussion = m_channelSetup[channelNumber].m_kitIfPercussion) {
        if (const seqwires::PercussionOnEvent* percussionOn = e.as<seqwires::PercussionOnEvent>()) {
            if (auto maybePitch = kitIfPercussion->tryGetPitchFromInstrument(percussionOn->getInstrument())) {
                writeModelDuration(timeSinceLastEvent);
                m_os->put(0b10010000 | channelNumber);
                m_os->put(*maybePitch);
                m_os->put(percussionOn->getVelocity());
                return WriteTrackEventResult::Written;
            } else {
                return WriteTrackEventResult::NotInPercussionSet;
            }
        } else if (const seqwires::PercussionOffEvent* percussionOff = e.as<seqwires::PercussionOffEvent>()) {
            if (auto maybePitch = kitIfPercussion->tryGetPitchFromInstrument(percussionOn->getInstrument())) {
                writeModelDuration(timeSinceLastEvent);
                m_os->put(0b10000000 | channelNumber);
                m_os->put(*maybePitch);
                m_os->put(percussionOff->getVelocity());
                return WriteTrackEventResult::Written;
            } else {
                return WriteTrackEventResult::NotInPercussionSet;
            }
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

void smf::SmfWriter::writeNotes(const std::vector<const target::ChannelTrackFeature*>& tracks) {
    const int numTracks = tracks.size();

    seqwires::ModelDuration trackDuration = 0;

    // TODO Use the mergeFunction to multiplex the channels.

    std::vector<seqwires::TrackTraverser<seqwires::FilteredTrackIterator<seqwires::TrackEvent>>> traversers;

    for (int i = 0; i < numTracks; ++i) {
        const smf::target::ChannelTrackFeature& channelTrack = *tracks[i];
        const seqwires::Track& track = channelTrack.m_trackFeature->get();
        const int channelNumber = channelTrack.m_channelNum->get();
        traversers.emplace_back(track, seqwires::iterateOver<seqwires::TrackEvent>(track));
        traversers.back().leastUpperBoundDuration(trackDuration);
    }

    seqwires::ModelDuration timeSinceStart = 0;
    seqwires::ModelDuration timeOfLastEvent = 0;
    while (timeSinceStart < trackDuration) {
        seqwires::ModelDuration timeToNextEvent = trackDuration - timeSinceStart;
        for (int i = 0; i < numTracks; ++i) {
            traversers[i].greatestLowerBoundNextEvent(timeToNextEvent);
        }

        bool isFirstEventAtThisTime = true;
        for (int i = 0; i < numTracks; ++i) {
                    const smf::target::ChannelTrackFeature& channelTrack = *tracks[i];
            traversers[i].advance(timeToNextEvent, [this, &isFirstEventAtThisTime, &timeToNextEvent, &timeOfLastEvent,
                                                    &timeSinceStart, &channelTrack](const seqwires::TrackEvent& event) {
                const seqwires::ModelDuration timeToThisEvent = isFirstEventAtThisTime ? timeToNextEvent : 0;
                const WriteTrackEventResult result =
                    writeTrackEvent(channelTrack.m_channelNum->get(), timeToThisEvent, event);
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

template <std::size_t N> void smf::SmfWriter::writeMessage(const std::array<std::uint8_t, N>& message) {
    for (int i = 0; i < message.size(); ++i) {
        m_os->put(message[i]);
    }
}

void smf::SmfWriter::writeGlobalSetup() {
    const MidiMetadata& metadata = m_smfFormatFeature.getMidiMetadata();

    switch (metadata.getSpecFeature()->getAsValue()) {
        case GMSpecType::Value::GM:
            writeModelDuration(0);
            writeMessage(std::array<std::uint8_t, 7>{0b11110000, 0x05, 0x7E, 0x7F, 0x09, 0x01, 0xF7});
            break;
        case GMSpecType::Value::GM2:
            writeModelDuration(0);
            writeMessage(std::array<std::uint8_t, 7>{0b11110000, 0x05, 0x7E, 0x7F, 0x09, 0x03, 0xF7});
            break;
        case GMSpecType::Value::GS:
            writeModelDuration(0);
            writeMessage(
                std::array<std::uint8_t, 12>{0b11110000, 0x0A, 0x41, 0x10, 0x42, 0x12, 0x40, 0x00, 0x7F, 0x00, 0x41, 0xF7});
            break;
        case GMSpecType::Value::XG:
            writeModelDuration(0);
            writeMessage(std::array<std::uint8_t, 10>{0b11110000, 0x09, 0x43, 0x10, 0x4C, 0x00, 0x00, 0x7E, 0x00, 0xF7});
        default:
        case GMSpecType::Value::NONE:
            break;
    }

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
}

void smf::SmfWriter::writeTrack(const std::vector<const target::ChannelTrackFeature*>* tracks, bool includeGlobalSetup) {
    std::ostream* oldStream = m_os;
    std::ostringstream tempStream;
    m_os = &tempStream;

    if (includeGlobalSetup) {
        writeGlobalSetup();
    }

    if (tracks) {
        const GMSpecType::Value gmSpec = m_smfFormatFeature.getMidiMetadata().getSpecFeature()->getAsValue();

        for (int i = 0; i < tracks->size(); ++i) {
            const target::ChannelTrackFeature& channelTrack = *(*tracks)[i];
            const int channelNumber = channelTrack.getChannelNumber();
            ChannelSetup& channelSetup = m_channelSetup[channelNumber];
            if (!channelSetup.m_setupWritten) {
                const std::optional<StandardPercussionSets::ChannelSetupInfo> info =
                    m_standardPercussionSets.getChannelSetupInfoFromPercussionSet(channelSetup.m_kitIfPercussion);
                if (info) {
                    if (gmSpec == GMSpecType::Value::GS) {
                        // Set GS "Use For Rhythm Part"
                        writeModelDuration(0);
                        const std::uint8_t block = 0x10 | s_gsChannelToBlockMapping[channelNumber];
                        const std::uint8_t checksum = (0x80 - ((0x40 + block + 0x15 + info->m_gsPartMode) % 0x80)) % 0x80;
                        writeMessage(std::array<std::uint8_t, 12>{0b11110000, 0x0A, 0x41, 0x10, 0x42, 0x12, 0x40, block, 0x15,
                                                                  info->m_gsPartMode, checksum, 0xF7});
                    }

                    // Bank select MSB
                    writeModelDuration(0);
                    writeMessage(std::array<std::uint8_t, 3>{static_cast<std::uint8_t>(0b10110000 | channelNumber),
                                                             0x00, info->m_bankMSB});

                    // Bank select LSB
                    writeModelDuration(0);
                    writeMessage(std::array<std::uint8_t, 3>{static_cast<std::uint8_t>(0b10110000 | channelNumber),
                                                             0x20, info->m_bankLSB});

                    // Program change.
                    writeModelDuration(0);
                    writeMessage(std::array<std::uint8_t, 2>{static_cast<std::uint8_t>(0b11000000 | channelNumber),
                                                             info->m_program});
                }
                channelSetup.m_setupWritten = true;
            }
        }

        writeNotes(*tracks);
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

void smf::SmfWriter::setUpPercussionKit(const std::unordered_set<babelwires::Identifier>& instrumentsInUse,
                                        int channelNumber) {
    const GMSpecType::Value gmSpec = m_smfFormatFeature.getMidiMetadata().getSpecFeature()->getAsValue();
    std::unordered_set<babelwires::Identifier> excludedInstruments;
    m_channelSetup[channelNumber].m_kitIfPercussion =
        m_standardPercussionSets.getBestPercussionSet(gmSpec, channelNumber, instrumentsInUse, excludedInstruments);
    if (!excludedInstruments.empty()) {
        m_userLogger.logWarning() << "Percussion events for " << excludedInstruments.size()
                                  << " instruments could not be represented in channel " << channelNumber;
    }
}

namespace {
    void getPercussionInstrumentsInUse(const seqwires::Track& track,
                                       std::unordered_set<babelwires::Identifier>& instrumentsInUse) {
        for (auto it : seqwires::iterateOver<seqwires::PercussionEvent>(track)) {
            instrumentsInUse.insert(it.getInstrument());
        }
    }

} // namespace

void smf::SmfWriter::setUpPercussionSets() {
    std::array<std::unordered_set<babelwires::Identifier>, 16> instrumentsInUse;
    const int numTracks = m_smfFormatFeature.getNumMidiTracks();
    for (int i = 0; i < numTracks; ++i) {
        const smf::target::ChannelTrackFeature& channelAndTrack = m_smfFormatFeature.getMidiTrack(i);
        getPercussionInstrumentsInUse(channelAndTrack.getTrack(),
                                        instrumentsInUse[channelAndTrack.getChannelNumber()]);
    }
    for (int i = 0; i < 16; ++i) {
        setUpPercussionKit(instrumentsInUse[i], i);
    }
}

void smf::SmfWriter::write() {
    setUpPercussionSets();

    writeHeaderChunk();

    std::vector<const target::ChannelTrackFeature*> tracks;
    const int numTracks = m_smfFormatFeature.getNumMidiTracks();

    if (m_smfFormatFeature.getSelectedTagIndex() == 0) {
        for (int i = 0; i < numTracks; ++i) {
            tracks.emplace_back(&m_smfFormatFeature.getMidiTrack(i));
        }
        writeTrack(&tracks, true);
    } else {
        for (int i = 0; i < numTracks; ++i) {
            tracks.clear();
            tracks.emplace_back(&m_smfFormatFeature.getMidiTrack(i));
            writeTrack(&tracks, (i == 0));
        }
    }
}

void smf::writeToSmf(const babelwires::ProjectContext& projectContext, babelwires::UserLogger& userLogger,
                     const target::SmfFormatFeature& smfFormatFeature, std::ostream& output) {
    smf::SmfWriter writer(projectContext, userLogger, smfFormatFeature, output);
    writer.write();
}

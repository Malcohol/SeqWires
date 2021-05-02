/**
 * Representation of a Standard MIDI File as a tree of Features.
 * 
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include "BabelWiresPlugins/Smf/smfImportModel.hpp"
#include "BabelWires/Features/featureMixins.hpp"
#include "BabelWiresPlugins/Smf/smfFormat.hpp"
#include "SeqWiresLib/Features/trackFeature.hpp"

#include "BabelWires/Features/Path/fieldName.hpp"


namespace {
    static const babelwires::FieldIdentifiersSource s_trackNames = {
        {"ch0", "Ch. 0", "18cb72e5-6891-428c-beae-aa17db453852"},
        {"ch1", "Ch. 1", "f4bc5369-5d7e-4e18-83ec-44560309edb7"},
        {"ch2", "Ch. 2", "8e58bcc7-af0b-40d0-8a13-ca2c35db8c5e"},
        {"ch3", "Ch. 3", "cddd1035-ca20-4094-ae29-7947d9e71b4f"},
        {"ch4", "Ch. 4", "f4d6c484-028f-4d17-bcb0-a2ddf21904d9"},
        {"ch5", "Ch. 5", "9ba09b7c-2301-4846-9ac0-ccd3708abb7b"},
        {"ch6", "Ch. 6", "2382cc8d-7920-40e8-8813-35b75f40befb"},
        {"ch7", "Ch. 7", "18d1584d-67e4-4f50-ad5a-9d887842414f"},
        {"ch8", "Ch. 8", "6925461b-02b3-448c-a7a9-4cd7f1d5c444"},
        {"ch9", "Ch. 9", "95cfcb8f-af02-4d24-95b6-5b4c19cb3c1b"},
        {"ch10", "Ch. 10", "a445a439-2335-445e-b1f9-ae0a6c23cbd7"},
        {"ch11", "Ch. 11", "b1b96d5b-4c13-4c5d-b876-a7752cdf4694"},
        {"ch12", "Ch. 12", "0df2d856-80d7-47ee-88ba-508bdffec70c"},
        {"ch13", "Ch. 13", "3b9e4821-d6eb-45da-9118-419e190dc763"},
        {"ch14", "Ch. 14", "2742e897-3182-48bf-9558-21e6184cddec"},
        {"ch15", "Ch. 15", "46fb6f3a-15bf-4c3a-972a-78e7ba2ca5b9"}};
}

seqwires::TrackFeature* smf::import::RecordChannelGroup::addTrack(int c) {
    assert((0 <= c) && "Negative channel number");
    assert((c <= 15) && "Channel number out of range");
    assert((tryGetChildFromStep(babelwires::PathStep(std::get<0>(s_trackNames[c]))) == nullptr) && "A track with that channel number is already present");
    // TODO Assert that the fields are in channel order.
    return addField(std::make_unique<seqwires::TrackFeature>(), FIELD_NAME_VECTOR(s_trackNames)[c]);
}

const seqwires::TrackFeature* smf::import::RecordChannelGroup::getTrack(int c) const {
    return static_cast<const seqwires::TrackFeature*>(tryGetChildFromStep(babelwires::PathStep(std::get<0>(s_trackNames[c]))));
}

smf::import::ChannelTrackFeature::ChannelTrackFeature() {
    m_channelNum = addField(std::make_unique<babelwires::HasStaticRange<babelwires::IntFeature, 0, 15>>(),
                            FIELD_NAME("Chan", "channel", "011e3ef1-4c06-4e40-bba4-b242dc8a3d3a"));
    m_noteTrackFeature = addField(std::make_unique<seqwires::TrackFeature>(),
                                  FIELD_NAME("Notes", "Notes", "b48b1dff-6fa4-4c2f-8f77-bc50f44fb09a"));
}

std::unique_ptr<babelwires::Feature> smf::import::ArrayChannelGroup::createNextEntry() const {
    return std::make_unique<ChannelTrackFeature>();
}

seqwires::TrackFeature* smf::import::ArrayChannelGroup::addTrack(int c) {
    assert((getTrack(c) == nullptr) && "Channel c already in use");
    ChannelTrackFeature* channelTrack = dynamic_cast<ChannelTrackFeature*>(addEntry());
    channelTrack->m_channelNum->set(c);
    return channelTrack->m_noteTrackFeature;
}

const seqwires::TrackFeature* smf::import::ArrayChannelGroup::getTrack(int c) const {
    for (int i = 0; i < getNumFeatures(); ++i) {
        const ChannelTrackFeature* entry = dynamic_cast<const ChannelTrackFeature*>(getFeature(i));
        assert(entry && "There should be an ith element");
        if (entry->m_channelNum->get() == c) {
            return entry->m_noteTrackFeature;
        }
    }
    return nullptr;
}

smf::import::SmfSequence::SmfSequence(Format f)
    : babelwires::FileFeature(SmfFormat::getThisIdentifier())
    , m_format(f) {
    assert((f != UNKNOWN_SEQUENCE_TYPE) && "You can only construct a format 0, 1 or 2 MIDI file");
    m_sequenceName = addField(std::make_unique<babelwires::StringFeature>(),
                              FIELD_NAME("Name", "Name", "c2e4910f-d006-4a93-97a7-ae5973157ec8"));
    m_copyright = addField(std::make_unique<babelwires::StringFeature>(),
                           FIELD_NAME("CopyR", "Copyright", "a59dc914-d060-4f03-be83-5804fc4d6b6a"));
    m_tempo = addField(std::make_unique<seqwires::TempoFeature>(),
                       FIELD_NAME("Tempo", "Tempo", "3ef804e9-e34a-4a25-b6bf-ce7597d9d90b"));
}

smf::import::SmfSequence::Format smf::import::SmfSequence::getFormat() const {
    return m_format;
}

seqwires::TempoFeature* smf::import::SmfSequence::getTempoFeature() {
    return m_tempo;
}

const seqwires::TempoFeature* smf::import::SmfSequence::getTempoFeature() const {
    return m_tempo;
}

babelwires::StringFeature* smf::import::SmfSequence::getCopyright() {
    return m_copyright;
}

const babelwires::StringFeature* smf::import::SmfSequence::getCopyright() const {
    return m_copyright;
}

babelwires::StringFeature* smf::import::SmfSequence::getSequenceName() {
    return m_sequenceName;
}

const babelwires::StringFeature* smf::import::SmfSequence::getSequenceName() const {
    return m_sequenceName;
}

smf::import::Format0Sequence::Format0Sequence()
    : SmfSequence(FORMAT_0_SEQUENCE) {
    m_channelGroup = addField(std::make_unique<RecordChannelGroup>(),
                              FIELD_NAME("tracks", "tracks", "3fb0f062-4e8e-4b37-a598-edcd63f82971"));
}

int smf::import::Format0Sequence::getNumMidiTracks() const {
    return 1;
}

const smf::import::ChannelGroup& smf::import::Format0Sequence::getMidiTrack(int i) const {
    assert((i == 0) && "There is only 1 in format 0 smf files.");
    return *m_channelGroup;
}

smf::import::ChannelGroup* smf::import::Format0Sequence::getMidiTrack0() {
    return m_channelGroup;
}

smf::import::Format1Sequence::Format1Sequence()
    : SmfSequence(FORMAT_1_SEQUENCE) {
    m_tracks = addField(std::make_unique<TrackArray>(),
                        FIELD_NAME("Tracks", "tracks", "3042e0e6-62a6-4a75-b886-77b873005da8"));
}

int smf::import::Format1Sequence::getNumMidiTracks() const {
    return m_tracks->getNumFeatures();
}

const smf::import::ChannelGroup& smf::import::Format1Sequence::getMidiTrack(int i) const {
    return dynamic_cast<const ChannelGroup&>(*m_tracks->getFeature(i));
}

smf::import::ChannelGroup* smf::import::Format1Sequence::addMidiTrack() {
    return dynamic_cast<ChannelGroup*>(m_tracks->addEntry());
}

std::unique_ptr<babelwires::Feature> smf::import::TrackArray::createNextEntry() const {
    return std::make_unique<ArrayChannelGroup>();
}

/**
 * Representation of a Standard MIDI File as a tree of Features.
 * 
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include "BabelWiresPlugins/Smf/smfTargetModel.hpp"

#include "BabelWires/Features/featureMixins.hpp"
#include "BabelWiresPlugins/Smf/smfFormat.hpp"
#include "SeqWiresLib/Features/trackFeature.hpp"

#include "BabelWires/Features/Path/fieldName.hpp"

smf::target::ChannelTrackFeature::ChannelTrackFeature() {
    m_channelNum = addField(std::make_unique<babelwires::HasStaticRange<babelwires::IntFeature, 0, 15>>(),
                            FIELD_NAME("Chan", "channel", "011e3ef1-4c06-4e40-bba4-b242dc8a3d3a"));
    m_noteTrackFeature = addField(std::make_unique<seqwires::TrackFeature>(),
                                  FIELD_NAME("Notes", "Notes", "b48b1dff-6fa4-4c2f-8f77-bc50f44fb09a"));
}

int smf::target::ChannelTrackFeature::getNumTracks() const {
    return 1;
}

const smf::target::ChannelTrackFeature& smf::target::ChannelTrackFeature::getTrack(int i) const {
    return *this;
}

int smf::target::ArrayChannelGroup::getNumTracks() const {
    return getNumFeatures();
}

const smf::target::ChannelTrackFeature& smf::target::ArrayChannelGroup::getTrack(int i) const {
    return static_cast<const ChannelTrackFeature&>(*getFeature(i));
}

std::unique_ptr<babelwires::Feature> smf::target::ArrayChannelGroup::createNextEntry() const {
    return std::make_unique<ChannelTrackFeature>();
}

smf::target::SmfSequence::SmfSequence(Format f)
    : babelwires::FileFeature(SmfSourceFormat::getThisIdentifier())
    , m_format(f) {
    assert((f != UNKNOWN_SEQUENCE_TYPE) && "You can only construct a format 0, 1 or 2 MIDI file");
    m_sequenceName = addField(std::make_unique<babelwires::StringFeature>(),
                              FIELD_NAME("Name", "Name", "c2e4910f-d006-4a93-97a7-ae5973157ec8"));
    m_copyright = addField(std::make_unique<babelwires::StringFeature>(),
                           FIELD_NAME("CopyR", "Copyright", "a59dc914-d060-4f03-be83-5804fc4d6b6a"));
    m_tempo = addField(std::make_unique<seqwires::TempoFeature>(),
                       FIELD_NAME("Tempo", "Tempo", "3ef804e9-e34a-4a25-b6bf-ce7597d9d90b"));
}

smf::target::SmfSequence::Format smf::target::SmfSequence::getFormat() const {
    return m_format;
}

seqwires::TempoFeature* smf::target::SmfSequence::getTempoFeature() {
    return m_tempo;
}

const seqwires::TempoFeature* smf::target::SmfSequence::getTempoFeature() const {
    return m_tempo;
}

babelwires::StringFeature* smf::target::SmfSequence::getCopyright() {
    return m_copyright;
}

const babelwires::StringFeature* smf::target::SmfSequence::getCopyright() const {
    return m_copyright;
}

babelwires::StringFeature* smf::target::SmfSequence::getSequenceName() {
    return m_sequenceName;
}

const babelwires::StringFeature* smf::target::SmfSequence::getSequenceName() const {
    return m_sequenceName;
}

smf::target::Format0Sequence::Format0Sequence()
    : SmfSequence(FORMAT_0_SEQUENCE) {
    m_channelGroup = addField(std::make_unique<ArrayChannelGroup>(),
                              FIELD_NAME("tracks", "tracks", "3fb0f062-4e8e-4b37-a598-edcd63f82971"));
}

int smf::target::Format0Sequence::getNumMidiTracks() const {
    return 1;
}

const smf::target::ChannelGroup& smf::target::Format0Sequence::getMidiTrack(int i) const {
    assert((i == 0) && "There is only 1 in format 0 smf files.");
    return *m_channelGroup;
}

smf::target::Format1Sequence::Format1Sequence()
    : SmfSequence(FORMAT_1_SEQUENCE) {
    m_channelGroup = addField(std::make_unique<ArrayChannelGroup>(),
                        FIELD_NAME("tracks", "tracks", "3042e0e6-62a6-4a75-b886-77b873005da8"));
}

int smf::target::Format1Sequence::getNumMidiTracks() const {
    return m_channelGroup->getNumFeatures();
}

const smf::target::ChannelGroup& smf::target::Format1Sequence::getMidiTrack(int i) const {
    return static_cast<const ChannelTrackFeature&>(*m_channelGroup->getFeature(i));
}


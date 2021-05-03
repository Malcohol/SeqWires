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
                            FIELD_NAME("Chan", "channel", "cad592ef-2355-4837-a265-38b49eae7599"));
    m_noteTrackFeature = addField(std::make_unique<seqwires::TrackFeature>(),
                                  FIELD_NAME("Track", "track", "a6db15c9-9f29-4fb3-92c4-771746b2b97f"));
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

babelwires::Range<unsigned int> smf::target::ArrayChannelGroup::doGetSizeRange() const {
    auto defaultRange = babelwires::ArrayFeature::doGetSizeRange();
    defaultRange.m_min = 1;
    return defaultRange;
}

smf::target::SmfFeature::SmfFeature(Format f)
    : babelwires::FileFeature(SmfSourceFormat::getThisIdentifier())
    , m_format(f) {
    assert((f != UNKNOWN_SEQUENCE_TYPE) && "You can only construct a format 0, 1 or 2 MIDI file");
    m_sequenceName = addField(std::make_unique<babelwires::StringFeature>(),
                              FIELD_NAME("Name", "Name", "160acc82-b374-433f-9a5d-b971e5871396"));
    m_copyright = addField(std::make_unique<babelwires::StringFeature>(),
                           FIELD_NAME("CopyR", "Copyright", "008830f3-087d-4af5-83c1-3530520b2d29"));
    m_tempo = addField(std::make_unique<seqwires::TempoFeature>(),
                       FIELD_NAME("Tempo", "Tempo", "f5721a7d-f1b7-468e-875c-36036982a571"));
}

smf::target::SmfFeature::Format smf::target::SmfFeature::getFormat() const {
    return m_format;
}

seqwires::TempoFeature* smf::target::SmfFeature::getTempoFeature() {
    return m_tempo;
}

const seqwires::TempoFeature* smf::target::SmfFeature::getTempoFeature() const {
    return m_tempo;
}

babelwires::StringFeature* smf::target::SmfFeature::getCopyright() {
    return m_copyright;
}

const babelwires::StringFeature* smf::target::SmfFeature::getCopyright() const {
    return m_copyright;
}

babelwires::StringFeature* smf::target::SmfFeature::getSequenceName() {
    return m_sequenceName;
}

const babelwires::StringFeature* smf::target::SmfFeature::getSequenceName() const {
    return m_sequenceName;
}

smf::target::Format0SmfFeature::Format0SmfFeature()
    : SmfFeature(FORMAT_0_SEQUENCE) {
    m_channelGroup = addField(std::make_unique<ArrayChannelGroup>(),
                              FIELD_NAME("tracks", "tracks", "1aa014da-2fb3-46e2-96df-98c7798b4a87"));
}

int smf::target::Format0SmfFeature::getNumMidiTracks() const {
    return 1;
}

const smf::target::ChannelGroup& smf::target::Format0SmfFeature::getMidiTrack(int i) const {
    assert((i == 0) && "There is only 1 in format 0 smf files.");
    return *m_channelGroup;
}

smf::target::Format1SmfFeature::Format1SmfFeature()
    : SmfFeature(FORMAT_1_SEQUENCE) {
    m_channelGroup = addField(std::make_unique<ArrayChannelGroup>(),
                        FIELD_NAME("tracks", "tracks", "9b3642d2-b50e-487e-96c9-bc39e418c687"));
}

int smf::target::Format1SmfFeature::getNumMidiTracks() const {
    return m_channelGroup->getNumFeatures();
}

const smf::target::ChannelGroup& smf::target::Format1SmfFeature::getMidiTrack(int i) const {
    return static_cast<const ChannelTrackFeature&>(*m_channelGroup->getFeature(i));
}


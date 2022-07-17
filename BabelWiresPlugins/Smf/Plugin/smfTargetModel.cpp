/**
 * Representation of a Standard MIDI File as a tree of Features.
 * 
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <BabelWiresPlugins/Smf/Plugin/smfTargetModel.hpp>

#include <BabelWiresPlugins/Smf/Plugin/smfFormat.hpp>

#include <BabelWiresLib/Features/featureMixins.hpp>

#include <SeqWiresLib/Features/trackFeature.hpp>

#include <Common/Identifiers/registeredIdentifier.hpp>

smf::target::ChannelTrackFeature::ChannelTrackFeature() {
    m_channelNum = addField(std::make_unique<babelwires::HasStaticRange<babelwires::IntFeature, 0, 15>>(),
                            REGISTERED_ID("Chan", "channel", "cad592ef-2355-4837-a265-38b49eae7599"));
    m_trackFeature = addField(std::make_unique<seqwires::TrackFeature>(),
                                  REGISTERED_ID("Track", "track", "a6db15c9-9f29-4fb3-92c4-771746b2b97f"));
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

smf::target::SmfFeature::SmfFeature(const babelwires::ProjectContext& projectContext, Format f)
    : babelwires::FileFeature(projectContext, SmfSourceFormat::getThisIdentifier())
    , m_format(f) {
    assert((f != SMF_UNKNOWN_FORMAT) && "You can only construct a format 0, 1 or 2 MIDI file");
    m_metadata = addField(std::make_unique<MidiMetadata>(),
        REGISTERED_ID("Meta", "Metadata", "72bbbcee-2b53-4fb2-bfb8-4f5e495f9166"));
}

smf::target::SmfFeature::Format smf::target::SmfFeature::getFormat() const {
    return m_format;
}

const smf::MidiMetadata& smf::target::SmfFeature::getMidiMetadata() const {
    return *m_metadata;
}

smf::target::Format0SmfFeature::Format0SmfFeature(const babelwires::ProjectContext& projectContext)
    : SmfFeature(projectContext, SMF_FORMAT_0) {
    m_channelGroup = addField(std::make_unique<ArrayChannelGroup>(),
                              REGISTERED_ID("tracks", "tracks", "1aa014da-2fb3-46e2-96df-98c7798b4a87"));
}

int smf::target::Format0SmfFeature::getNumMidiTracks() const {
    return 1;
}

const smf::target::ChannelGroup& smf::target::Format0SmfFeature::getMidiTrack(int i) const {
    assert((i == 0) && "There is only 1 in format 0 smf files.");
    return *m_channelGroup;
}

smf::target::Format1SmfFeature::Format1SmfFeature(const babelwires::ProjectContext& projectContext)
    : SmfFeature(projectContext, SMF_FORMAT_1) {
    m_channelGroup = addField(std::make_unique<ArrayChannelGroup>(),
                        REGISTERED_ID("tracks", "tracks", "9b3642d2-b50e-487e-96c9-bc39e418c687"));
}

int smf::target::Format1SmfFeature::getNumMidiTracks() const {
    return m_channelGroup->getNumFeatures();
}

const smf::target::ChannelGroup& smf::target::Format1SmfFeature::getMidiTrack(int i) const {
    return static_cast<const ChannelTrackFeature&>(*m_channelGroup->getFeature(i));
}


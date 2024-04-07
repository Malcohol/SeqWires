/**
 * Representation of a Standard MIDI File as a tree of Features.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <Plugins/Smf/Plugin/smfTargetModel.hpp>

#include <Plugins/Smf/Plugin/smfFormat.hpp>

#include <SeqWiresLib/Types/Track/trackFeature.hpp>

#include <Common/Identifiers/registeredIdentifier.hpp>

smf::target::ChannelTrackFeature::ChannelTrackFeature() {
    m_channelNum = addField(std::make_unique<babelwires::IntFeature>(0, 15),
                            BW_SHORT_ID("Chan", "channel", "cad592ef-2355-4837-a265-38b49eae7599"));
    m_trackFeature = addField(std::make_unique<seqwires::TrackFeature>(),
                              BW_SHORT_ID("Track", "track", "a6db15c9-9f29-4fb3-92c4-771746b2b97f"));
}

int smf::target::ChannelTrackFeature::getChannelNumber() const {
    return m_channelNum->get();
}

const seqwires::Track& smf::target::ChannelTrackFeature::getTrack() const {
    return m_trackFeature->get();
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

namespace {
    const babelwires::IdentifiersSource formatIdentifiersSource = {
        {"SMF0", "0", "82835685-a66e-417e-b076-5d706d03a989"}, {"SMF1", "1", "ce16e042-445b-423d-a493-874ed4c238b3"}};
}

smf::target::SmfFormatFeature::SmfFormatFeature()
    : UnionFeature(REGISTERED_ID_VECTOR(formatIdentifiersSource), 0) {
    m_metadata = addField(std::make_unique<MidiMetadataFeature>(),
                          BW_SHORT_ID("Meta", "Metadata", "72bbbcee-2b53-4fb2-bfb8-4f5e495f9166"));
    m_channelGroup = addField(std::make_unique<ArrayChannelGroup>(),
                              BW_SHORT_ID("tracks", "tracks", "1aa014da-2fb3-46e2-96df-98c7798b4a87"));
}

const smf::MidiMetadataFeature& smf::target::SmfFormatFeature::getMidiMetadata() const {
    return *m_metadata;
}

smf::MidiMetadataFeature& smf::target::SmfFormatFeature::getMidiMetadata() {
    return *m_metadata;
}

smf::target::SmfFeature::SmfFeature(const babelwires::ProjectContext& projectContext)
    : babelwires::FileFeature(projectContext, SmfSourceFormat::getThisIdentifier()) {
    m_formatFeature = addField(std::make_unique<SmfFormatFeature>(),
                               BW_SHORT_ID("Format", "Format", "1f2eaefb-b48d-484d-8793-e14f2fa0193b"));
}

int smf::target::SmfFormatFeature::getNumMidiTracks() const {
    return m_channelGroup->getNumTracks();
}

const smf::target::ChannelTrackFeature& smf::target::SmfFormatFeature::getMidiTrack(int i) const {
    return m_channelGroup->getTrack(i);
}

const smf::target::SmfFormatFeature& smf::target::SmfFeature::getFormatFeature() const {
    return *m_formatFeature;
}

babelwires::Feature::Style smf::target::SmfFormatFeature::getStyle() const {
    // Not collapsable and inlined.
    return babelwires::Feature::Style::isInlined;
}

/**
 * Representation of a Standard MIDI File as a tree of Features.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <Plugins/Smf/Plugin/smfTargetModel.hpp>

#include <Plugins/Smf/Plugin/midiTrackAndChannel.hpp>
#include <Plugins/Smf/Plugin/smfFormat.hpp>

#include <SeqWiresLib/Types/Track/trackFeature.hpp>

#include <BabelWiresLib/Features/simpleValueFeature.hpp>

#include <Common/Identifiers/registeredIdentifier.hpp>
namespace {
    const babelwires::IdentifiersSource formatIdentifiersSource = {
        {"SMF0", "0", "82835685-a66e-417e-b076-5d706d03a989"}, {"SMF1", "1", "ce16e042-445b-423d-a493-874ed4c238b3"}};
}

smf::target::SmfFormatFeature::SmfFormatFeature()
    : UnionFeature(REGISTERED_ID_VECTOR(formatIdentifiersSource), 0) {
    m_metadata = addField(std::make_unique<MidiMetadataFeature>(),
                          BW_SHORT_ID("Meta", "Metadata", "72bbbcee-2b53-4fb2-bfb8-4f5e495f9166"));
    m_midiTrackAndChannelArray = addField(std::make_unique<babelwires::SimpleValueFeature>(MidiTrackAndChannelArray::getThisIdentifier()),
                                    BW_SHORT_ID("tracks", "tracks", "38ae4e20-1468-4dce-890b-981454e6dbe0"));
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
    return MidiTrackAndChannelArray::getArraySize(*m_midiTrackAndChannelArray);
}

const babelwires::ValueFeature& smf::target::SmfFormatFeature::getMidiTrack(int index) const {
    return MidiTrackAndChannelArray::getChild(*m_midiTrackAndChannelArray, index);
}

const smf::target::SmfFormatFeature& smf::target::SmfFeature::getFormatFeature() const {
    return *m_formatFeature;
}

babelwires::Feature::Style smf::target::SmfFormatFeature::getStyle() const {
    // Not collapsable and inlined.
    return babelwires::Feature::Style::isInlined;
}


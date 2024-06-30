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

smf::SmfType::SmfType()
    : babelwires::RecordWithVariantsType(REGISTERED_ID_VECTOR(formatIdentifiersSource),
                                         {{BW_SHORT_ID("Meta", "Metadata", "72bbbcee-2b53-4fb2-bfb8-4f5e495f9166"),
                                           MidiMetadata::getThisIdentifier(),
                                           {}},
                                          {BW_SHORT_ID("Tracks", "Tracks", "38ae4e20-1468-4dce-890b-981454e6dbe0"),
                                           MidiTrackAndChannelArray::getThisIdentifier(),
                                           {}}}) {}

smf::target::SmfFeature::SmfFeature(const babelwires::ProjectContext& projectContext)
    : babelwires::FileFeature(projectContext, SmfSourceFormat::getThisIdentifier()) {
    m_smfTypeFeature = addField(std::make_unique<babelwires::SimpleValueFeature>(SmfType::getThisIdentifier()),
                              BW_SHORT_ID("Smf", "Format", "3c918862-5835-4cbf-ade6-af6f7ab7821d"));
}

smf::SmfType::Instance<const babelwires::ValueFeature> smf::target::SmfFeature::getSmfTypeFeature() const {
    return m_smfTypeFeature;
}

smf::SmfType::Instance<babelwires::ValueFeature> smf::target::SmfFeature::getSmfTypeFeature() {
    return m_smfTypeFeature;
}

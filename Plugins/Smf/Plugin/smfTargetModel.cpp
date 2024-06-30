/**
 * Representation of a Standard MIDI File as a tree of Features.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <Plugins/Smf/Plugin/smfTargetModel.hpp>

#include <Plugins/Smf/Plugin/smfFormat.hpp>

#include <BabelWiresLib/Features/simpleValueFeature.hpp>

#include <Common/Identifiers/registeredIdentifier.hpp>

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

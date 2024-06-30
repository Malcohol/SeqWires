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
    m_smfTypeFeature = addField(std::make_unique<babelwires::SimpleValueFeature>(SmfSequenceType::getThisIdentifier()),
                              BW_SHORT_ID("SmfSeq", "Format", "1f2eaefb-b48d-484d-8793-e14f2fa0193b"));
}

smf::SmfSequenceType::Instance<const babelwires::ValueFeature> smf::target::SmfFeature::getSmfTypeFeature() const {
    return m_smfTypeFeature;
}

smf::SmfSequenceType::Instance<babelwires::ValueFeature> smf::target::SmfFeature::getSmfTypeFeature() {
    return m_smfTypeFeature;
}

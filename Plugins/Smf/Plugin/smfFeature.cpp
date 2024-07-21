/**
 * Representation of a Standard MIDI File as a tree of Features.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <Plugins/Smf/Plugin/smfFeature.hpp>

#include <Plugins/Smf/Plugin/smfFormat.hpp>

#include <BabelWiresLib/Features/simpleValueFeature.hpp>

#include <Common/Identifiers/registeredIdentifier.hpp>

smf::SmfFeature::SmfFeature(const babelwires::ProjectContext& projectContext)
    : babelwires::FileFeature(projectContext, SmfSourceFormat::getThisIdentifier()) {
    m_smfTypeFeature = addField(std::make_unique<babelwires::SimpleValueFeature>(SmfSequence::getThisIdentifier()),
                              BW_SHORT_ID("SmfSeq", "Format", "1f2eaefb-b48d-484d-8793-e14f2fa0193b"));
}

smf::SmfSequence::Instance<const babelwires::ValueFeature> smf::SmfFeature::getSmfSequence() const {
    return *m_smfTypeFeature;
}

smf::SmfSequence::Instance<babelwires::ValueFeature> smf::SmfFeature::getSmfSequence() {
    return *m_smfTypeFeature;
}

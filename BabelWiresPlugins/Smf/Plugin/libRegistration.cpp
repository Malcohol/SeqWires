/**
 * Registration of factories for the Standard MIDI File support.
 * 
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <BabelWiresPlugins/Smf/Plugin/libRegistration.hpp>

#include <BabelWiresLib/Project/projectContext.hpp>
#include <BabelWiresLib/TypeSystem/typeSystem.hpp>

#include <BabelWiresPlugins/Smf/Plugin/smfFormat.hpp>
#include <BabelWiresPlugins/Smf/Plugin/gmSpec.hpp>

void smf::registerLib(babelwires::ProjectContext& context) {
    // Formats
    context.m_sourceFileFormatReg.addEntry(std::make_unique<smf::SmfSourceFormat>());
    context.m_targetFileFormatReg.addEntry(std::make_unique<smf::SmfTargetFormat>());

    // Types
    context.m_typeSystem.addEntry(std::make_unique<smf::GMSpecType>());
}

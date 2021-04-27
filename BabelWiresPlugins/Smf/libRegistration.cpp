/**
 * Registration of factories for the Standard MIDI File support.
 * 
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include "BabelWiresPlugins/Smf/libRegistration.hpp"

#include "BabelWires/Project/projectContext.hpp"

#include "BabelWiresPlugins/Smf/smfFormat.hpp"

void smf::registerLib(babelwires::ProjectContext& context) {
    context.m_sourceFileFormatReg.addEntry(std::make_unique<smf::SmfFormat>());
    context.m_targetFileFormatReg.addEntry(std::make_unique<smf::Format0FactoryFormat>());
    context.m_targetFileFormatReg.addEntry(std::make_unique<smf::Format1FactoryFormat>());
}

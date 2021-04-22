#include "BabelWiresPlugins/Smf/libRegistration.hpp"

#include "BabelWires/Project/projectContext.hpp"

#include "BabelWiresPlugins/Smf/smfFormat.hpp"

void smf::registerLib(babelwires::ProjectContext& context) {
    context.m_fileFormatReg.addEntry(std::make_unique<smf::SmfFormat>());
    context.m_factoryFormatReg.addEntry(std::make_unique<smf::Format0FactoryFormat>());
    context.m_factoryFormatReg.addEntry(std::make_unique<smf::Format1FactoryFormat>());
}

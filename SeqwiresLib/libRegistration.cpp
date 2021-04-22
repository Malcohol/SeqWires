#include "SeqwiresLib/libRegistration.hpp"

#include "BabelWires/Processors/processorFactory.hpp"
#include "BabelWires/Processors/processorFactoryRegistry.hpp"
#include "BabelWires/Project/projectContext.hpp"

#include "SeqwiresLib/Processors/excerptProcessor.hpp"

void seqwires::registerLib(babelwires::ProjectContext& context) {
    context.m_processorReg.addEntry(std::make_unique<seqwires::ExcerptProcessorFactory>());
}

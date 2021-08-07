/**
 * Register factories etc. for SeqWires into BabelWires.
 * 
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include "SeqWiresLib/libRegistration.hpp"

#include "BabelWires/Processors/processorFactory.hpp"
#include "BabelWires/Processors/processorFactoryRegistry.hpp"
#include "BabelWires/Project/projectContext.hpp"

#include "SeqWiresLib/Processors/excerptProcessor.hpp"
#include "SeqWiresLib/Processors/repeatProcessor.hpp"
#include "SeqWiresLib/Processors/concatenateProcessor.hpp"
#include "SeqWiresLib/Processors/silenceProcessor.hpp"

void seqwires::registerLib(babelwires::ProjectContext& context) {
    context.m_processorReg.addEntry(std::make_unique<ConcatenateProcessorFactory>());
    context.m_processorReg.addEntry(std::make_unique<ExcerptProcessorFactory>());
    context.m_processorReg.addEntry(std::make_unique<RepeatProcessorFactory>());
    context.m_processorReg.addEntry(std::make_unique<SilenceProcessorFactory>());
}

/**
 * Register factories etc. for SeqWires into BabelWires.
 * 
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <SeqWiresLib/libRegistration.hpp>

#include <BabelWiresLib/Processors/processorFactory.hpp>
#include <BabelWiresLib/Processors/processorFactoryRegistry.hpp>
#include <BabelWiresLib/Project/projectContext.hpp>
#include <BabelWiresLib/TypeSystem/typeSystem.hpp>

#include <SeqWiresLib/Processors/excerptProcessor.hpp>
#include <SeqWiresLib/Processors/concatenateProcessor.hpp>
#include <SeqWiresLib/Processors/mergeProcessor.hpp>
#include <SeqWiresLib/Processors/monophonicSubtracksProcessor.hpp>
#include <SeqWiresLib/Processors/repeatProcessor.hpp>
#include <SeqWiresLib/Processors/silenceProcessor.hpp>
#include <SeqWiresLib/Processors/splitAtPitchProcessor.hpp>
#include <SeqWiresLib/Processors/transposeProcessor.hpp>
#include <SeqWiresLib/Processors/chordsFromNotesProcessor.hpp>
#include <SeqWiresLib/Processors/chordMapProcessor.hpp>

#include <SeqWiresLib/Functions/monophonicSubtracksFunction.hpp>
#include <SeqWiresLib/chord.hpp>
#include <SeqWiresLib/pitchClass.hpp>

void seqwires::registerLib(babelwires::ProjectContext& context) {
    context.m_typeSystem.addEntry(std::make_unique<MonophonicSubtracksPolicyEnum>());
    context.m_typeSystem.addEntry(std::make_unique<ChordType>());
    context.m_typeSystem.addEntry(std::make_unique<PitchClass>());
    
    context.m_processorReg.addEntry(std::make_unique<ChordMapProcessor::Factory>());
    context.m_processorReg.addEntry(std::make_unique<ConcatenateProcessor::Factory>());
    context.m_processorReg.addEntry(std::make_unique<ExcerptProcessor::Factory>());
    context.m_processorReg.addEntry(std::make_unique<MergeProcessor::Factory>());
    context.m_processorReg.addEntry(std::make_unique<MonophonicSubtracksProcessor::Factory>());
    context.m_processorReg.addEntry(std::make_unique<ChordsFromNotesProcessor::Factory>());
    context.m_processorReg.addEntry(std::make_unique<RepeatProcessor::Factory>());
    context.m_processorReg.addEntry(std::make_unique<SilenceProcessor::Factory>());
    context.m_processorReg.addEntry(std::make_unique<SplitAtPitchProcessor::Factory>());
    context.m_processorReg.addEntry(std::make_unique<TransposeProcessor::Factory>());
}

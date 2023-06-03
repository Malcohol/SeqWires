/**
 * Register factories etc. for SeqWires into BabelWires.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <SeqWiresLib/libRegistration.hpp>

#include <SeqWiresLib/Functions/fingeredChordsFunction.hpp>
#include <SeqWiresLib/Functions/monophonicSubtracksFunction.hpp>
#include <SeqWiresLib/Functions/percussionMapFunction.hpp>
#include <SeqWiresLib/Percussion/abstractPercussionSet.hpp>
#include <SeqWiresLib/Percussion/builtInPercussionInstruments.hpp>
#include <SeqWiresLib/Processors/chordMapProcessor.hpp>
#include <SeqWiresLib/Processors/concatenateProcessor.hpp>
#include <SeqWiresLib/Processors/excerptProcessor.hpp>
#include <SeqWiresLib/Processors/fingeredChordsProcessor.hpp>
#include <SeqWiresLib/Processors/mergeProcessor.hpp>
#include <SeqWiresLib/Processors/monophonicSubtracksProcessor.hpp>
#include <SeqWiresLib/Processors/percussionMapProcessor.hpp>
#include <SeqWiresLib/Processors/quantizeProcessor.hpp>
#include <SeqWiresLib/Processors/repeatProcessor.hpp>
#include <SeqWiresLib/Processors/silenceProcessor.hpp>
#include <SeqWiresLib/Processors/splitAtPitchProcessor.hpp>
#include <SeqWiresLib/Processors/transposeProcessor.hpp>
#include <SeqWiresLib/chord.hpp>
#include <SeqWiresLib/pitch.hpp>
#include <SeqWiresLib/Types/Track/trackTypeConstructor.hpp>

#include <BabelWiresLib/Processors/processorFactory.hpp>
#include <BabelWiresLib/Processors/processorFactoryRegistry.hpp>
#include <BabelWiresLib/Project/projectContext.hpp>
#include <BabelWiresLib/TypeSystem/typeSystem.hpp>

void seqwires::registerLib(babelwires::ProjectContext& context) {
    context.m_typeSystem.addEntry<DefaultTrackType>();
    context.m_typeSystem.addEntry<ChordType>();
    context.m_typeSystem.addEntry<PitchClass>();
    context.m_typeSystem.addEntry<PitchEnum>();
    context.m_typeSystem.addEntry<AbstractPercussionSet>();
    context.m_typeSystem.addEntry<BuiltInPercussionInstruments>();
    context.m_typeSystem.addEntry<MonophonicSubtracksPolicyEnum>();
    context.m_typeSystem.addEntry<FingeredChordsSustainPolicyEnum>();

    context.m_typeSystem.addTypeConstructor<TrackTypeConstructor>();
    context.m_typeSystem.addTypeConstructor<PercussionMapType>();

    context.m_typeSystem.addRelatedTypes(BuiltInPercussionInstruments::getThisIdentifier(),
                                         {{}, {AbstractPercussionSet::getThisIdentifier()}});

    context.m_processorReg.addEntry<ChordMapProcessor::Factory>();
    context.m_processorReg.addEntry<ConcatenateProcessor::Factory>();
    context.m_processorReg.addEntry<ExcerptProcessor::Factory>();
    context.m_processorReg.addEntry<MergeProcessor::Factory>();
    context.m_processorReg.addEntry<MonophonicSubtracksProcessor::Factory>();
    context.m_processorReg.addEntry<FingeredChordsProcessor::Factory>();
    context.m_processorReg.addEntry<PercussionMapProcessor::Factory>();
    context.m_processorReg.addEntry<QuantizeProcessor::Factory>();
    context.m_processorReg.addEntry<RepeatProcessor::Factory>();
    context.m_processorReg.addEntry<SilenceProcessor::Factory>();
    context.m_processorReg.addEntry<SplitAtPitchProcessor::Factory>();
    context.m_processorReg.addEntry<TransposeProcessor::Factory>();
}

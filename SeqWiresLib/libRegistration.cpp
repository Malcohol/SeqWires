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
#include <SeqWiresLib/Types/Track/trackTypeConstructor.hpp>
#include <SeqWiresLib/Types/tempo.hpp>
#include <SeqWiresLib/Types/duration.hpp>
#include <SeqWiresLib/chord.hpp>
#include <SeqWiresLib/pitch.hpp>

#include <BabelWiresLib/Processors/processorFactory.hpp>
#include <BabelWiresLib/Processors/processorFactoryRegistry.hpp>
#include <BabelWiresLib/Project/projectContext.hpp>
#include <BabelWiresLib/TypeSystem/typeSystem.hpp>

void seqwires::registerLib(babelwires::ProjectContext& context) {
    context.m_typeSystem.addEntry<DefaultTrackType>();
    context.m_typeSystem.addEntry<ChordType>();
    context.m_typeSystem.addEntry<NoChord>();
    context.m_typeSystem.addEntry<PitchClass>();
    context.m_typeSystem.addEntry<PitchEnum>();
    context.m_typeSystem.addEntry<BuiltInPercussionInstruments>();
    context.m_typeSystem.addEntry<Duration>();
    context.m_typeSystem.addEntry<Tempo>();

    context.m_typeSystem.addEntry<SplitAtPitchProcessorInput>();
    context.m_typeSystem.addEntry<SplitAtPitchProcessorOutput>();
    context.m_processorReg.addProcessor<SplitAtPitchProcessor>();

    context.m_typeSystem.addEntry<MonophonicSubtracksPolicyEnum>();
    context.m_typeSystem.addEntry<MonophonicSubtracksProcessorInput>();
    context.m_typeSystem.addEntry<MonophonicSubtracksProcessorOutput>();
    context.m_processorReg.addProcessor<MonophonicSubtracksProcessor>();

    context.m_typeSystem.addEntry<FingeredChordsSustainPolicyEnum>();
    context.m_typeSystem.addEntry<FingeredChordsProcessorInput>();
    context.m_typeSystem.addEntry<FingeredChordsProcessorOutput>();
    context.m_processorReg.addProcessor<FingeredChordsProcessor>();

    context.m_typeSystem.addEntry<MergeProcessorInput>();
    context.m_typeSystem.addEntry<MergeProcessorOutput>();
    context.m_processorReg.addProcessor<MergeProcessor>();

    context.m_typeSystem.addEntry<SilenceProcessorInput>();
    context.m_typeSystem.addEntry<SilenceProcessorOutput>();
    context.m_processorReg.addProcessor<SilenceProcessor>();

    context.m_typeSystem.addEntry<ConcatenateProcessorInput>();
    context.m_typeSystem.addEntry<ConcatenateProcessorOutput>();
    context.m_processorReg.addProcessor<ConcatenateProcessor>();

    context.m_typeSystem.addEntry<ExcerptProcessorInput>();
    context.m_typeSystem.addEntry<ExcerptProcessorOutput>();
    context.m_processorReg.addProcessor<ExcerptProcessor>();

    context.m_typeSystem.addEntry<RepeatProcessorInput>();
    context.m_typeSystem.addEntry<RepeatProcessorOutput>();
    context.m_processorReg.addProcessor<RepeatProcessor>();

    context.m_typeSystem.addEntry<TransposeProcessorInput>();
    context.m_typeSystem.addEntry<TransposeProcessorOutput>();
    context.m_processorReg.addProcessor<TransposeProcessor>();

    context.m_typeSystem.addEntry<QuantizeProcessorInput>();
    context.m_typeSystem.addEntry<QuantizeProcessorOutput>();
    context.m_processorReg.addProcessor<QuantizeProcessor>();

    context.m_typeSystem.addEntry<ChordMapProcessorInput>();
    context.m_typeSystem.addEntry<ChordMapProcessorOutput>();
    context.m_processorReg.addProcessor<ChordMapProcessor>();

    context.m_typeSystem.addEntry<PercussionMapProcessorInput>();
    context.m_typeSystem.addEntry<PercussionMapProcessorOutput>();
    context.m_processorReg.addProcessor<PercussionMapProcessor>();

    context.m_typeSystem.addTypeConstructor<TrackTypeConstructor>();
    context.m_typeSystem.addTypeConstructor<PercussionMapType>();
}

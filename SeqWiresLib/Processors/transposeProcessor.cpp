/**
 * A processor that adjusts the pitch of note events.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <SeqWiresLib/Processors/transposeProcessor.hpp>

#include <SeqWiresLib/Functions/transposeFunction.hpp>
#include <SeqWiresLib/Types/Track/trackInstance.hpp>

#include <BabelWiresLib/Types/Int/intTypeConstructor.hpp>
#include <BabelWiresLib/Types/Int/intValue.hpp>

#include <Common/Identifiers/registeredIdentifier.hpp>

#include <set>

seqwires::TransposeProcessorInput::TransposeProcessorInput()
    : babelwires::ParallelProcessorInputBase(
          {{BW_SHORT_ID("Offset", "Pitch Offset", "5cfa1541-f25e-4671-ac11-2ff71c883418"),
            babelwires::TypeRef{
                babelwires::IntTypeConstructor::getThisIdentifier(),
                babelwires::TypeConstructorArguments{
                    {}, {babelwires::IntValue(-127), babelwires::IntValue(127), babelwires::IntValue(0)}}}}},
          TransposeProcessor::getCommonArrayId(), seqwires::DefaultTrackType::getThisType()) {}

seqwires::TransposeProcessor::TransposeProcessor(const babelwires::ProjectContext& projectContext)
    : babelwires::ParallelProcessor(projectContext, TransposeProcessorInput::getThisType(),
                                         TransposeProcessorOutput::getThisType()) {}

seqwires::TransposeProcessorOutput::TransposeProcessorOutput()
    : babelwires::ParallelProcessorOutputBase(TransposeProcessor::getCommonArrayId(),
                                                   seqwires::DefaultTrackType::getThisType()) {}

babelwires::ShortId seqwires::TransposeProcessor::getCommonArrayId() {
    return BW_SHORT_ID("Tracks", "Tracks", "83f05b66-7890-4542-8344-1409e50539b5");
}

void seqwires::TransposeProcessor::processEntry(babelwires::UserLogger& userLogger,
                                                const babelwires::ValueTreeNode& input,
                                                const babelwires::ValueTreeNode& inputEntry,
                                                babelwires::ValueTreeNode& outputEntry) const {
    TransposeProcessorInput::ConstInstance in{input};
    babelwires::ConstInstance<TrackType> entryIn{inputEntry};
    babelwires::Instance<TrackType> entryOut{outputEntry};

    entryOut.set(transposeTrack(entryIn.get(), in.getOffset().get()));
}

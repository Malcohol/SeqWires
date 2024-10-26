/**
 * A processor which extracts a section of sequence data from a track.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <SeqWiresLib/Processors/excerptProcessor.hpp>

#include <SeqWiresLib/Functions/excerptFunction.hpp>
#include <SeqWiresLib/Types/Track/trackInstance.hpp>
#include <SeqWiresLib/Types/duration.hpp>

#include <BabelWiresLib/Types/Rational/rationalValue.hpp>

#include <Common/Identifiers/registeredIdentifier.hpp>

seqwires::ExcerptProcessorInput::ExcerptProcessorInput()
    : babelwires::ParallelProcessorInputBase(
          {{BW_SHORT_ID("Start", "Start", "4b95f5db-a542-4660-a8db-97d3a5f831ca"), Duration::getThisIdentifier()},
           {BW_SHORT_ID("Duratn", "Duration", "d83ebbc2-1492-4578-a3b8-4969eb6a2042"), Duration::getThisIdentifier()}},
          ExcerptProcessor::getCommonArrayId(), seqwires::DefaultTrackType::getThisIdentifier()) {}

seqwires::ExcerptProcessorOutput::ExcerptProcessorOutput()
    : babelwires::ParallelProcessorOutputBase(ExcerptProcessor::getCommonArrayId(),
                                                   seqwires::DefaultTrackType::getThisIdentifier()) {}

babelwires::ShortId seqwires::ExcerptProcessor::getCommonArrayId() {
    return BW_SHORT_ID("Tracks", "Tracks", "983b3bcb-7086-4791-8e18-d8c7550d45d3");
}

seqwires::ExcerptProcessor::ExcerptProcessor(const babelwires::ProjectContext& projectContext)
    : babelwires::ParallelProcessor(projectContext, ExcerptProcessorInput::getThisIdentifier(), ExcerptProcessorOutput::getThisIdentifier()) {}

void seqwires::ExcerptProcessor::processEntry(babelwires::UserLogger& userLogger, const babelwires::ValueTreeNode& inputFeature,
                          const babelwires::ValueTreeNode& inputEntry, babelwires::ValueTreeNode& outputEntry)  const {
    ExcerptProcessorInput::ConstInstance input{inputFeature};                        
    babelwires::ConstInstance<TrackType> entryIn{inputEntry};
    babelwires::Instance<TrackType> entryOut{outputEntry};

    entryOut.set(getTrackExcerpt(entryIn.get(), input.getStart().get(), input.getDuratn().get()));
}

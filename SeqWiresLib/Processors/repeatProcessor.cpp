/**
 * A processor which repeats sequence data a number of times.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <SeqWiresLib/Processors/repeatProcessor.hpp>

#include <SeqWiresLib/Functions/appendTrackFunction.hpp>
#include <SeqWiresLib/Types/Track/trackInstance.hpp>

#include <BabelWiresLib/Features/arrayFeature.hpp>
#include <BabelWiresLib/Types/Int/intTypeConstructor.hpp>
#include <BabelWiresLib/Types/Int/intValue.hpp>

#include <Common/Identifiers/registeredIdentifier.hpp>

seqwires::RepeatProcessorInput::RepeatProcessorInput()
    : babelwires::ParallelValueProcessorInputBase(
          {{BW_SHORT_ID("Count", "Count", "f5d2ab08-4430-47fa-b26c-0ff2154826e3"),
            babelwires::TypeRef{babelwires::IntTypeConstructor::getThisIdentifier(),
                                babelwires::TypeConstructorArguments{
                                    {},
                                    {babelwires::IntValue(0),
                                     babelwires::IntValue(std::numeric_limits<babelwires::IntValue::NativeType>::max()),
                                     babelwires::IntValue(2)}}}}},
          RepeatProcessor::getCommonArrayId(), seqwires::DefaultTrackType::getThisIdentifier()) {}

seqwires::RepeatProcessorOutput::RepeatProcessorOutput()
    : babelwires::ParallelValueProcessorOutputBase(RepeatProcessor::getCommonArrayId(),
                                                   seqwires::DefaultTrackType::getThisIdentifier()) {}

seqwires::RepeatProcessor::RepeatProcessor(const babelwires::ProjectContext& projectContext)
    : babelwires::ParallelValueProcessor(projectContext, RepeatProcessorInput::getThisIdentifier(),
                                         RepeatProcessorOutput::getThisIdentifier()) {}

babelwires::ShortId seqwires::RepeatProcessor::getCommonArrayId() {
    return BW_SHORT_ID("Tracks", "Tracks", "f727937f-0215-4527-bab4-0eca269d6c5c");
}

void seqwires::RepeatProcessor::processEntry(babelwires::UserLogger& userLogger,
                                             const babelwires::ValueFeature& inputFeature,
                                             const babelwires::ValueFeature& inputEntry,
                                             babelwires::ValueFeature& outputEntry) const {
    RepeatProcessorInput::ConstInstance input{inputFeature};
    babelwires::ConstInstance<TrackType> entryIn{inputEntry};
    babelwires::Instance<TrackType> entryOut{outputEntry};
    Track trackOut;

    const Track& trackIn = entryIn.get();
    for (int i = 0; i < input.getCount().get(); ++i) {
        appendTrack(trackOut, trackIn);
    }
    entryOut.set(std::move(trackOut));
}

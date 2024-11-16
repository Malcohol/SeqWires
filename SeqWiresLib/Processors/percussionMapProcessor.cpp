/**
 * Processor which applies a map to chord events.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <SeqWiresLib/Processors/percussionMapProcessor.hpp>

#include <SeqWiresLib/Functions/percussionMapFunction.hpp>

#include <BabelWiresLib/TypeSystem/typeSystem.hpp>
#include <BabelWiresLib/Types/Map/mapTypeConstructor.hpp>
#include <BabelWiresLib/Types/Map/mapValue.hpp>

#include <Common/Identifiers/registeredIdentifier.hpp>

seqwires::PercussionMapProcessorInput::PercussionMapProcessorInput()
    : babelwires::ParallelProcessorInputBase(
          {{BW_SHORT_ID("Map", "Map", "b8cbf8c9-579b-4292-bdef-524b7d1010bc"), seqwires::getPercussionMapType()}},
          PercussionMapProcessor::getCommonArrayId(), seqwires::DefaultTrackType::getThisType()) {}

seqwires::PercussionMapProcessorOutput::PercussionMapProcessorOutput()
    : babelwires::ParallelProcessorOutputBase(PercussionMapProcessor::getCommonArrayId(),
                                                   seqwires::DefaultTrackType::getThisType()) {}

seqwires::PercussionMapProcessor::PercussionMapProcessor(const babelwires::ProjectContext& projectContext)
    : babelwires::ParallelProcessor(projectContext, PercussionMapProcessorInput::getThisType(),
                                         PercussionMapProcessorOutput::getThisType()) {}

babelwires::ShortId seqwires::PercussionMapProcessor::getCommonArrayId() {
    return BW_SHORT_ID("Tracks", "Tracks", "fe71b1c6-6604-430b-a731-f40b2692d2cf");
}

void seqwires::PercussionMapProcessor::processEntry(babelwires::UserLogger& userLogger,
                                                    const babelwires::ValueTreeNode& input,
                                                    const babelwires::ValueTreeNode& inputEntry,
                                                    babelwires::ValueTreeNode& outputEntry) const {
    PercussionMapProcessorInput::ConstInstance in{input};
    babelwires::ConstInstance<TrackType> entryIn{inputEntry};
    babelwires::Instance<TrackType> entryOut{outputEntry};

    const auto& percMap = in.getMap()->getValue()->is<babelwires::MapValue>();

    entryOut.set(mapPercussionFunction(in->getTypeSystem(), entryIn.get(), percMap));
}

/**
 * Processor which applies a map to chord events.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <SeqWiresLib/Processors/chordMapProcessor.hpp>

#include <SeqWiresLib/Functions/mapChordsFunction.hpp>
#include <SeqWiresLib/chord.hpp>
#include <SeqWiresLib/pitch.hpp>

#include <BabelWiresLib/TypeSystem/typeSystem.hpp>
#include <BabelWiresLib/Types/Map/mapTypeConstructor.hpp>
#include <BabelWiresLib/Types/Map/mapValue.hpp>

#include <Common/Identifiers/registeredIdentifier.hpp>

seqwires::ChordMapProcessorInput::ChordMapProcessorInput()
    : babelwires::ParallelProcessorInputBase(
          {{BW_SHORT_ID("TypMap", "Type map", "6054b8e9-5f48-4e9f-8807-b6377d36d6aa"),
            babelwires::MapTypeConstructor::makeTypeRef(seqwires::getMapChordFunctionChordTypeRef(),
                                                        seqwires::getMapChordFunctionChordTypeRef(),
                                                        babelwires::MapEntryData::Kind::All2Sm)},
           {BW_SHORT_ID("RtMap", "Root map", "4df92989-554b-426a-aa0c-2c0c7ca2dfd6"),
            babelwires::MapTypeConstructor::makeTypeRef(seqwires::getMapChordFunctionPitchClassRef(),
                                                        seqwires::getMapChordFunctionPitchClassRef(),
                                                        babelwires::MapEntryData::Kind::All2Sm)}},
          ChordMapProcessor::getCommonArrayId(), seqwires::DefaultTrackType::getThisType()) {}

seqwires::ChordMapProcessorOutput::ChordMapProcessorOutput()
    : babelwires::ParallelProcessorOutputBase(ChordMapProcessor::getCommonArrayId(),
                                              seqwires::DefaultTrackType::getThisType()) {}

seqwires::ChordMapProcessor::ChordMapProcessor(const babelwires::ProjectContext& projectContext)
    : babelwires::ParallelProcessor(projectContext, ChordMapProcessorInput::getThisType(),
                                    ChordMapProcessorOutput::getThisType()) {}

babelwires::ShortId seqwires::ChordMapProcessor::getCommonArrayId() {
    return BW_SHORT_ID("Tracks", "Tracks", "24e56b0d-eb1e-4c93-97fd-ba4d639e112a");
}

void seqwires::ChordMapProcessor::processEntry(babelwires::UserLogger& userLogger,
                                               const babelwires::ValueTreeNode& input,
                                               const babelwires::ValueTreeNode& inputEntry,
                                               babelwires::ValueTreeNode& outputEntry) const {
    ChordMapProcessorInput::ConstInstance in{input};
    babelwires::ConstInstance<TrackType> entryIn{inputEntry};
    babelwires::Instance<TrackType> entryOut{outputEntry};

    const auto& chordTypeMap = in.getTypMap()->getValue()->is<babelwires::MapValue>();
    const auto& chordRootMap = in.getRtMap()->getValue()->is<babelwires::MapValue>();

    entryOut.set(mapChordsFunction(in->getTypeSystem(), entryIn.get(), chordTypeMap, chordRootMap));
}

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

#include <BabelWiresLib/Features/rootFeature.hpp>
#include <BabelWiresLib/TypeSystem/typeSystem.hpp>
#include <BabelWiresLib/Types/Enum/addBlankToEnum.hpp>
#include <BabelWiresLib/Types/Map/mapTypeConstructor.hpp>
#include <BabelWiresLib/Types/Map/mapValue.hpp>

#include <Common/Identifiers/registeredIdentifier.hpp>

seqwires::ChordMapProcessorInput::ChordMapProcessorInput()
    : babelwires::ParallelValueProcessorInputBase(
          {{BW_SHORT_ID("TypMap", "Type map", "6054b8e9-5f48-4e9f-8807-b6377d36d6aa"),
            babelwires::TypeRef{
                babelwires::MapTypeConstructor::getThisIdentifier(),
                babelwires::TypeConstructorArguments{
                    {seqwires::getMapChordFunctionChordTypeRef(), seqwires::getMapChordFunctionChordTypeRef()},
                    {babelwires::EnumValue(babelwires::MapEntryFallbackKind::getIdentifierFromValue(
                        babelwires::MapEntryData::Kind::All2Sm))}}}},
           {BW_SHORT_ID("RtMap", "Root map", "4df92989-554b-426a-aa0c-2c0c7ca2dfd6"),
            babelwires::TypeRef{
                babelwires::MapTypeConstructor::getThisIdentifier(),
                babelwires::TypeConstructorArguments{
                    {seqwires::getMapChordFunctionPitchClassRef(), seqwires::getMapChordFunctionPitchClassRef()},
                    {babelwires::EnumValue(babelwires::MapEntryFallbackKind::getIdentifierFromValue(
                        babelwires::MapEntryData::Kind::All2Sm))}}}}},
          ChordMapProcessor::getCommonArrayId(), seqwires::DefaultTrackType::getThisIdentifier()) {}

seqwires::ChordMapProcessorOutput::ChordMapProcessorOutput()
    : babelwires::ParallelValueProcessorOutputBase(ChordMapProcessor::getCommonArrayId(),
                                                   seqwires::DefaultTrackType::getThisIdentifier()) {}

seqwires::ChordMapProcessor::ChordMapProcessor(const babelwires::ProjectContext& projectContext)
    : babelwires::ParallelValueProcessor(projectContext, ChordMapProcessorInput::getThisIdentifier(),
                                         ChordMapProcessorOutput::getThisIdentifier()) {}

babelwires::ShortId seqwires::ChordMapProcessor::getCommonArrayId() {
    return BW_SHORT_ID("Tracks", "Tracks", "24e56b0d-eb1e-4c93-97fd-ba4d639e112a");
}

void seqwires::ChordMapProcessor::processEntry(babelwires::UserLogger& userLogger,
                                               const babelwires::ValueFeature& inputFeature,
                                               const babelwires::ValueFeature& inputEntry,
                                               babelwires::ValueFeature& outputEntry) const {
    ChordMapProcessorInput::ConstInstance input{inputFeature};
    babelwires::ConstInstance<TrackType> entryIn{inputEntry};
    babelwires::Instance<TrackType> entryOut{outputEntry};

    const auto& chordTypeMap = input.getTypMap()->getValue()->is<babelwires::MapValue>();
    const auto& chordRootMap = input.getRtMap()->getValue()->is<babelwires::MapValue>();

    entryOut.set(mapChordsFunction(input->getTypeSystem(), entryIn.get(), chordTypeMap, chordRootMap));
}

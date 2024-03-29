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
#include <BabelWiresLib/Project/projectContext.hpp>
#include <BabelWiresLib/TypeSystem/typeSystem.hpp>
#include <BabelWiresLib/Types/Enum/addBlankToEnum.hpp>
#include <BabelWiresLib/Types/Map/mapFeature.hpp>

#include <Common/Identifiers/registeredIdentifier.hpp>

namespace {
    struct ChordTypeMap : babelwires::MapFeature {
        ChordTypeMap()
            : babelwires::MapFeature(seqwires::getMapChordFunctionChordTypeRef(),
                                      seqwires::getMapChordFunctionChordTypeRef(),
                                      babelwires::MapEntryData::Kind::All2Sm) {}
    };

    struct PitchClassMap : babelwires::MapFeature {
        PitchClassMap()
            : babelwires::MapFeature(seqwires::getMapChordFunctionPitchClassRef(),
                                             seqwires::getMapChordFunctionPitchClassRef(),
                                      babelwires::MapEntryData::Kind::All2Sm) {}
    };
} // namespace

seqwires::ChordMapProcessor::ChordMapProcessor(const babelwires::ProjectContext& context)
    : babelwires::ParallelProcessor<seqwires::TrackFeature, seqwires::TrackFeature>(context) {
    m_chordTypeMapFeature = m_inputFeature->addField(
        std::make_unique<ChordTypeMap>(), BW_SHORT_ID("TypMap", "Type map", "6054b8e9-5f48-4e9f-8807-b6377d36d6aa"));
    m_pitchClassMapFeature = m_inputFeature->addField(
        std::make_unique<PitchClassMap>(), BW_SHORT_ID("RtMap", "Root map", "4df92989-554b-426a-aa0c-2c0c7ca2dfd6"));
    addArrayFeature(BW_SHORT_ID("Tracks", "Tracks", "24e56b0d-eb1e-4c93-97fd-ba4d639e112a"));
}

seqwires::ChordMapProcessor::Factory::Factory()
    : CommonProcessorFactory(BW_LONG_ID("ChordMapProcessor", "Chord Map", "b7227130-8274-4451-bd60-8fe34a74c4b6"), 1) {}

void seqwires::ChordMapProcessor::processEntry(babelwires::UserLogger& userLogger, const seqwires::TrackFeature& input,
                                               seqwires::TrackFeature& output) const {
    const babelwires::ProjectContext& context = babelwires::RootFeature::getProjectContextAt(*m_chordTypeMapFeature);

    output.set(mapChordsFunction(context.m_typeSystem, input.get(), m_chordTypeMapFeature->get(),
                                 m_pitchClassMapFeature->get()));
}

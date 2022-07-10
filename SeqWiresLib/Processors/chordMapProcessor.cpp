/**
 * Processor which applies a map to chord events.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <SeqWiresLib/Processors/chordMapProcessor.hpp>

#include <SeqWiresLib/Tracks/chordEvents.hpp>
#include <SeqWiresLib/Tracks/trackEventHolder.hpp>
#include <SeqWiresLib/chord.hpp>
#include <SeqWiresLib/pitchClass.hpp>

#include <BabelWiresLib/Features/mapFeature.hpp>
#include <BabelWiresLib/Features/rootFeature.hpp>
#include <BabelWiresLib/Maps/Helpers/enumValueAdapters.hpp>
#include <BabelWiresLib/Maps/Helpers/enumSourceMapApplicator.hpp>
#include <BabelWiresLib/Project/projectContext.hpp>
#include <BabelWiresLib/TypeSystem/typeSystem.hpp>

#include <Common/Identifiers/registeredIdentifier.hpp>

namespace {
    struct ChordTypeMap : babelwires::MapFeature {
        ChordTypeMap()
            : babelwires::MapFeature(seqwires::ChordType::getThisIdentifier(), seqwires::ChordType::getThisIdentifier()) {}
    };

    struct PitchClassMap : babelwires::MapFeature {
        PitchClassMap()
            : babelwires::MapFeature(seqwires::PitchClass::getThisIdentifier(), seqwires::PitchClass::getThisIdentifier()) {}
    };
} // namespace

seqwires::ChordMapProcessor::ChordMapProcessor(const babelwires::ProjectContext& context)
    : babelwires::ParallelProcessor<seqwires::TrackFeature, seqwires::TrackFeature>(context) {
    m_chordTypeMapFeature = m_inputFeature->addField(
        std::make_unique<ChordTypeMap>(), REGISTERED_ID("TypMap", "Type map", "6054b8e9-5f48-4e9f-8807-b6377d36d6aa"));
    m_pitchClassMapFeature = m_inputFeature->addField(
        std::make_unique<PitchClassMap>(), REGISTERED_ID("RtMap", "Root map", "4df92989-554b-426a-aa0c-2c0c7ca2dfd6"));
    addArrayFeature(REGISTERED_ID("Tracks", "Tracks", "24e56b0d-eb1e-4c93-97fd-ba4d639e112a"));
}

seqwires::ChordMapProcessor::Factory::Factory()
    : CommonProcessorFactory(
          REGISTERED_LONGID("ChordMapProcessor", "Chord Map", "b7227130-8274-4451-bd60-8fe34a74c4b6"), 1) {}

void seqwires::ChordMapProcessor::processEntry(babelwires::UserLogger& userLogger, const seqwires::TrackFeature& input,
                                               seqwires::TrackFeature& output) const {
    const babelwires::ProjectContext& context = babelwires::RootFeature::getProjectContextAt(*m_chordTypeMapFeature);

    const babelwires::MapData& chordTypeMapData = m_chordTypeMapFeature->get();
    if (!chordTypeMapData.isValid(context.m_typeSystem)) {
        throw babelwires::ModelException() << "The Chord Type Map is not valid.";
    }

    const ChordType& chordType =
        context.m_typeSystem.getEntryByIdentifier(seqwires::ChordType::getThisIdentifier())->is<ChordType>();
    const babelwires::EnumToValueValueAdapter<ChordType> chordTypeTargetAdapter{chordType};
    babelwires::EnumSourceMapApplicator<ChordType, ChordType::Value> chordTypeApplicator(chordTypeMapData, chordType, chordTypeTargetAdapter);

    const babelwires::MapData& pitchClassMapData = m_pitchClassMapFeature->get();
    if (!pitchClassMapData.isValid(context.m_typeSystem)) {
        throw babelwires::ModelException() << "The Pitch Class Map is not valid.";
    }

    const PitchClass& pitchClass =
        context.m_typeSystem.getEntryByIdentifier(seqwires::PitchClass::getThisIdentifier())->is<PitchClass>();
    const babelwires::EnumToValueValueAdapter<PitchClass> pitchClassTargetAdapter{pitchClass};
    babelwires::EnumSourceMapApplicator<PitchClass, PitchClass::Value> pitchClassApplicator(pitchClassMapData, pitchClass, pitchClassTargetAdapter);

    const Track& trackIn = input.get();
    Track trackOut;

    for (auto it = trackIn.begin(); it != trackIn.end(); ++it) {
        if (it->as<ChordOnEvent>()) {
            TrackEventHolder holder(*it);
            Chord& chord = holder->is<ChordOnEvent>().m_chord;
            chord.m_chordType = chordTypeApplicator[chord.m_chordType];
            //chord.m_root = pitchClassApplicator[chord.m_root];
            trackOut.addEvent(holder.release());
        } else {
            trackOut.addEvent(*it);
        }
    }
    trackOut.setDuration(trackIn.getDuration());
}

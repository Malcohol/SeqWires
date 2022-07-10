/**
 * Function which applies maps to chord events.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include "SeqWiresLib/Functions/mapChordsFunction.hpp"

#include <SeqWiresLib/Tracks/chordEvents.hpp>
#include <SeqWiresLib/Tracks/trackEventHolder.hpp>
#include <SeqWiresLib/chord.hpp>
#include <SeqWiresLib/pitchClass.hpp>

#include <BabelWiresLib/Features/mapFeature.hpp>
#include <BabelWiresLib/Maps/Helpers/enumValueAdapters.hpp>
#include <BabelWiresLib/Maps/Helpers/enumSourceMapApplicator.hpp>
#include <BabelWiresLib/TypeSystem/typeSystem.hpp>
#include <BabelWiresLib/Features/modelExceptions.hpp>

seqwires::Track seqwires::mapChordsFunction(const babelwires::TypeSystem& typeSystem, const Track& sourceTrack, const babelwires::MapData& chordTypeMapData, const babelwires::MapData& pitchClassMapData) {

    if (!chordTypeMapData.isValid(typeSystem)) {
        throw babelwires::ModelException() << "The Chord Type Map is not valid.";
    }

    const ChordType& chordType =
        typeSystem.getEntryByIdentifier(seqwires::ChordType::getThisIdentifier())->is<ChordType>();
    const babelwires::EnumToValueValueAdapter<ChordType> chordTypeTargetAdapter{chordType};
    babelwires::EnumSourceMapApplicator<ChordType, ChordType::Value> chordTypeApplicator(chordTypeMapData, chordType, chordTypeTargetAdapter);

    if (!pitchClassMapData.isValid(typeSystem)) {
        throw babelwires::ModelException() << "The Pitch Class Map is not valid.";
    }

    const PitchClass& pitchClass =
        typeSystem.getEntryByIdentifier(seqwires::PitchClass::getThisIdentifier())->is<PitchClass>();
    const babelwires::EnumToValueValueAdapter<PitchClass> pitchClassTargetAdapter{pitchClass};
    babelwires::EnumSourceMapApplicator<PitchClass, PitchClass::Value> pitchClassApplicator(pitchClassMapData, pitchClass, pitchClassTargetAdapter);

    Track trackOut;

    for (auto it = sourceTrack.begin(); it != sourceTrack.end(); ++it) {
        if (it->as<ChordOnEvent>()) {
            TrackEventHolder holder(*it);
            Chord& chord = holder->is<ChordOnEvent>().m_chord;
            chord.m_chordType = chordTypeApplicator[chord.m_chordType];
            chord.m_root = pitchClassApplicator[chord.m_root];
            trackOut.addEvent(holder.release());
        } else {
            trackOut.addEvent(*it);
        }
    }
    trackOut.setDuration(sourceTrack.getDuration());
    return trackOut;
}
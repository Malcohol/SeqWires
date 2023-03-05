/**
 * Function which applies maps to chord events.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <SeqWiresLib/Functions/mapChordsFunction.hpp>

#include <SeqWiresLib/Tracks/chordEvents.hpp>
#include <SeqWiresLib/Tracks/trackEventHolder.hpp>
#include <SeqWiresLib/chord.hpp>
#include <SeqWiresLib/pitchClass.hpp>

#include <BabelWiresLib/Enums/addDummy.hpp>
#include <BabelWiresLib/Features/mapFeature.hpp>
#include <BabelWiresLib/Features/modelExceptions.hpp>
#include <BabelWiresLib/Maps/Helpers/enumSourceMapApplicator.hpp>
#include <BabelWiresLib/Maps/Helpers/enumValueAdapters.hpp>
#include <BabelWiresLib/TypeSystem/typeSystem.hpp>

babelwires::TypeRef seqwires::getMapChordFunctionTypeRef() {
    return babelwires::TypeRef(babelwires::AddDummy::getThisIdentifier(), {{seqwires::ChordType::getThisIdentifier()}});
}

seqwires::Track seqwires::mapChordsFunction(const babelwires::TypeSystem& typeSystem, const Track& sourceTrack,
                                            const babelwires::MapData& chordTypeMapData,
                                            const babelwires::MapData& pitchClassMapData) {

    if (!chordTypeMapData.isValid(typeSystem)) {
        throw babelwires::ModelException() << "The Chord Type Map is not valid.";
    }

    babelwires::TypeRef chordTypeWithDummyRef = getMapChordFunctionTypeRef();
    const babelwires::Enum& chordTypeWithDummy = chordTypeWithDummyRef.resolve(typeSystem).is<babelwires::Enum>();
    const babelwires::EnumToIndexValueAdapter chordTypeTargetAdapter{chordTypeWithDummy};
    babelwires::EnumSourceIndexMapApplicator<unsigned int> chordTypeApplicator(chordTypeMapData, chordTypeWithDummy, chordTypeTargetAdapter);

    // The dummy value is always last.
    const unsigned int indexOfDummyValue = chordTypeWithDummy.getEnumValues().size() - 1;

    if (!pitchClassMapData.isValid(typeSystem)) {
        throw babelwires::ModelException() << "The Pitch Class Map is not valid.";
    }

    const PitchClass& pitchClass = typeSystem.getEntryByType<seqwires::PitchClass>();
    const babelwires::EnumToValueValueAdapter<PitchClass> pitchClassTargetAdapter{pitchClass};
    babelwires::EnumSourceMapApplicator<PitchClass, PitchClass::Value> pitchClassApplicator(
        pitchClassMapData, pitchClass, pitchClassTargetAdapter);

    Track trackOut;

    // If an event is dropped, then we need to carry its time forward for the next event.
    bool droppingChordEvent = false;
    ModelDuration timeFromDroppedEvent;

    for (auto it = sourceTrack.begin(); it != sourceTrack.end(); ++it) {
        if (it->as<ChordOnEvent>()) {
            TrackEventHolder holder(*it);
            Chord& chord = holder->is<ChordOnEvent>().m_chord;
            const unsigned int mappedChordIndex = chordTypeApplicator[static_cast<unsigned int>(chord.m_chordType)];
            // The dummy value is always the last entry.
            if (mappedChordIndex != indexOfDummyValue) {
                holder->setTimeSinceLastEvent(holder->getTimeSinceLastEvent() + timeFromDroppedEvent);
                timeFromDroppedEvent = 0;
                chord.m_chordType = static_cast<ChordType::Value>(mappedChordIndex);
                chord.m_root = pitchClassApplicator[chord.m_root];
                trackOut.addEvent(holder.release());
            } else {
                timeFromDroppedEvent += holder->getTimeSinceLastEvent();
                droppingChordEvent = true;
            }
        } else if (droppingChordEvent && it->as<ChordOffEvent>()) {
            timeFromDroppedEvent += it->getTimeSinceLastEvent();
            droppingChordEvent = false;
        } else if (timeFromDroppedEvent > 0) {
            TrackEventHolder holder(*it);
            holder->setTimeSinceLastEvent(holder->getTimeSinceLastEvent() + timeFromDroppedEvent);
            timeFromDroppedEvent = 0;
            trackOut.addEvent(holder.release());
        } else {
            trackOut.addEvent(*it);
        }
    }
    trackOut.setDuration(sourceTrack.getDuration());
    return trackOut;
}
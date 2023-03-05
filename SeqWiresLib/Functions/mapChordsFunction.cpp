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

#include <BabelWiresLib/Enums/addBlank.hpp>
#include <BabelWiresLib/Features/mapFeature.hpp>
#include <BabelWiresLib/Features/modelExceptions.hpp>
#include <BabelWiresLib/Maps/Helpers/enumSourceMapApplicator.hpp>
#include <BabelWiresLib/Maps/Helpers/enumValueAdapters.hpp>
#include <BabelWiresLib/TypeSystem/typeSystem.hpp>

babelwires::TypeRef seqwires::getMapChordFunctionTypeRef() {
    return babelwires::TypeRef(babelwires::AddBlank::getThisIdentifier(), {{seqwires::ChordType::getThisIdentifier()}});
}

seqwires::Track seqwires::mapChordsFunction(const babelwires::TypeSystem& typeSystem, const Track& sourceTrack,
                                            const babelwires::MapData& chordTypeMapData,
                                            const babelwires::MapData& pitchClassMapData) {

    if (!chordTypeMapData.isValid(typeSystem)) {
        throw babelwires::ModelException() << "The Chord Type Map is not valid.";
    }

    babelwires::TypeRef chordTypeWithDummyRef = getMapChordFunctionTypeRef();
    const babelwires::Enum& chordTypeWithBlank = chordTypeWithDummyRef.resolve(typeSystem).is<babelwires::Enum>();
    const babelwires::EnumToIndexValueAdapter chordTypeTargetAdapter{chordTypeWithBlank};
    babelwires::EnumSourceIndexMapApplicator<unsigned int> chordTypeApplicator(chordTypeMapData, chordTypeWithBlank,
                                                                               chordTypeTargetAdapter);

    // The blank value is always last.
    const unsigned int indexOfBlankChordValue = chordTypeWithBlank.getEnumValues().size() - 1;

    if (!pitchClassMapData.isValid(typeSystem)) {
        throw babelwires::ModelException() << "The Pitch Class Map is not valid.";
    }

    const PitchClass& pitchClass = typeSystem.getEntryByType<seqwires::PitchClass>();
    const babelwires::EnumToValueValueAdapter<PitchClass> pitchClassTargetAdapter{pitchClass};
    babelwires::EnumSourceMapApplicator<PitchClass, PitchClass::Value> pitchClassApplicator(
        pitchClassMapData, pitchClass, pitchClassTargetAdapter);

    Track trackOut;
    ModelDuration totalEventDuration;

    // Blank source handling:
    const unsigned int noChordIndex = chordTypeApplicator[indexOfBlankChordValue];
    const bool hasNoChordChord = (noChordIndex != indexOfBlankChordValue);
    const Chord noChordChord{PitchClass::Value::FSharp,
                             hasNoChordChord ? static_cast<ChordType::Value>(noChordIndex) : ChordType::Value::M9};
    bool isChordPlaying = false;

    // Blank target handling:
    // If an event is dropped, then we need to carry its time forward for the next event.
    bool droppingChordEvent = false;
    ModelDuration timeSinceLastEvent;

    for (auto it = sourceTrack.begin(); it != sourceTrack.end(); ++it) {
        timeSinceLastEvent += it->getTimeSinceLastEvent();
        totalEventDuration += it->getTimeSinceLastEvent();

        if (hasNoChordChord && !isChordPlaying && (timeSinceLastEvent > 0)) {
            trackOut.addEvent(ChordOnEvent(0, noChordChord));
            isChordPlaying = true;
        }

        if (it->as<ChordOnEvent>()) {
            TrackEventHolder holder(*it);
            Chord& chord = holder->is<ChordOnEvent>().m_chord;
            const unsigned int mappedChordIndex = chordTypeApplicator[static_cast<unsigned int>(chord.m_chordType)];
            if (mappedChordIndex != indexOfBlankChordValue) {
                if (isChordPlaying) {
                    // Only if a noChordChord was added.
                    trackOut.addEvent(ChordOffEvent(timeSinceLastEvent));
                    timeSinceLastEvent = 0;
                }
                holder->setTimeSinceLastEvent(timeSinceLastEvent);
                timeSinceLastEvent = 0;
                chord.m_chordType = static_cast<ChordType::Value>(mappedChordIndex);
                chord.m_root = pitchClassApplicator[chord.m_root];
                trackOut.addEvent(holder.release());
                isChordPlaying = true;
            } else {
                droppingChordEvent = true;
                isChordPlaying = false;
            }
        } else if (droppingChordEvent && it->as<ChordOffEvent>()) {
            droppingChordEvent = false;
        } else if (timeSinceLastEvent != it->getTimeSinceLastEvent()) {
            if (it->as<ChordOffEvent>()) {
                isChordPlaying = false;
            }
            TrackEventHolder holder(*it);
            holder->setTimeSinceLastEvent(timeSinceLastEvent);
            timeSinceLastEvent = 0;
            trackOut.addEvent(holder.release());
        } else {
            if (it->as<ChordOffEvent>()) {
                isChordPlaying = false;
            }
            trackOut.addEvent(*it);
            timeSinceLastEvent = 0;
        }
    }
    if (hasNoChordChord && (totalEventDuration < sourceTrack.getDuration())) {
        trackOut.addEvent(ChordOnEvent(0, noChordChord));
        trackOut.addEvent(ChordOffEvent(sourceTrack.getDuration() - totalEventDuration));
    }
    
    trackOut.setDuration(sourceTrack.getDuration());
    return trackOut;
}

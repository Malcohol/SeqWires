/**
 * Function which applies maps to chord events.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <SeqWiresLib/Functions/mapChordsFunction.hpp>

#include <SeqWiresLib/Types/Track/TrackEvents/chordEvents.hpp>
#include <SeqWiresLib/Types/Track/TrackEvents/trackEventHolder.hpp>
#include <SeqWiresLib/chord.hpp>
#include <SeqWiresLib/pitch.hpp>

#include <BabelWiresLib/Features/mapFeature.hpp>
#include <BabelWiresLib/Features/modelExceptions.hpp>
#include <BabelWiresLib/Maps/Helpers/enumSourceMapApplicator.hpp>
#include <BabelWiresLib/Maps/Helpers/enumValueAdapters.hpp>
#include <BabelWiresLib/TypeSystem/typeSystem.hpp>
#include <BabelWiresLib/Types/Enum/addBlankToEnum.hpp>

babelwires::TypeRef seqwires::getMapChordFunctionChordTypeRef() {
    return babelwires::TypeRef(babelwires::AddBlankToEnum::getThisIdentifier(),
                               seqwires::ChordType::getThisIdentifier());
}

babelwires::TypeRef seqwires::getMapChordFunctionPitchClassRef() {
    return babelwires::TypeRef(babelwires::AddBlankToEnum::getThisIdentifier(),
                               seqwires::PitchClass::getThisIdentifier());
}

seqwires::Track seqwires::mapChordsFunction(const babelwires::TypeSystem& typeSystem, const Track& sourceTrack,
                                            const babelwires::MapData& chordTypeMapData,
                                            const babelwires::MapData& pitchClassMapData) {

    if (!chordTypeMapData.isValid(typeSystem)) {
        throw babelwires::ModelException() << "The Chord Type Map is not valid.";
    }
    if (!pitchClassMapData.isValid(typeSystem)) {
        throw babelwires::ModelException() << "The Pitch Class Map is not valid.";
    }

    const babelwires::TypeRef chordTypeWithBlankTypeRef = getMapChordFunctionChordTypeRef();
    const babelwires::Enum& chordTypeWithBlank = chordTypeWithBlankTypeRef.resolve(typeSystem).is<babelwires::Enum>();
    const babelwires::EnumToIndexValueAdapter chordTypeTargetAdapter{chordTypeWithBlank};
    const babelwires::EnumSourceIndexMapApplicator<unsigned int> chordTypeApplicator(
        chordTypeMapData, chordTypeWithBlank, chordTypeTargetAdapter);
    // The blank value is always last.
    const unsigned int indexOfBlankChordValue = chordTypeWithBlank.getEnumValues().size() - 1;

    const babelwires::TypeRef pitchClassWithBlankTypeRef = getMapChordFunctionPitchClassRef();
    const babelwires::Enum& pitchClassWithBlank = pitchClassWithBlankTypeRef.resolve(typeSystem).is<babelwires::Enum>();
    const babelwires::EnumToIndexValueAdapter pitchClassTargetAdapter{pitchClassWithBlank};
    const babelwires::EnumSourceIndexMapApplicator<unsigned int> pitchClassApplicator(
        pitchClassMapData, pitchClassWithBlank, pitchClassTargetAdapter);
    const unsigned int indexOfBlankPitchClass = pitchClassWithBlank.getEnumValues().size() - 1;

    Track trackOut;
    ModelDuration totalEventDuration;

    // Blank source handling:
    const unsigned int noChordChordTypeIndex = chordTypeApplicator[indexOfBlankChordValue];
    const unsigned int noChordPitchClassIndex = pitchClassApplicator[indexOfBlankPitchClass];
    const bool hasNoChordChord =
        (noChordChordTypeIndex != indexOfBlankChordValue) && (noChordPitchClassIndex != indexOfBlankPitchClass);
    const Chord noChordChord{
        hasNoChordChord ? static_cast<PitchClass::Value>(noChordPitchClassIndex) : PitchClass::Value::C,
        hasNoChordChord ? static_cast<ChordType::Value>(noChordChordTypeIndex) : ChordType::Value::M};
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
            const unsigned int mappedChordTypeIndex = chordTypeApplicator[static_cast<unsigned int>(chord.m_chordType)];
            const unsigned int mappedPitchClassIndex = pitchClassApplicator[static_cast<unsigned int>(chord.m_root)];
            if ((mappedChordTypeIndex != indexOfBlankChordValue) && (mappedPitchClassIndex != indexOfBlankPitchClass)) {
                if (isChordPlaying) {
                    // Only if a noChordChord was added.
                    trackOut.addEvent(ChordOffEvent(timeSinceLastEvent));
                    timeSinceLastEvent = 0;
                }
                holder->setTimeSinceLastEvent(timeSinceLastEvent);
                timeSinceLastEvent = 0;
                chord.m_chordType = static_cast<ChordType::Value>(mappedChordTypeIndex);
                chord.m_root = static_cast<PitchClass::Value>(mappedPitchClassIndex);
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

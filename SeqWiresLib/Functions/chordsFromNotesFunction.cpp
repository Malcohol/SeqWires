/**
 * Function which creates tracks of chord events from tracks of note events.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include "SeqWiresLib/Functions/chordsFromNotesFunction.hpp"

#include "SeqWiresLib/Tracks/chordEvents.hpp"
#include "SeqWiresLib/Utilities/filteredTrackIterator.hpp"

namespace {
    // TODO: Offer different schemes.
    // TODO: Optional intervals (but are they in tention with inversion?)
    /// Represent a chord type by intervals from the root note (which does not need to be represented).
    std::uint16_t definingIntevals[] = {
        // CHORD_TYPE_Maj
        0b0000000001001000,
        // CHORD_TYPE_Maj6
        0b0000000101000000,
        // CHORD_TYPE_Maj7
        0b0000010000001000,
        // CHORD_TYPE_Maj7se
        0b0000010001101000,
        // CHORD_TYPE_Maj9
        0b0000000001001010,
        // CHORD_TYPE_Maj79
        0b0000001000001010,
        // CHORD_TYPE_Maj69
        0b0000000100001010,
        // CHORD_TYPE_aug
        0b0000000010001000,
        // CHORD_TYPE_min
        0b0000000001000100,
        // CHORD_TYPE_min6
        0b0000000101000100,
        // CHORD_TYPE_min7
        0b0000001000000100,
        // CHORD_TYPE_min7b5
        0b0000001000100100,
        // CHORD_TYPE_min9
        0b0000000001000110,
        // CHORD_TYPE_min79
        0b0000010000000110,
        // CHORD_TYPE_min7e
        0b0000000001010100,
        // CHORD_TYPE_mnMj7
        0b0000010000000100,
        // CHORD_TYPE_mnMj79
        0b0000010000000110,
        // CHORD_TYPE_dim
        0b0000000000100100,
        // CHORD_TYPE_dim7
        0b0000000100100100,
        // CHORD_TYPE_svth
        0b0000001000001000,
        // CHORD_TYPE_svsus4
        0b0000001000010000,
        // CHORD_TYPE_svb5
        0b0000001000101000,
        // CHORD_TYPE_sv9
        0b0000001000001010,
        // CHORD_TYPE_svs11
        0b0000001001101000,
        // CHORD_TYPE_sv13
        0b0000001100001000,
        // CHORD_TYPE_svb9
        0b0000001000101000,
        // CHORD_TYPE_svb13
        0b0000001011001000,
        // CHORD_TYPE_svs9
        0b0000001000001100,
        // CHORD_TYPE_Mj7aug
        0b0000010010000000,
        // CHORD_TYPE_svaug
        0b0000001000101000,
        // CHORD_TYPE_onepl8
        // TODO - Find out what this is
        0b0000000000000000,
        // CHORD_TYPE_onepl5
        // TODO - Find out what this is.
        0b0000000000000000,
        // CHORD_TYPE_sus4
        0b0000000001010000,
        // CHORD_TYPE_opl2pl5
        0b0000000000100010,
    };

    struct PitchClassAndIntervals {
        seqwires::PitchClass m_pitchClass;
        std::uint16_t m_intervals = 0;
    };

    struct ActivePitches {
        void addPitch(seqwires::Pitch pitch) {}

        void removePitch(seqwires::Pitch pitch) {}

        /// Considered inversions of the chord, i.e. also intervals where the chord root note is not the lowest pitch.
        unsigned int getNumPossibleInversions() const 
        { 
            const unsigned int pitchCount = m_pitches.size();
            // No chords are possible for fewer than 3 notes.
            return (pitchCount >= 3) ? pitchCount : 0;
        }

        /// Get the intervals defined by the active pitches, for the given inversion.
        PitchClassAndIntervals getPitchClassAndIntervals(unsigned int inversion) const {
            assert(inversion < getNumPossibleInversions());
            return {seqwires::PITCH_CLASS_C, 0};
        }

        /// The pitches of the current active set, in lowest-to-highest order.
        std::vector<seqwires::Pitch> m_pitches;
    };

    /// Try to identify a chord type which matches the intervals.
    seqwires::ChordType getChordTypeFromIntervals(std::uint16_t intervals) { return seqwires::CHORD_TYPE_Maj; }
} // namespace

seqwires::Track seqwires::chordsFromNotesFunction(const Track& sourceTrack) {
    seqwires::Track trackOut;

    ActivePitches activePitches;

    seqwires::ModelDuration timeSinceLastChordEvent = 0;
    seqwires::PitchClass currentPitchClass;
    seqwires::ChordType currentChordType = seqwires::CHORD_TYPE_NotAChord;

    for (const auto& event : iterateOver<NoteEvent>(sourceTrack)) {
        if (event.getTimeSinceLastEvent() > 0) {
            seqwires::PitchClass activePitchClass;
            seqwires::ChordType activeChordType = CHORD_TYPE_NotAChord;

            // Test all inversions in order.
            for (unsigned int i = 0; i < activePitches.getNumPossibleInversions(); ++i) {
                const PitchClassAndIntervals pitchClassAndIntervals = activePitches.getPitchClassAndIntervals(i);
                const seqwires::ChordType chordType = getChordTypeFromIntervals(pitchClassAndIntervals.m_intervals);
                if (chordType != CHORD_TYPE_NotAChord) {
                    activePitchClass = pitchClassAndIntervals.m_pitchClass;
                    activeChordType = chordType;
                    break;
                }
            }

            if ((currentPitchClass != activePitchClass) || (currentChordType != activeChordType)) {
                if (currentChordType != CHORD_TYPE_NotAChord) {
                    trackOut.addEvent(ChordOffEvent(timeSinceLastChordEvent));
                    timeSinceLastChordEvent = 0;
                }
                if (activeChordType != CHORD_TYPE_NotAChord) {
                    trackOut.addEvent(ChordOnEvent(timeSinceLastChordEvent, activePitchClass, activeChordType));
                    timeSinceLastChordEvent = 0;
                }
            }
        }

        timeSinceLastChordEvent += event.getTimeSinceLastEvent();

        if (const auto* noteOnEvent = event.as<NoteOnEvent>()) {
            activePitches.addPitch(noteOnEvent->m_pitch);
        } else if (const auto* noteOffEvent = event.as<NoteOffEvent>()) {
            activePitches.removePitch(noteOffEvent->m_pitch);
        }
    }
    if (currentChordType != CHORD_TYPE_NotAChord) {
        trackOut.addEvent(ChordOffEvent(timeSinceLastChordEvent));
    }
    return trackOut;
}

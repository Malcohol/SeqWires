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

#include <algorithm>
#include <array>

namespace {
    using IntervalSet = std::uint16_t;

    struct IntervalSetToChordType {
        IntervalSet m_intervals;
        seqwires::ChordType m_chordType;

        bool operator<(IntervalSet otherIntervals) const { return m_intervals < otherIntervals; }

        bool operator<(const IntervalSetToChordType& other) const { return m_intervals < other.m_intervals; }
    };

    /// Represent a chord type by intervals from the root note, which is in the unit position.
    // The "fingered chords" of many arranger-style keyboards are supported, although the root note is always required.
    // Not in XF spec: {0b0000000001010001} b5
    // Not in XF spec: {0b0000100001010001} M7b5
    // Not in XF spec: {0b0000100001001001} mM7b5
    const std::array<IntervalSetToChordType, 60> recognizedIntervals = {{
        // clang-format off
        // This must be sorted (the alphabetic sort of a typical editor will work to keep this sorted).
        {0b0000000000001101, seqwires::CHORD_TYPE_min9},
        {0b0000000000010101, seqwires::CHORD_TYPE_Maj9},
        {0b0000000001001001, seqwires::CHORD_TYPE_dim},
        {0b0000000010000001, seqwires::CHORD_TYPE_onep5},
        {0b0000000010000101, seqwires::CHORD_TYPE_op2p5},
        {0b0000000010001001, seqwires::CHORD_TYPE_min},
        {0b0000000010001101, seqwires::CHORD_TYPE_min9},
        {0b0000000010010001, seqwires::CHORD_TYPE_Maj},
        {0b0000000010010101, seqwires::CHORD_TYPE_Maj9},
        {0b0000000010100001, seqwires::CHORD_TYPE_sus4},
        {0b0000000010101001, seqwires::CHORD_TYPE_min7e},
        {0b0000000010101101, seqwires::CHORD_TYPE_min7e},
        {0b0000000100010001, seqwires::CHORD_TYPE_aug},
        {0b0000001000010101, seqwires::CHORD_TYPE_Maj69},
        {0b0000001001001001, seqwires::CHORD_TYPE_dim7},
        {0b0000001010000001, seqwires::CHORD_TYPE_Maj6},
        {0b0000001010001001, seqwires::CHORD_TYPE_min6},
        {0b0000001010010001, seqwires::CHORD_TYPE_Maj6},
        {0b0000001010010101, seqwires::CHORD_TYPE_Maj69},
        {0b0000010000001001, seqwires::CHORD_TYPE_min7},
        {0b0000010000001101, seqwires::CHORD_TYPE_min79},
        {0b0000010000010001, seqwires::CHORD_TYPE_svth},
        {0b0000010000010011, seqwires::CHORD_TYPE_svb9},
        {0b0000010000010101, seqwires::CHORD_TYPE_sv9},
        {0b0000010000011001, seqwires::CHORD_TYPE_svs9},
        {0b0000010000100001, seqwires::CHORD_TYPE_svsus4},
        {0b0000010001001001, seqwires::CHORD_TYPE_min7b5},
        {0b0000010001010001, seqwires::CHORD_TYPE_svb5},
        {0b0000010001010101, seqwires::CHORD_TYPE_svs11},
        {0b0000010010001001, seqwires::CHORD_TYPE_min7},
        {0b0000010010001101, seqwires::CHORD_TYPE_min79},
        {0b0000010010010001, seqwires::CHORD_TYPE_svth},
        {0b0000010010010011, seqwires::CHORD_TYPE_svb9},
        {0b0000010010010101, seqwires::CHORD_TYPE_sv9},
        {0b0000010010011001, seqwires::CHORD_TYPE_svs9},
        {0b0000010010100001, seqwires::CHORD_TYPE_svsus4},
        {0b0000010010101001, seqwires::CHORD_TYPE_min7e},
        {0b0000010010101101, seqwires::CHORD_TYPE_min7e},
        {0b0000010011010001, seqwires::CHORD_TYPE_svs11},
        {0b0000010011010101, seqwires::CHORD_TYPE_svs11},
        {0b0000010100010001, seqwires::CHORD_TYPE_svaug},
        {0b0000010110010001, seqwires::CHORD_TYPE_svb13},
        {0b0000011000010001, seqwires::CHORD_TYPE_sv13},
        {0b0000011010010001, seqwires::CHORD_TYPE_sv13},
        {0b0000100000001001, seqwires::CHORD_TYPE_mnMj7},
        {0b0000100000001101, seqwires::CHORD_TYPE_mnMj79},
        {0b0000100000010001, seqwires::CHORD_TYPE_Maj7},
        {0b0000100000010101, seqwires::CHORD_TYPE_Maj79},
        {0b0000100001010001, seqwires::CHORD_TYPE_Maj7se},
        {0b0000100001010101, seqwires::CHORD_TYPE_Maj7se},
        {0b0000100010001001, seqwires::CHORD_TYPE_mnMj7},
        {0b0000100010001101, seqwires::CHORD_TYPE_mnMj79},
        {0b0000100010010001, seqwires::CHORD_TYPE_Maj7},
        {0b0000100010010101, seqwires::CHORD_TYPE_Maj79},
        {0b0000100011010001, seqwires::CHORD_TYPE_Maj7se},
        {0b0000100011010001, seqwires::CHORD_TYPE_Maj7se},
        {0b0000100011010101, seqwires::CHORD_TYPE_Maj7se},
        {0b0000100100000001, seqwires::CHORD_TYPE_Mj7aug},
        {0b0000100100010001, seqwires::CHORD_TYPE_Mj7aug},
        {0b0001000000000001, seqwires::CHORD_TYPE_onep8},
        // clang-format on
    }};
    
    /// Try to identify a chord type which matches the interval.
    seqwires::ChordType getMatchingChordTypeFromIntervals(IntervalSet intervals) {
        // Sortedness is asserted at the beginning of chordsFromNotesFunction.
        // For such a small array size, I guessed that binary search would be a good approach,
        // but I didn't do any timings.
        const auto it = std::lower_bound(recognizedIntervals.begin(), recognizedIntervals.end(), intervals);
        if ((it != recognizedIntervals.end()) && (it->m_intervals == intervals)) {
            return it->m_chordType;
        }
        return seqwires::CHORD_TYPE_NotAChord;
    }

    /// The pitches of the set of currently playing notes.
    struct ActivePitches {
        void addPitch(seqwires::Pitch pitch) {
            const auto it = std::upper_bound(m_pitches.begin(), m_pitches.end(), pitch);
            assert(((it == m_pitches.end()) || (*it > pitch)) && "NoteOnEvent for same pitch as currently playing note");
            m_pitches.insert(it, pitch);
        }

        void removePitch(seqwires::Pitch pitch) {
            const auto it = std::find(m_pitches.begin(), m_pitches.end(), pitch);
            assert((it != m_pitches.end()) && "NoteOffEvent without matching NoteOnEvent");
            m_pitches.erase(it);
        }

        /// Check whether the currently active pitches match an known IntervalSet or inversion of that IntervalSet.
        seqwires::Chord getBestMatchChord() const {
            const unsigned int numPitches = m_pitches.size();
            // TODO Assert min and max match the recognized chords.
            constexpr unsigned int minNumPitches = 2;
            constexpr unsigned int maxNumPitches = 6;
            if ((numPitches < minNumPitches) || (numPitches > maxNumPitches)) {
                return seqwires::Chord();
            }
            // The intervals between neighbouring pitches as integers.
            unsigned int pitchDiffs[maxNumPitches - 1] = { 0 };

            IntervalSet interval = 1;
            {
                unsigned int shift = 0;
                for (unsigned int i = 1; i < numPitches; ++i) {
                    // Bring neighbouring intervals within the octave.
                    pitchDiffs[i - 1] = (m_pitches[i] - m_pitches[i - 1]) % 12;
                    shift += pitchDiffs[i - 1];
                    interval |= IntervalSet(1) << shift;
                }
            }

            // Try each inversion.
            for (unsigned int i = 0; i < numPitches; ++i) {
                const seqwires::ChordType chordType = getMatchingChordTypeFromIntervals(interval);
                if (chordType != seqwires::CHORD_TYPE_NotAChord) {
                    return seqwires::Chord{seqwires::pitchToPitchClass(m_pitches[i]), chordType};
                }
                if (i < numPitches - 1) {
                    // Add the old root raised by an octave.
                    interval = (interval + (1 << 12)) >> pitchDiffs[i];
                }
            }
            return seqwires::Chord();
        }

        /// The pitches of the current active set, in lowest-to-highest order.
        std::vector<seqwires::Pitch> m_pitches;
    };
} // namespace

seqwires::Track seqwires::chordsFromNotesFunction(const Track& sourceTrack) {
    // Required for getMatchingChordTypeFromIntervals
    assert(std::is_sorted(recognizedIntervals.begin(), recognizedIntervals.end()));

    seqwires::Track trackOut;

    ActivePitches activePitches;
    seqwires::ModelDuration timeSinceLastChordEvent = 0;
    Chord currentChord;

    for (const auto& event : iterateOver<NoteEvent>(sourceTrack)) {
        if (event.getTimeSinceLastEvent() > 0) {
            const Chord bestChord = activePitches.getBestMatchChord();
            if (currentChord != bestChord) {
                if (currentChord.m_chordType != CHORD_TYPE_NotAChord) {
                    trackOut.addEvent(ChordOffEvent(timeSinceLastChordEvent));
                    currentChord.m_chordType = CHORD_TYPE_NotAChord;
                    timeSinceLastChordEvent = 0;
                }
                if (bestChord.m_chordType != CHORD_TYPE_NotAChord) {
                    trackOut.addEvent(
                        ChordOnEvent(timeSinceLastChordEvent, bestChord));
                    currentChord = bestChord;
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
    if (currentChord.m_chordType != CHORD_TYPE_NotAChord) {
        trackOut.addEvent(ChordOffEvent(timeSinceLastChordEvent));
    }
    trackOut.setDuration(sourceTrack.getDuration());
    return trackOut;
}

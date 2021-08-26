#pragma once

#include <SeqWiresLib/musicTypes.hpp>

#include <vector>

namespace seqwires {
    class Track;
}

namespace testUtils {
    /// Add notes of the given pitches to the track. Each has quaternote duration.
    void addSimpleNotes(const std::vector<seqwires::Pitch>& expectedPitches, seqwires::Track& track);

    /// Check that there is a sequence of the given pitches in the track, each with quaternote duration.
    void testSimpleNotes(const std::vector<seqwires::Pitch>& expectedPitches, const seqwires::Track& track);

    struct NoteInfo {
        seqwires::Pitch m_pitch;
        seqwires::ModelDuration m_noteOnTime = 0;
        seqwires::ModelDuration m_noteOffTime = babelwires::Rational(1, 4);;
    };

    /// Add notes as described to the track. Each has quaternote duration.
    void addNotes(const std::vector<NoteInfo>& notes, seqwires::Track& track);

    /// Check that there is a sequence of the given notes in the track.
    void testNotes(const std::vector<NoteInfo>& expectedNotes, const seqwires::Track& track);

    struct ChordInfo {
        seqwires::PitchClass m_root;
        seqwires::ChordType m_chordType;
        seqwires::ModelDuration m_chordOffTime = babelwires::Rational(1, 2);
        seqwires::ModelDuration m_chordOnTime = 0;
    };

    /// Add the given chords. Each has halfnote duration.
    void addChords(const std::vector<ChordInfo>& chords, seqwires::Track& track);

    void testChords(const std::vector<ChordInfo>& expectedChords, const seqwires::Track& track);
}

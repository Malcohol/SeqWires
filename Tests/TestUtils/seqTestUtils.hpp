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
        seqwires::ModelDuration m_noteOnTime;
        seqwires::ModelDuration m_noteOffTime;
    };

    /// Add notes as described to the track. Each has quaternote duration.
    void addNotes(const std::vector<NoteInfo>& notes, seqwires::Track& track);

    /// Check that there is a sequence of the given notes in the track.
    void testNotes(const std::vector<NoteInfo>& expectedNotes, const seqwires::Track& track);
}

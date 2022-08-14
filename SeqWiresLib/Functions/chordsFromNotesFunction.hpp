/**
 * Function which creates tracks of chord events from tracks of note events.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <SeqWiresLib/Tracks/track.hpp>

namespace seqwires {
#define CHORD_FROM_NOTES_SUSTAIN_POLICY(X)                                                                             \
    X(Notes, "Same as notes", "bee38885-c5d2-4742-9a46-26ad2318bb55")                                                  \
    X(Hold, "Hold until next chord", "fc31d04e-d77e-4436-9321-899a374fedff")

    /// The enum that determines the how chords are sustained.
    /// The "Notes" policy means that the duration of chords is defined exactly by the note events.
    /// The "Hold" policy means that chords are sustained until the next recognized chord, or until 
    /// a "Cancel Chord" event is observed (three consecutive pitches).
    class ChordFromNotesSustainPolicyEnum : public babelwires::Enum {
      public:
        ChordFromNotesSustainPolicyEnum();

        static babelwires::LongIdentifier getThisIdentifier();

        ENUM_DEFINE_CPP_ENUM(CHORD_FROM_NOTES_SUSTAIN_POLICY);
    };

    /// Create tracks of chord events from tracks of note events.
    Track chordsFromNotesFunction(const Track& sourceTrack, ChordFromNotesSustainPolicyEnum::Value sustainPolicy);
} // namespace seqwires

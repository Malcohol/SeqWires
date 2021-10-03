#include <gtest/gtest.h>

#include <SeqWiresLib/Functions/chordsFromNotesFunction.hpp>
#include <SeqWiresLib/Tracks/noteEvents.hpp>

#include <Tests/TestUtils/seqTestUtils.hpp>

TEST(ChordsFromNotesTest, functionBasic) {
    seqwires::Track track;
    track.addEvent(seqwires::NoteOnEvent(0, 60));
    track.addEvent(seqwires::NoteOnEvent(0, 64));
    track.addEvent(seqwires::NoteOnEvent(0, 67));

    track.addEvent(seqwires::NoteOffEvent(1, 60));
    track.addEvent(seqwires::NoteOffEvent(0, 64));
    track.addEvent(seqwires::NoteOffEvent(0, 67));
    
    track.addEvent(seqwires::NoteOnEvent(0, 62));
    track.addEvent(seqwires::NoteOnEvent(0, 65));
    track.addEvent(seqwires::NoteOnEvent(0, 69));

    track.addEvent(seqwires::NoteOffEvent(1, 62));
    track.addEvent(seqwires::NoteOffEvent(0, 65));
    track.addEvent(seqwires::NoteOffEvent(0, 69));

    seqwires::Track chordTrack = seqwires::chordsFromNotesFunction(track);
    EXPECT_EQ(chordTrack.getDuration(), 2);

    std::vector<testUtils::ChordInfo> expectedChords = {
      { seqwires::PITCH_CLASS_C, seqwires::CHORD_TYPE_Maj, 1 },
      { seqwires::PITCH_CLASS_D, seqwires::CHORD_TYPE_min, 1 }
    };

    testUtils::testChords(expectedChords, chordTrack);
}

TEST(ChordsFromNotesTest, functionChordToChord) {
    seqwires::Track track;
    track.addEvent(seqwires::NoteOnEvent(0, 60));
    track.addEvent(seqwires::NoteOnEvent(0, 64));
    track.addEvent(seqwires::NoteOnEvent(0, 67));

    track.addEvent(seqwires::NoteOffEvent(1, 64));
    
    track.addEvent(seqwires::NoteOnEvent(0, 63));

    track.addEvent(seqwires::NoteOffEvent(1, 60));
    track.addEvent(seqwires::NoteOffEvent(0, 63));
    track.addEvent(seqwires::NoteOffEvent(0, 67));

    seqwires::Track chordTrack = seqwires::chordsFromNotesFunction(track);
    EXPECT_EQ(chordTrack.getDuration(), 2);

    std::vector<testUtils::ChordInfo> expectedChords = {
      { seqwires::PITCH_CLASS_C, seqwires::CHORD_TYPE_Maj, 1 },
      { seqwires::PITCH_CLASS_C, seqwires::CHORD_TYPE_min, 1 }
    };

    testUtils::testChords(expectedChords, chordTrack);
}

// Only major and minor inversions are guaranteed to be recognized.
TEST(ChordsFromNotesTest, majorInversions) {
    seqwires::Track track;
    track.addEvent(seqwires::NoteOnEvent(0, 60));
    track.addEvent(seqwires::NoteOnEvent(0, 64));
    track.addEvent(seqwires::NoteOnEvent(0, 67));

    track.addEvent(seqwires::NoteOffEvent(1, 60));
    track.addEvent(seqwires::NoteOffEvent(0, 64));
    track.addEvent(seqwires::NoteOffEvent(0, 67));

    track.addEvent(seqwires::NoteOnEvent(1, 64));
    track.addEvent(seqwires::NoteOnEvent(0, 67));
    track.addEvent(seqwires::NoteOnEvent(0, 72));

    track.addEvent(seqwires::NoteOffEvent(1, 64));
    track.addEvent(seqwires::NoteOffEvent(0, 67));
    track.addEvent(seqwires::NoteOffEvent(0, 72));

    track.addEvent(seqwires::NoteOnEvent(1, 67));
    track.addEvent(seqwires::NoteOnEvent(0, 72));
    track.addEvent(seqwires::NoteOnEvent(0, 76));

    track.addEvent(seqwires::NoteOffEvent(1, 67));
    track.addEvent(seqwires::NoteOffEvent(0, 72));
    track.addEvent(seqwires::NoteOffEvent(0, 76));

    seqwires::Track chordTrack = seqwires::chordsFromNotesFunction(track);
    EXPECT_EQ(chordTrack.getDuration(), 5);

    std::vector<testUtils::ChordInfo> expectedChords = {
      { seqwires::PITCH_CLASS_C, seqwires::CHORD_TYPE_Maj, 1, 0 },
      { seqwires::PITCH_CLASS_C, seqwires::CHORD_TYPE_Maj, 1, 1 },
      { seqwires::PITCH_CLASS_C, seqwires::CHORD_TYPE_Maj, 1, 1 }
    };

    testUtils::testChords(expectedChords, chordTrack);
}

// Only major and minor inversions are guaranteed to be recognized.
TEST(ChordsFromNotesTest, minorInversions) {
    seqwires::Track track;
    track.addEvent(seqwires::NoteOnEvent(0, 60));
    track.addEvent(seqwires::NoteOnEvent(0, 63));
    track.addEvent(seqwires::NoteOnEvent(0, 67));

    track.addEvent(seqwires::NoteOffEvent(1, 60));
    track.addEvent(seqwires::NoteOffEvent(0, 63));
    track.addEvent(seqwires::NoteOffEvent(0, 67));

    track.addEvent(seqwires::NoteOnEvent(1, 63));
    track.addEvent(seqwires::NoteOnEvent(0, 67));
    track.addEvent(seqwires::NoteOnEvent(0, 72));

    track.addEvent(seqwires::NoteOffEvent(1, 63));
    track.addEvent(seqwires::NoteOffEvent(0, 67));
    track.addEvent(seqwires::NoteOffEvent(0, 72));

    track.addEvent(seqwires::NoteOnEvent(1, 67));
    track.addEvent(seqwires::NoteOnEvent(0, 72));
    track.addEvent(seqwires::NoteOnEvent(0, 75));

    track.addEvent(seqwires::NoteOffEvent(1, 67));
    track.addEvent(seqwires::NoteOffEvent(0, 72));
    track.addEvent(seqwires::NoteOffEvent(0, 75));

    seqwires::Track chordTrack = seqwires::chordsFromNotesFunction(track);
    EXPECT_EQ(chordTrack.getDuration(), 5);

    std::vector<testUtils::ChordInfo> expectedChords = {
      { seqwires::PITCH_CLASS_C, seqwires::CHORD_TYPE_min, 1, 0 },
      { seqwires::PITCH_CLASS_C, seqwires::CHORD_TYPE_min, 1, 1 },
      { seqwires::PITCH_CLASS_C, seqwires::CHORD_TYPE_min, 1, 1 }
    };

    testUtils::testChords(expectedChords, chordTrack);
}

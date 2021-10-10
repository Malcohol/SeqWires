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

    std::vector<testUtils::ChordInfo> expectedChords = {{seqwires::PITCH_CLASS_C, seqwires::CHORD_TYPE_Maj, 1},
                                                        {seqwires::PITCH_CLASS_D, seqwires::CHORD_TYPE_min, 1}};

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

    std::vector<testUtils::ChordInfo> expectedChords = {{seqwires::PITCH_CLASS_C, seqwires::CHORD_TYPE_Maj, 1},
                                                        {seqwires::PITCH_CLASS_C, seqwires::CHORD_TYPE_min, 1}};

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

    std::vector<testUtils::ChordInfo> expectedChords = {{seqwires::PITCH_CLASS_C, seqwires::CHORD_TYPE_Maj, 1, 0},
                                                        {seqwires::PITCH_CLASS_C, seqwires::CHORD_TYPE_Maj, 1, 1},
                                                        {seqwires::PITCH_CLASS_C, seqwires::CHORD_TYPE_Maj, 1, 1}};

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

    std::vector<testUtils::ChordInfo> expectedChords = {{seqwires::PITCH_CLASS_C, seqwires::CHORD_TYPE_min, 1, 0},
                                                        {seqwires::PITCH_CLASS_C, seqwires::CHORD_TYPE_min, 1, 1},
                                                        {seqwires::PITCH_CLASS_C, seqwires::CHORD_TYPE_min, 1, 1}};

    testUtils::testChords(expectedChords, chordTrack);
}

// Yamaha-style fingered chords.
TEST(ChordsFromNotesTest, schemeY) {
    using namespace seqwires;

    std::vector<std::vector<seqwires::Pitch>> pitches = {
    // C1+8
    { 60, 72 },
    // C1+5
    { 60, 67 },
    // C
    { 60, 64, 67 },
    // C9
    { 60, 62, 64, 67 },
    // C6
    { 60, 67, 69 },
    // C6
    { 60, 64, 67, 69 },
    // C69
    { 60, 62, 64, 69 },
    // C69
    { 60, 62, 64, 67, 69 },
    // CM7
    { 60, 64, 71 },
    // CM7
    { 60, 64, 67, 71 },
    // CM7(9)
    { 60, 62, 64, 71 },
    // CM7(9)
    { 60, 62, 64, 67, 71 },
    // CM7(#11)
    { 60, 64, 66, 67, 71 },
    // CM7(#11)
    { 60, 62, 64, 66, 67, 71 },
/* TODO
    // C(b5)
    { 60, 64, 66 },
    // CM7(b5)
    { 60, 64, 66, 71 },
*/
    // Csus4
    { 60, 65, 67 },
    // Caug
    { 60, 64, 68 },
    // CM7aug
    { 60, 68, 71 },
    // CM7aug
    { 60, 64, 68, 71 },
    // Cm
    { 60, 63, 67 },
    // Cm(9)
    { 60, 62, 63, 67 },
    // Cm6
    { 60, 63, 67, 69 },
    // Cm7
    { 60, 63, 70 },
    // Cm7
    { 60, 63, 67, 70 },
    // Cm7(9)
    { 60, 62, 63, 70 },
    // Cm7(9)
    { 60, 62, 63, 67, 70 },
    // Cm7(11)
    { 60, 63, 65, 67 },
    // Cm7(11)
    { 60, 62, 63, 65, 67 },
    // Cm7(11)
    { 60, 63, 65, 67, 70 },
    // Cm7(11)
    { 60, 62, 63, 65, 67, 70 },
    // CmM7
    { 60, 63, 71 },
    // CmM7
    { 60, 63, 67, 71 },
    // CmM7(9)
    { 60, 62, 63, 71 },
    // CmM7(9)
    { 60, 62, 63, 67, 71 },
    // Cm7b5
    { 60, 63, 66, 70 },
/* TODO
    // CmM7b5
    { 60, 63, 66, 71 },
*/
    // Cdim
    { 60, 63, 66 },
    // Cdim7
    { 60, 63, 66, 69 },
    // C7
    { 60, 64, 70 },
    // C7
    { 60, 64, 67, 70 },
    // C7(b9)
    { 60, 61, 64, 70 },
    // C7(b9)
    { 60, 61, 64, 67, 70 },
    // C7(b13)
    { 60, 64, 67, 68, 70 },
    // C7(9)
    { 60, 62, 64, 70 },
    // C7(9)
    { 60, 62, 64, 67, 70 },
    // C7(#11)
    { 60, 64, 66, 67, 70 },
    // C7(#11)
    { 60, 62, 64, 66, 67, 70 },
    // C7(13)
    { 60, 64, 69, 70 },
    // C7(13)
    { 60, 64, 67, 69, 70 },
    // C7(#9)
    { 60, 63, 64, 70 },
    // C7(#9)
    { 60, 63, 64, 67, 70 },
    // C7b5
    { 60, 64, 66, 70 },
    // C7aug
    { 60, 64, 68, 70 },
    // C7sus4
    { 60, 65, 70 },
    // C7sus4
    { 60, 65, 67, 70 },
    // C1+2+5
    { 60, 62, 67 }};

    seqwires::Track track;
    for (const auto& v : pitches) {
        babelwires::Rational time = 1;
        for (auto p : v) {
            track.addEvent(NoteOnEvent{ time, p });
            time = 0;
        }
        time = 1;
        for (auto p : v) {
            track.addEvent(NoteOffEvent{ time, p });
            time = 0;
        }
    }

    seqwires::Track chordTrack = seqwires::chordsFromNotesFunction(track);

    std::vector<ChordType> expectedChordTypes = {
        CHORD_TYPE_onep8,
        CHORD_TYPE_onep5,
        CHORD_TYPE_Maj,
        CHORD_TYPE_Maj9,
        CHORD_TYPE_Maj6,
        CHORD_TYPE_Maj6,
        CHORD_TYPE_Maj69,
        CHORD_TYPE_Maj69,
        CHORD_TYPE_Maj7,
        CHORD_TYPE_Maj7,
        CHORD_TYPE_Maj79,
        CHORD_TYPE_Maj79,
        CHORD_TYPE_Maj7se,
        CHORD_TYPE_Maj7se,
        // C(b5)
        // CM7(b5)
        CHORD_TYPE_sus4,
        CHORD_TYPE_aug,
        CHORD_TYPE_Mj7aug,
        CHORD_TYPE_Mj7aug,
        CHORD_TYPE_min,
        CHORD_TYPE_min9,
        CHORD_TYPE_min6,
        CHORD_TYPE_min7,
        CHORD_TYPE_min7,
        CHORD_TYPE_min79,
        CHORD_TYPE_min79,
        CHORD_TYPE_min7e,
        CHORD_TYPE_min7e,
        CHORD_TYPE_min7e,
        CHORD_TYPE_min7e,
        CHORD_TYPE_mnMj7,
        CHORD_TYPE_mnMj7,
        CHORD_TYPE_mnMj79,
        CHORD_TYPE_mnMj79,
        CHORD_TYPE_min7b5,
        // mM7b5,
        CHORD_TYPE_dim,
        CHORD_TYPE_dim7,
        CHORD_TYPE_svth,
        CHORD_TYPE_svth,
        CHORD_TYPE_svb9,
        CHORD_TYPE_svb9,
        CHORD_TYPE_svb13,
        CHORD_TYPE_sv9,
        CHORD_TYPE_sv9,
        CHORD_TYPE_svs11,
        CHORD_TYPE_svs11,
        CHORD_TYPE_sv13,
        CHORD_TYPE_sv13,
        CHORD_TYPE_svs9,
        CHORD_TYPE_svs9,
        CHORD_TYPE_svb5,
        CHORD_TYPE_svaug,
        CHORD_TYPE_svsus4,
        CHORD_TYPE_svsus4,
        CHORD_TYPE_op2p5
    };

    EXPECT_EQ(pitches.size(), expectedChordTypes.size());

    std::vector<testUtils::ChordInfo> expectedChords;
    for (auto c : expectedChordTypes) {
        expectedChords.emplace_back(testUtils::ChordInfo{PITCH_CLASS_C, c, 1, 1});
    }

    testUtils::testChords(expectedChords, chordTrack);
}

// Roland-style fingered chords.
TEST(ChordsFromNotesTest, schemeR) {
    using namespace seqwires;

    std::vector<std::vector<seqwires::Pitch>> pitches = {
    // C
    { 60, 64, 67 },
    // C6
    { 60, 67, 69 },
    // C6
    { 60, 64, 67, 69 },
    // CM7
    { 60, 64, 71 },
    // CM7
    { 60, 64, 67, 71 },
    // CM7(#11)
    { 60, 64, 66, 71 },
    // CM7(#11)
    { 60, 62, 64, 66, 71 },
    // CM7(#11)
    { 60, 64, 66, 67, 71 },
    // CM7(#11)
    { 60, 62, 64, 66, 67, 71 },
    // CM(9)
    { 60, 62, 64, 67 },
    // CM7(9)
    { 60, 62, 64, 71 },
    // CM7(9)
    { 60, 62, 64, 67, 71 },
    // C6(9)
    { 60, 62, 64, 69 },
    // C6(9)
    { 60, 62, 64, 67, 69 },
    // Caug
    { 60, 64, 68 },
    // Cm
    { 60, 63, 67 },
    // Cm6
    { 60, 63, 67, 69 },
    // Cm7
    { 60, 63, 70 },
    // Cm7
    { 60, 63, 67, 70 },
    // Cm7(b5)
    { 60, 63, 66, 70 },
    // Cm(9)
    { 60, 62, 63, 67 },
    // Cm7(9)
    { 60, 62, 63, 70 },
    // Cm7(9)
    { 60, 62, 63, 67, 70 },
    // Cm7(11)
    { 60, 63, 65, 67 },
    // Cm7(11)
    { 60, 62, 63, 65, 67 },
    // Cm7(11)
    { 60, 63, 65, 67, 70 },
    // Cm7(11)
    { 60, 62, 63, 65, 67, 70 },
    // CmM7
    { 60, 63, 71 },
    // CmM7
    { 60, 63, 67, 71 },
    // CmM7(9)
    { 60, 62, 63, 71 },
    // CmM7(9)
    { 60, 62, 63, 67, 71 },
    // Cdim
    { 60, 63, 66 },
    // Cdim7
    { 60, 63, 66, 69 },
    // C7
    { 60, 64, 70 },
    // C7
    { 60, 64, 67, 70 },
    // C7sus4
    { 60, 65, 67, 70 },
    // C7(b5)
    { 60, 64, 66, 70 },
    // C7(9)
    { 60, 62, 64, 70 },
    // C7(9)
    { 60, 62, 64, 67, 70 },
    // C7(#11)
    { 60, 62, 64, 66, 70 },
    // C7(#11)
    { 60, 62, 64, 66, 67, 70 },
    // C7(13)
    { 60, 64, 69, 70 },
    // C7(13)
    { 60, 64, 67, 69, 70 },
    // C7(b9)
    { 60, 61, 64, 70 },
    // C7(b9)
    { 60, 61, 64, 67, 70 },
    // C7(b13)
    { 60, 64, 67, 68, 70 },
    // C7(#9)
    // No root note
    // { 63, 64, 70 },
    // C7(#9)
    { 60, 63, 64, 70 },
    // C7(#9)
    // No root note
    //{ 63, 64, 67, 70 },
    // C7(#9)
    { 60, 63, 64, 67, 70 },
    // CM7aug
    { 60, 64, 68, 71 },
    // C7aug
    // No root note
    //{ 64, 68, 70 },
    // C7aug
    { 60, 64, 68, 70 },
    // Csus4
    { 60, 65, 67 },
    // C1+2+5
    { 60, 62, 67 }};

    seqwires::Track track;
    for (const auto& v : pitches) {
        babelwires::Rational time = 1;
        for (auto p : v) {
            track.addEvent(NoteOnEvent{ time, p });
            time = 0;
        }
        time = 1;
        for (auto p : v) {
            track.addEvent(NoteOffEvent{ time, p });
            time = 0;
        }
    }

    seqwires::Track chordTrack = seqwires::chordsFromNotesFunction(track);

    std::vector<ChordType> expectedChordTypes = {
        CHORD_TYPE_Maj,
        CHORD_TYPE_Maj6,
        CHORD_TYPE_Maj6,
        CHORD_TYPE_Maj7,
        CHORD_TYPE_Maj7,
        CHORD_TYPE_Maj7se,
        CHORD_TYPE_Maj7se,
        CHORD_TYPE_Maj7se,
        CHORD_TYPE_Maj7se,
        CHORD_TYPE_Maj9,
        CHORD_TYPE_Maj79,
        CHORD_TYPE_Maj79,
        CHORD_TYPE_Maj69,
        CHORD_TYPE_Maj69,
        CHORD_TYPE_aug,
        CHORD_TYPE_min,
        CHORD_TYPE_min6,
        CHORD_TYPE_min7,
        CHORD_TYPE_min7,
        CHORD_TYPE_min7b5,
        CHORD_TYPE_min9,
        CHORD_TYPE_min79,
        CHORD_TYPE_min79,
        CHORD_TYPE_min7e,
        CHORD_TYPE_min7e,
        CHORD_TYPE_min7e,
        CHORD_TYPE_min7e,
        CHORD_TYPE_mnMj7,
        CHORD_TYPE_mnMj7,
        CHORD_TYPE_mnMj79,
        CHORD_TYPE_mnMj79,
        CHORD_TYPE_dim,
        CHORD_TYPE_dim7,
        CHORD_TYPE_svth,
        CHORD_TYPE_svth,
        CHORD_TYPE_svsus4,
        CHORD_TYPE_svb5,
        CHORD_TYPE_sv9,
        CHORD_TYPE_sv9,
        CHORD_TYPE_svs11,
        CHORD_TYPE_svs11,
        CHORD_TYPE_sv13,
        CHORD_TYPE_sv13,
        CHORD_TYPE_svb9,
        CHORD_TYPE_svb9,
        CHORD_TYPE_svb13,
        CHORD_TYPE_svs9,
        // No root note,
        //CHORD_TYPE_svs9,
        CHORD_TYPE_svs9,
        // No root note,
        //CHORD_TYPE_svs9,
        CHORD_TYPE_Mj7aug,
        CHORD_TYPE_svaug,
        // CHORD_TYPE_No root note,
        //CHORD_TYPE_svaug,
        CHORD_TYPE_sus4,
        CHORD_TYPE_op2p5,
    };

    EXPECT_EQ(pitches.size(), expectedChordTypes.size());

    std::vector<testUtils::ChordInfo> expectedChords;
    for (auto c : expectedChordTypes) {
        expectedChords.emplace_back(testUtils::ChordInfo{PITCH_CLASS_C, c, 1, 1});
    }

    testUtils::testChords(expectedChords, chordTrack);
}

// Casio-style fingered chords.
TEST(ChordsFromNotesTest, schemeC) {
    using namespace seqwires;

    std::vector<std::vector<seqwires::Pitch>> pitches = {
        // C
        { 60, 64, 67 },
        // Cm
        { 60, 63, 67 },
        // Cdim
        { 60, 63, 66 },
        // Caug
        { 60, 64, 68 },
        // Csus4
        { 60, 65, 67 },
        // C7
        { 60, 64, 70 },
        // C7
        { 60, 64, 67, 70 },
        // Cm7
        { 60, 63, 70 },
        // Cm7
        { 60, 63, 67, 70 },
        // Cmaj7
        { 60, 64, 71 },
        // Cmaj7
        { 60, 64, 67, 71 },
        // Cm7b5
        { 60, 63, 66, 70 },
        // C7b5
        { 60, 64, 66, 70 },
        // C7sus4
        { 60, 65, 67, 70 },
        // Cadd9
        { 60, 62, 64 },
        // Cadd9
        { 60, 62, 64, 67 },
        // Cmadd9
        { 60, 62, 63 },
        // Cmadd9
        { 60, 62, 63, 67 },
        // CmM7
        { 60, 63, 71 },
        // CmM7
        { 60, 63, 67, 71 },
        // Cdim7
        { 60, 63, 66, 69 }};

    seqwires::Track track;
    for (const auto& v : pitches) {
        babelwires::Rational time = 1;
        for (auto p : v) {
            track.addEvent(NoteOnEvent{ time, p });
            time = 0;
        }
        time = 1;
        for (auto p : v) {
            track.addEvent(NoteOffEvent{ time, p });
            time = 0;
        }
    }

    seqwires::Track chordTrack = seqwires::chordsFromNotesFunction(track);

    std::vector<ChordType> expectedChordTypes = {
        CHORD_TYPE_Maj,
        CHORD_TYPE_min,
        CHORD_TYPE_dim,
        CHORD_TYPE_aug,
        CHORD_TYPE_sus4,
        CHORD_TYPE_svth,
        CHORD_TYPE_svth,
        CHORD_TYPE_min7,
        CHORD_TYPE_min7,
        CHORD_TYPE_Maj7,
        CHORD_TYPE_Maj7,
        CHORD_TYPE_min7b5,
        CHORD_TYPE_svb5,
        CHORD_TYPE_svsus4,
        CHORD_TYPE_Maj9,
        CHORD_TYPE_Maj9,
        CHORD_TYPE_min9,
        CHORD_TYPE_min9,
        CHORD_TYPE_mnMj7,
        CHORD_TYPE_mnMj7,
        CHORD_TYPE_dim7
    };

    EXPECT_EQ(pitches.size(), expectedChordTypes.size());

    std::vector<testUtils::ChordInfo> expectedChords;
    for (auto c : expectedChordTypes) {
        expectedChords.emplace_back(testUtils::ChordInfo{PITCH_CLASS_C, c, 1, 1});
    }

    testUtils::testChords(expectedChords, chordTrack);
}


#include <gtest/gtest.h>

#include <BabelWiresLib/ValueTree/valueTreeRoot.hpp>

#include <SeqWiresLib/Functions/fingeredChordsFunction.hpp>
#include <SeqWiresLib/Processors/fingeredChordsProcessor.hpp>
#include <SeqWiresLib/Types/Track/TrackEvents/noteEvents.hpp>
#include <SeqWiresLib/libRegistration.hpp>

#include <Tests/BabelWiresLib/TestUtils/testEnvironment.hpp>
#include <Tests/TestUtils/seqTestUtils.hpp>

TEST(FingeredChordsTest, functionBasicNotesPolicy) {
    seqwires::Track track;
    track.addEvent(seqwires::NoteOnEvent(0, 60));
    track.addEvent(seqwires::NoteOnEvent(0, 64));
    track.addEvent(seqwires::NoteOnEvent(0, 67));

    track.addEvent(seqwires::NoteOffEvent(1, 60));
    track.addEvent(seqwires::NoteOffEvent(0, 64));
    track.addEvent(seqwires::NoteOffEvent(0, 67));

    track.addEvent(seqwires::NoteOnEvent(1, 62));
    track.addEvent(seqwires::NoteOnEvent(0, 65));
    track.addEvent(seqwires::NoteOnEvent(0, 69));

    track.addEvent(seqwires::NoteOffEvent(1, 62));
    track.addEvent(seqwires::NoteOffEvent(0, 65));
    track.addEvent(seqwires::NoteOffEvent(0, 69));

    // Cancel chord - which is ignored in the "Notes" policy
    track.addEvent(seqwires::NoteOnEvent(1, 61));
    track.addEvent(seqwires::NoteOnEvent(0, 62));
    track.addEvent(seqwires::NoteOnEvent(0, 63));

    track.addEvent(seqwires::NoteOffEvent(1, 61));
    track.addEvent(seqwires::NoteOffEvent(0, 62));
    track.addEvent(seqwires::NoteOffEvent(0, 63));

    seqwires::Track chordTrack =
        seqwires::fingeredChordsFunction(track, seqwires::FingeredChordsSustainPolicyEnum::Value::Notes);
    EXPECT_EQ(chordTrack.getDuration(), 5);

    std::vector<testUtils::ChordInfo> expectedChords = {
        {seqwires::PitchClass::Value::C, seqwires::ChordType::Value::M, 1},
        {seqwires::PitchClass::Value::D, seqwires::ChordType::Value::m, 1, 1}};

    testUtils::testChords(expectedChords, chordTrack);
}

TEST(FingeredChordsTest, functionBasicHoldPolicy) {
    seqwires::Track track;
    track.addEvent(seqwires::NoteOnEvent(0, 60));
    track.addEvent(seqwires::NoteOnEvent(0, 64));
    track.addEvent(seqwires::NoteOnEvent(0, 67));

    track.addEvent(seqwires::NoteOffEvent(1, 60));
    track.addEvent(seqwires::NoteOffEvent(0, 64));
    track.addEvent(seqwires::NoteOffEvent(0, 67));

    track.addEvent(seqwires::NoteOnEvent(1, 62));
    track.addEvent(seqwires::NoteOnEvent(0, 65));
    track.addEvent(seqwires::NoteOnEvent(0, 69));

    track.addEvent(seqwires::NoteOffEvent(1, 62));
    track.addEvent(seqwires::NoteOffEvent(0, 65));
    track.addEvent(seqwires::NoteOffEvent(0, 69));

    // Cancel chord - which is used in the "Hold" policy
    track.addEvent(seqwires::NoteOnEvent(1, 61));
    track.addEvent(seqwires::NoteOnEvent(0, 62));
    track.addEvent(seqwires::NoteOnEvent(0, 63));

    track.addEvent(seqwires::NoteOffEvent(1, 61));
    track.addEvent(seqwires::NoteOffEvent(0, 62));
    track.addEvent(seqwires::NoteOffEvent(0, 63));

    seqwires::Track chordTrack =
        seqwires::fingeredChordsFunction(track, seqwires::FingeredChordsSustainPolicyEnum::Value::Hold);
    EXPECT_EQ(chordTrack.getDuration(), 5);

    std::vector<testUtils::ChordInfo> expectedChords = {
        {seqwires::PitchClass::Value::C, seqwires::ChordType::Value::M, 2},
        {seqwires::PitchClass::Value::D, seqwires::ChordType::Value::m, 2}};

    testUtils::testChords(expectedChords, chordTrack);
}

TEST(FingeredChordsTest, rootPitchClass) {
    seqwires::Track track;

    for (int i = 0; i < 12 * 10; ++i) {
        track.addEvent(seqwires::NoteOnEvent(1, i));
        track.addEvent(seqwires::NoteOnEvent(0, i + 4));
        track.addEvent(seqwires::NoteOnEvent(0, i + 7));
        track.addEvent(seqwires::NoteOffEvent(1, i));
        track.addEvent(seqwires::NoteOffEvent(0, i + 4));
        track.addEvent(seqwires::NoteOffEvent(0, i + 7));
    }

    seqwires::Track chordTrack =
        seqwires::fingeredChordsFunction(track, seqwires::FingeredChordsSustainPolicyEnum::Value::Notes);

    std::vector<testUtils::ChordInfo> expectedChords;
    for (int o = 0; o < 10; ++o) {
        for (int pc = 0; pc < 12; ++pc) {
            expectedChords.emplace_back(testUtils::ChordInfo{static_cast<seqwires::PitchClass::Value>(pc),
                                                             seqwires::ChordType::Value::M, 1, 1});
        }
    }

    testUtils::testChords(expectedChords, chordTrack);
}

TEST(FingeredChordsTest, functionChordToChord) {
    seqwires::Track track;
    track.addEvent(seqwires::NoteOnEvent(0, 60));
    track.addEvent(seqwires::NoteOnEvent(0, 64));
    track.addEvent(seqwires::NoteOnEvent(0, 67));

    track.addEvent(seqwires::NoteOffEvent(1, 64));

    track.addEvent(seqwires::NoteOnEvent(0, 63));

    track.addEvent(seqwires::NoteOffEvent(1, 60));
    track.addEvent(seqwires::NoteOffEvent(0, 63));
    track.addEvent(seqwires::NoteOffEvent(0, 67));

    seqwires::Track chordTrack =
        seqwires::fingeredChordsFunction(track, seqwires::FingeredChordsSustainPolicyEnum::Value::Notes);
    EXPECT_EQ(chordTrack.getDuration(), 2);

    std::vector<testUtils::ChordInfo> expectedChords = {
        {seqwires::PitchClass::Value::C, seqwires::ChordType::Value::M, 1},
        {seqwires::PitchClass::Value::C, seqwires::ChordType::Value::m, 1}};

    testUtils::testChords(expectedChords, chordTrack);
}

// Only major and minor inversions are guaranteed to be recognized.
TEST(FingeredChordsTest, majorInversions) {
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

    seqwires::Track chordTrack =
        seqwires::fingeredChordsFunction(track, seqwires::FingeredChordsSustainPolicyEnum::Value::Notes);
    EXPECT_EQ(chordTrack.getDuration(), 5);

    std::vector<testUtils::ChordInfo> expectedChords = {
        {seqwires::PitchClass::Value::C, seqwires::ChordType::Value::M, 1, 0},
        {seqwires::PitchClass::Value::C, seqwires::ChordType::Value::M, 1, 1},
        {seqwires::PitchClass::Value::C, seqwires::ChordType::Value::M, 1, 1}};

    testUtils::testChords(expectedChords, chordTrack);
}

// Only major and minor inversions are guaranteed to be recognized.
TEST(FingeredChordsTest, minorInversions) {
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

    seqwires::Track chordTrack =
        seqwires::fingeredChordsFunction(track, seqwires::FingeredChordsSustainPolicyEnum::Value::Notes);
    EXPECT_EQ(chordTrack.getDuration(), 5);

    std::vector<testUtils::ChordInfo> expectedChords = {
        {seqwires::PitchClass::Value::C, seqwires::ChordType::Value::m, 1, 0},
        {seqwires::PitchClass::Value::C, seqwires::ChordType::Value::m, 1, 1},
        {seqwires::PitchClass::Value::C, seqwires::ChordType::Value::m, 1, 1}};

    testUtils::testChords(expectedChords, chordTrack);
}

// Yamaha-style fingered chords.
TEST(FingeredChordsTest, schemeY) {
    using namespace seqwires;

    std::vector<std::vector<seqwires::Pitch>> pitches = {// C1+8
                                                         {60, 72},
                                                         // C1+5
                                                         {60, 67},
                                                         // C
                                                         {60, 64, 67},
                                                         // C9
                                                         {60, 62, 64, 67},
                                                         // C6
                                                         {60, 67, 69},
                                                         // C6
                                                         {60, 64, 67, 69},
                                                         // C69
                                                         {60, 62, 64, 69},
                                                         // C69
                                                         {60, 62, 64, 67, 69},
                                                         // CM7
                                                         {60, 64, 71},
                                                         // CM7
                                                         {60, 64, 67, 71},
                                                         // CM7(9)
                                                         {60, 62, 64, 71},
                                                         // CM7(9)
                                                         {60, 62, 64, 67, 71},
                                                         // CM7(#11)
                                                         {60, 64, 66, 67, 71},
                                                         // CM7(#11)
                                                         {60, 62, 64, 66, 67, 71},
                                                         // C(b5)
                                                         {60, 64, 66},
                                                         // CM7(b5)
                                                         {60, 64, 66, 71},
                                                         // Csus4
                                                         {60, 65, 67},
                                                         // Caug
                                                         {60, 64, 68},
                                                         // CM7aug
                                                         {60, 68, 71},
                                                         // CM7aug
                                                         {60, 64, 68, 71},
                                                         // Cm
                                                         {60, 63, 67},
                                                         // Cm(9)
                                                         {60, 62, 63, 67},
                                                         // Cm6
                                                         {60, 63, 67, 69},
                                                         // Cm7
                                                         {60, 63, 70},
                                                         // Cm7
                                                         {60, 63, 67, 70},
                                                         // Cm7(9)
                                                         {60, 62, 63, 70},
                                                         // Cm7(9)
                                                         {60, 62, 63, 67, 70},
                                                         // Cm7(11)
                                                         {60, 63, 65, 67},
                                                         // Cm7(11)
                                                         {60, 62, 63, 65, 67},
                                                         // Cm7(11)
                                                         {60, 63, 65, 67, 70},
                                                         // Cm7(11)
                                                         {60, 62, 63, 65, 67, 70},
                                                         // CmM7
                                                         {60, 63, 71},
                                                         // CmM7
                                                         {60, 63, 67, 71},
                                                         // CmM7(9)
                                                         {60, 62, 63, 71},
                                                         // CmM7(9)
                                                         {60, 62, 63, 67, 71},
                                                         // Cm7b5
                                                         {60, 63, 66, 70},
                                                         // CmM7b5
                                                         {60, 63, 66, 71},
                                                         // Cdim
                                                         {60, 63, 66},
                                                         // Cdim7
                                                         {60, 63, 66, 69},
                                                         // C7
                                                         {60, 64, 70},
                                                         // C7
                                                         {60, 64, 67, 70},
                                                         // C7(b9)
                                                         {60, 61, 64, 70},
                                                         // C7(b9)
                                                         {60, 61, 64, 67, 70},
                                                         // C7(b13)
                                                         {60, 64, 67, 68, 70},
                                                         // C7(9)
                                                         {60, 62, 64, 70},
                                                         // C7(9)
                                                         {60, 62, 64, 67, 70},
                                                         // C7(#11)
                                                         {60, 64, 66, 67, 70},
                                                         // C7(#11)
                                                         {60, 62, 64, 66, 67, 70},
                                                         // C7(13)
                                                         {60, 64, 69, 70},
                                                         // C7(13)
                                                         {60, 64, 67, 69, 70},
                                                         // C7(#9)
                                                         {60, 63, 64, 70},
                                                         // C7(#9)
                                                         {60, 63, 64, 67, 70},
                                                         // C7b5
                                                         {60, 64, 66, 70},
                                                         // C7aug
                                                         {60, 64, 68, 70},
                                                         // C7sus4
                                                         {60, 65, 70},
                                                         // C7sus4
                                                         {60, 65, 67, 70},
                                                         // C1+2+5
                                                         {60, 62, 67}};

    seqwires::Track track;
    for (const auto& v : pitches) {
        babelwires::Rational time = 1;
        for (auto p : v) {
            track.addEvent(NoteOnEvent{time, p});
            time = 0;
        }
        time = 1;
        for (auto p : v) {
            track.addEvent(NoteOffEvent{time, p});
            time = 0;
        }
    }

    seqwires::Track chordTrack =
        seqwires::fingeredChordsFunction(track, seqwires::FingeredChordsSustainPolicyEnum::Value::Notes);

    std::vector<ChordType::Value> expectedChordType = {
        ChordType::Value::_1p8,  ChordType::Value::_1p5,  ChordType::Value::M,      ChordType::Value::M9,
        ChordType::Value::M6,    ChordType::Value::M6,    ChordType::Value::M6_9,   ChordType::Value::M6_9,
        ChordType::Value::M7,    ChordType::Value::M7,    ChordType::Value::M7_9,   ChordType::Value::M7_9,
        ChordType::Value::M7s11, ChordType::Value::M7s11, ChordType::Value::b5,     ChordType::Value::M7b5,
        ChordType::Value::sus4,  ChordType::Value::aug,   ChordType::Value::Mj7aug, ChordType::Value::Mj7aug,
        ChordType::Value::m,     ChordType::Value::m9,    ChordType::Value::m6,     ChordType::Value::m7,
        ChordType::Value::m7,    ChordType::Value::m7_9,  ChordType::Value::m7_9,   ChordType::Value::m7_11,
        ChordType::Value::m7_11, ChordType::Value::m7_11, ChordType::Value::m7_11,  ChordType::Value::mM7,
        ChordType::Value::mM7,   ChordType::Value::mM7_9, ChordType::Value::mM7_9,  ChordType::Value::m7b5,
        ChordType::Value::mM7b5, ChordType::Value::dim,   ChordType::Value::dim7,   ChordType::Value::_7,
        ChordType::Value::_7,    ChordType::Value::_7b9,  ChordType::Value::_7b9,   ChordType::Value::_7b13,
        ChordType::Value::_79,   ChordType::Value::_79,   ChordType::Value::_7s11,  ChordType::Value::_7s11,
        ChordType::Value::_7_13, ChordType::Value::_7_13, ChordType::Value::_7s9,   ChordType::Value::_7s9,
        ChordType::Value::_7b5,  ChordType::Value::_7aug, ChordType::Value::_7sus4, ChordType::Value::_7sus4,
        ChordType::Value::_1p2p5};

    EXPECT_EQ(pitches.size(), expectedChordType.size());

    std::vector<testUtils::ChordInfo> expectedChords;
    for (auto c : expectedChordType) {
        expectedChords.emplace_back(testUtils::ChordInfo{PitchClass::Value::C, c, 1, 1});
    }

    testUtils::testChords(expectedChords, chordTrack);
}

// Roland-style fingered chords.
TEST(FingeredChordsTest, schemeR) {
    using namespace seqwires;

    std::vector<std::vector<seqwires::Pitch>> pitches = {// C
                                                         {60, 64, 67},
                                                         // C6
                                                         {60, 67, 69},
                                                         // C6
                                                         {60, 64, 67, 69},
                                                         // CM7
                                                         {60, 64, 71},
                                                         // CM7
                                                         {60, 64, 67, 71},
                                                         // CM7(#11)
                                                         // Ambiguous and has alternatives.
                                                         // { 60, 64, 66, 71 },
                                                         // CM7(#11)
                                                         {60, 62, 64, 66, 71},
                                                         // CM7(#11)
                                                         {60, 64, 66, 67, 71},
                                                         // CM7(#11)
                                                         {60, 62, 64, 66, 67, 71},
                                                         // CM(9)
                                                         {60, 62, 64, 67},
                                                         // CM7(9)
                                                         {60, 62, 64, 71},
                                                         // CM7(9)
                                                         {60, 62, 64, 67, 71},
                                                         // C6(9)
                                                         {60, 62, 64, 69},
                                                         // C6(9)
                                                         {60, 62, 64, 67, 69},
                                                         // Caug
                                                         {60, 64, 68},
                                                         // Cm
                                                         {60, 63, 67},
                                                         // Cm6
                                                         {60, 63, 67, 69},
                                                         // Cm7
                                                         {60, 63, 70},
                                                         // Cm7
                                                         {60, 63, 67, 70},
                                                         // Cm7(b5)
                                                         {60, 63, 66, 70},
                                                         // Cm(9)
                                                         {60, 62, 63, 67},
                                                         // Cm7(9)
                                                         {60, 62, 63, 70},
                                                         // Cm7(9)
                                                         {60, 62, 63, 67, 70},
                                                         // Cm7(11)
                                                         {60, 63, 65, 67},
                                                         // Cm7(11)
                                                         {60, 62, 63, 65, 67},
                                                         // Cm7(11)
                                                         {60, 63, 65, 67, 70},
                                                         // Cm7(11)
                                                         {60, 62, 63, 65, 67, 70},
                                                         // CmM7
                                                         {60, 63, 71},
                                                         // CmM7
                                                         {60, 63, 67, 71},
                                                         // CmM7(9)
                                                         {60, 62, 63, 71},
                                                         // CmM7(9)
                                                         {60, 62, 63, 67, 71},
                                                         // Cdim
                                                         {60, 63, 66},
                                                         // Cdim7
                                                         {60, 63, 66, 69},
                                                         // C7
                                                         {60, 64, 70},
                                                         // C7
                                                         {60, 64, 67, 70},
                                                         // C7sus4
                                                         {60, 65, 67, 70},
                                                         // C7(b5)
                                                         {60, 64, 66, 70},
                                                         // C7(9)
                                                         {60, 62, 64, 70},
                                                         // C7(9)
                                                         {60, 62, 64, 67, 70},
                                                         // C7(#11)
                                                         {60, 62, 64, 66, 70},
                                                         // C7(#11)
                                                         {60, 62, 64, 66, 67, 70},
                                                         // C7(13)
                                                         {60, 64, 69, 70},
                                                         // C7(13)
                                                         {60, 64, 67, 69, 70},
                                                         // C7(b9)
                                                         {60, 61, 64, 70},
                                                         // C7(b9)
                                                         {60, 61, 64, 67, 70},
                                                         // C7(b13)
                                                         {60, 64, 67, 68, 70},
                                                         // C7(#9)
                                                         // No root note
                                                         // { 63, 64, 70 },
                                                         // C7(#9)
                                                         {60, 63, 64, 70},
                                                         // C7(#9)
                                                         // No root note
                                                         //{ 63, 64, 67, 70 },
                                                         // C7(#9)
                                                         {60, 63, 64, 67, 70},
                                                         // CM7aug
                                                         {60, 64, 68, 71},
                                                         // C7aug
                                                         // No root note
                                                         //{ 64, 68, 70 },
                                                         // C7aug
                                                         {60, 64, 68, 70},
                                                         // Csus4
                                                         {60, 65, 67},
                                                         // C1+2+5
                                                         {60, 62, 67}};

    seqwires::Track track;
    for (const auto& v : pitches) {
        babelwires::Rational time = 1;
        for (auto p : v) {
            track.addEvent(NoteOnEvent{time, p});
            time = 0;
        }
        time = 1;
        for (auto p : v) {
            track.addEvent(NoteOffEvent{time, p});
            time = 0;
        }
    }

    seqwires::Track chordTrack =
        seqwires::fingeredChordsFunction(track, seqwires::FingeredChordsSustainPolicyEnum::Value::Notes);

    std::vector<ChordType::Value> expectedChordType = {
        ChordType::Value::M,
        ChordType::Value::M6,
        ChordType::Value::M6,
        ChordType::Value::M7,
        ChordType::Value::M7,
        ChordType::Value::M7s11,
        ChordType::Value::M7s11,
        ChordType::Value::M7s11,
        ChordType::Value::M9,
        ChordType::Value::M7_9,
        ChordType::Value::M7_9,
        ChordType::Value::M6_9,
        ChordType::Value::M6_9,
        ChordType::Value::aug,
        ChordType::Value::m,
        ChordType::Value::m6,
        ChordType::Value::m7,
        ChordType::Value::m7,
        ChordType::Value::m7b5,
        ChordType::Value::m9,
        ChordType::Value::m7_9,
        ChordType::Value::m7_9,
        ChordType::Value::m7_11,
        ChordType::Value::m7_11,
        ChordType::Value::m7_11,
        ChordType::Value::m7_11,
        ChordType::Value::mM7,
        ChordType::Value::mM7,
        ChordType::Value::mM7_9,
        ChordType::Value::mM7_9,
        ChordType::Value::dim,
        ChordType::Value::dim7,
        ChordType::Value::_7,
        ChordType::Value::_7,
        ChordType::Value::_7sus4,
        ChordType::Value::_7b5,
        ChordType::Value::_79,
        ChordType::Value::_79,
        ChordType::Value::_7s11,
        ChordType::Value::_7s11,
        ChordType::Value::_7_13,
        ChordType::Value::_7_13,
        ChordType::Value::_7b9,
        ChordType::Value::_7b9,
        ChordType::Value::_7b13,
        ChordType::Value::_7s9,
        // No root note,
        // ChordType::Value::_7s9,
        ChordType::Value::_7s9,
        // No root note,
        // ChordType::Value::_7s9,
        ChordType::Value::Mj7aug,
        ChordType::Value::_7aug,
        // ChordType::Value::No root note,
        // ChordType::Value::_7aug,
        ChordType::Value::sus4,
        ChordType::Value::_1p2p5,
    };

    EXPECT_EQ(pitches.size(), expectedChordType.size());

    std::vector<testUtils::ChordInfo> expectedChords;
    for (auto c : expectedChordType) {
        expectedChords.emplace_back(testUtils::ChordInfo{PitchClass::Value::C, c, 1, 1});
    }

    testUtils::testChords(expectedChords, chordTrack);
}

// Casio-style fingered chords.
TEST(FingeredChordsTest, schemeC) {
    using namespace seqwires;

    std::vector<std::vector<seqwires::Pitch>> pitches = {// C
                                                         {60, 64, 67},
                                                         // Cm
                                                         {60, 63, 67},
                                                         // Cdim
                                                         {60, 63, 66},
                                                         // Caug
                                                         {60, 64, 68},
                                                         // Csus4
                                                         {60, 65, 67},
                                                         // C7
                                                         {60, 64, 70},
                                                         // C7
                                                         {60, 64, 67, 70},
                                                         // Cm7
                                                         {60, 63, 70},
                                                         // Cm7
                                                         {60, 63, 67, 70},
                                                         // Cmaj7
                                                         {60, 64, 71},
                                                         // Cmaj7
                                                         {60, 64, 67, 71},
                                                         // Cm7b5
                                                         {60, 63, 66, 70},
                                                         // C7b5
                                                         {60, 64, 66, 70},
                                                         // C7sus4
                                                         {60, 65, 67, 70},
                                                         // Cadd9
                                                         {60, 62, 64},
                                                         // Cadd9
                                                         {60, 62, 64, 67},
                                                         // Cmadd9
                                                         {60, 62, 63},
                                                         // Cmadd9
                                                         {60, 62, 63, 67},
                                                         // CmM7
                                                         {60, 63, 71},
                                                         // CmM7
                                                         {60, 63, 67, 71},
                                                         // Cdim7
                                                         {60, 63, 66, 69}};

    seqwires::Track track;
    for (const auto& v : pitches) {
        babelwires::Rational time = 1;
        for (auto p : v) {
            track.addEvent(NoteOnEvent{time, p});
            time = 0;
        }
        time = 1;
        for (auto p : v) {
            track.addEvent(NoteOffEvent{time, p});
            time = 0;
        }
    }

    seqwires::Track chordTrack =
        seqwires::fingeredChordsFunction(track, seqwires::FingeredChordsSustainPolicyEnum::Value::Notes);

    std::vector<ChordType::Value> expectedChordType = {
        ChordType::Value::M,    ChordType::Value::m,      ChordType::Value::dim, ChordType::Value::aug,
        ChordType::Value::sus4, ChordType::Value::_7,     ChordType::Value::_7,  ChordType::Value::m7,
        ChordType::Value::m7,   ChordType::Value::M7,     ChordType::Value::M7,  ChordType::Value::m7b5,
        ChordType::Value::_7b5, ChordType::Value::_7sus4, ChordType::Value::M9,  ChordType::Value::M9,
        ChordType::Value::m9,   ChordType::Value::m9,     ChordType::Value::mM7, ChordType::Value::mM7,
        ChordType::Value::dim7};

    EXPECT_EQ(pitches.size(), expectedChordType.size());

    std::vector<testUtils::ChordInfo> expectedChords;
    for (auto c : expectedChordType) {
        expectedChords.emplace_back(testUtils::ChordInfo{PitchClass::Value::C, c, 1, 1});
    }

    testUtils::testChords(expectedChords, chordTrack);
}

TEST(FingeredChordsTest, processor) {
    testUtils::TestEnvironment testEnvironment;
    seqwires::registerLib(testEnvironment.m_projectContext);

    seqwires::FingeredChordsProcessor processor(testEnvironment.m_projectContext);

    processor.getInput().setToDefault();
    processor.getOutput().setToDefault();

    auto input = seqwires::FingeredChordsProcessorInput::Instance(processor.getInput());
    const auto output = seqwires::FingeredChordsProcessorOutput::ConstInstance(processor.getOutput());

    input.getPolicy().set(seqwires::FingeredChordsSustainPolicyEnum::Value::Notes);

    {
        seqwires::Track track;
        track.addEvent(seqwires::NoteOnEvent(0, 60));
        track.addEvent(seqwires::NoteOnEvent(0, 64));
        track.addEvent(seqwires::NoteOnEvent(0, 67));
        track.addEvent(seqwires::NoteOffEvent(1, 60));
        track.addEvent(seqwires::NoteOffEvent(0, 64));
        track.addEvent(seqwires::NoteOffEvent(0, 67));
        track.addEvent(seqwires::NoteOnEvent(1, 62));
        track.addEvent(seqwires::NoteOnEvent(0, 65));
        track.addEvent(seqwires::NoteOnEvent(0, 69));
        track.addEvent(seqwires::NoteOffEvent(1, 62));
        track.addEvent(seqwires::NoteOffEvent(0, 65));
        track.addEvent(seqwires::NoteOffEvent(0, 69));
        input.getNotes().set(std::move(track));
    }

    processor.process(testEnvironment.m_log);

    testUtils::testChords(
        {
            {seqwires::PitchClass::Value::C, seqwires::ChordType::Value::M, 1},
            {seqwires::PitchClass::Value::D, seqwires::ChordType::Value::m, 1, 1},
        },
        output.getChords().get());

    processor.getInput().clearChanges();
    input.getPolicy().set(seqwires::FingeredChordsSustainPolicyEnum::Value::Hold);
    processor.process(testEnvironment.m_log);

    testUtils::testChords(
        {
            {seqwires::PitchClass::Value::C, seqwires::ChordType::Value::M, 2},
            {seqwires::PitchClass::Value::D, seqwires::ChordType::Value::m, 1},
        },
        output.getChords().get());

    processor.getInput().clearChanges();
    {
        seqwires::Track track;
        track.addEvent(seqwires::NoteOnEvent(0, 60));
        track.addEvent(seqwires::NoteOnEvent(0, 64));
        track.addEvent(seqwires::NoteOnEvent(0, 67));
        track.addEvent(seqwires::NoteOffEvent(1, 60));
        track.addEvent(seqwires::NoteOffEvent(0, 64));
        track.addEvent(seqwires::NoteOffEvent(0, 67));
        track.addEvent(seqwires::NoteOnEvent(1, 64));
        track.addEvent(seqwires::NoteOnEvent(0, 67));
        track.addEvent(seqwires::NoteOnEvent(0, 71));
        track.addEvent(seqwires::NoteOffEvent(1, 64));
        track.addEvent(seqwires::NoteOffEvent(0, 67));
        track.addEvent(seqwires::NoteOffEvent(0, 71));
        input.getNotes().set(std::move(track));
    }
    processor.process(testEnvironment.m_log);

    testUtils::testChords(
        {
            {seqwires::PitchClass::Value::C, seqwires::ChordType::Value::M, 2},
            {seqwires::PitchClass::Value::E, seqwires::ChordType::Value::m, 1},
        },
        output.getChords().get());
}

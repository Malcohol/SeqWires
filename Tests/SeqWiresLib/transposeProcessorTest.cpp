#include <gtest/gtest.h>

#include <SeqWiresLib/Functions/transposeFunction.hpp>

#include <Tests/TestUtils/seqTestUtils.hpp>

TEST(TransposeProcessorTest, funcSimpleZero) {
    seqwires::Track trackIn;

    testUtils::addSimpleNotes(std::vector<seqwires::Pitch>{60, 62, 64, 65}, trackIn);

    auto trackOut = seqwires::transposeTrack(trackIn, 0);

    testUtils::testSimpleNotes(std::vector<seqwires::Pitch>{60, 62, 64, 65}, trackOut);
}

TEST(TransposeProcessorTest, funcSimplePositive) {
    seqwires::Track trackIn;

    testUtils::addSimpleNotes(std::vector<seqwires::Pitch>{60, 62, 64, 65}, trackIn);

    auto trackOut = seqwires::transposeTrack(trackIn, 1);

    testUtils::testSimpleNotes(std::vector<seqwires::Pitch>{61, 63, 65, 66}, trackOut);
}

TEST(TransposeProcessorTest, funcSimpleNegative) {
    seqwires::Track trackIn;

    testUtils::addSimpleNotes(std::vector<seqwires::Pitch>{60, 62, 64, 65}, trackIn);

    auto trackOut = seqwires::transposeTrack(trackIn, -1);

    testUtils::testSimpleNotes(std::vector<seqwires::Pitch>{59, 61, 63, 64}, trackOut);
}

TEST(TransposeProcessorTest, funcSimplePositiveLimit) {
    seqwires::Track trackIn;

    testUtils::addSimpleNotes(std::vector<seqwires::Pitch>{60, 62, 64, 65}, trackIn);

    auto trackOut = seqwires::transposeTrack(trackIn, 127);

    testUtils::testSimpleNotes(std::vector<seqwires::Pitch>{127, 127, 127, 127}, trackOut);
}

TEST(TransposeProcessorTest, funcSimpleNegativeLimit) {
    seqwires::Track trackIn;

    testUtils::addSimpleNotes(std::vector<seqwires::Pitch>{60, 62, 64, 65}, trackIn);

    auto trackOut = seqwires::transposeTrack(trackIn, -127);

    testUtils::testSimpleNotes(std::vector<seqwires::Pitch>{0, 0, 0, 0}, trackOut);
}

TEST(TransposeProcessorTest, funcSimpleChordsZero) {
    seqwires::Track trackIn;

    // Some random chords.
    testUtils::addChords({
        {seqwires::PitchClass::PITCH_CLASS_C, seqwires::ChordType::CHORD_TYPE_MAJOR},
        {seqwires::PitchClass::PITCH_CLASS_E, seqwires::ChordType::CHORD_TYPE_MINOR},
        {seqwires::PitchClass::PITCH_CLASS_G_SHARP, seqwires::ChordType::CHORD_TYPE_DIMINISHED},
        {seqwires::PitchClass::PITCH_CLASS_A, seqwires::ChordType::CHORD_TYPE_AUGMENTED_5TH},
        }, trackIn);

    auto trackOut = seqwires::transposeTrack(trackIn, 0);

    testUtils::testChords({
        {seqwires::PitchClass::PITCH_CLASS_C, seqwires::ChordType::CHORD_TYPE_MAJOR},
        {seqwires::PitchClass::PITCH_CLASS_E, seqwires::ChordType::CHORD_TYPE_MINOR},
        {seqwires::PitchClass::PITCH_CLASS_G_SHARP, seqwires::ChordType::CHORD_TYPE_DIMINISHED},
        {seqwires::PitchClass::PITCH_CLASS_A, seqwires::ChordType::CHORD_TYPE_AUGMENTED_5TH},
        }, trackIn);
}

TEST(TransposeProcessorTest, funcSimpleChordsPositive) {
    seqwires::Track trackIn;

    // Some random chords.
    testUtils::addChords({
        {seqwires::PitchClass::PITCH_CLASS_C, seqwires::ChordType::CHORD_TYPE_MAJOR},
        {seqwires::PitchClass::PITCH_CLASS_E, seqwires::ChordType::CHORD_TYPE_MINOR},
        {seqwires::PitchClass::PITCH_CLASS_G_SHARP, seqwires::ChordType::CHORD_TYPE_DIMINISHED},
        {seqwires::PitchClass::PITCH_CLASS_B, seqwires::ChordType::CHORD_TYPE_AUGMENTED_5TH},
        }, trackIn);

    auto trackOut = seqwires::transposeTrack(trackIn, 1);

    testUtils::testChords({
        {seqwires::PitchClass::PITCH_CLASS_C_SHARP, seqwires::ChordType::CHORD_TYPE_MAJOR},
        {seqwires::PitchClass::PITCH_CLASS_F, seqwires::ChordType::CHORD_TYPE_MINOR},
        {seqwires::PitchClass::PITCH_CLASS_A, seqwires::ChordType::CHORD_TYPE_DIMINISHED},
        {seqwires::PitchClass::PITCH_CLASS_C, seqwires::ChordType::CHORD_TYPE_AUGMENTED_5TH},
        }, trackOut);
}

TEST(TransposeProcessorTest, funcSimpleChordsNegative) {
    seqwires::Track trackIn;

    // Some random chords.
    testUtils::addChords({
        {seqwires::PitchClass::PITCH_CLASS_C, seqwires::ChordType::CHORD_TYPE_MAJOR},
        {seqwires::PitchClass::PITCH_CLASS_E, seqwires::ChordType::CHORD_TYPE_MINOR},
        {seqwires::PitchClass::PITCH_CLASS_G_SHARP, seqwires::ChordType::CHORD_TYPE_DIMINISHED},
        {seqwires::PitchClass::PITCH_CLASS_B, seqwires::ChordType::CHORD_TYPE_AUGMENTED_5TH},
        }, trackIn);

    auto trackOut = seqwires::transposeTrack(trackIn, -1);

    testUtils::testChords({
        {seqwires::PitchClass::PITCH_CLASS_B, seqwires::ChordType::CHORD_TYPE_MAJOR},
        {seqwires::PitchClass::PITCH_CLASS_D_SHARP, seqwires::ChordType::CHORD_TYPE_MINOR},
        {seqwires::PitchClass::PITCH_CLASS_G, seqwires::ChordType::CHORD_TYPE_DIMINISHED},
        {seqwires::PitchClass::PITCH_CLASS_A_SHARP, seqwires::ChordType::CHORD_TYPE_AUGMENTED_5TH},
        }, trackOut);
}

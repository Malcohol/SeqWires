#include <gtest/gtest.h>

#include <SeqWiresLib/Functions/appendTrackFunction.hpp>
#include <SeqWiresLib/Tracks/noteEvents.hpp>

#include <Tests/TestUtils/seqTestUtils.hpp>

TEST(ConcatenateProcessorTest, appendFuncSimple) {
    seqwires::Track trackA;
    testUtils::addSimpleNotes(std::vector<seqwires::Pitch>{60, 62, 64, 65}, trackA);

    seqwires::Track trackB;
    testUtils::addSimpleNotes(std::vector<seqwires::Pitch>{67, 69, 71, 72}, trackB);

    appendTrack(trackA, trackB);

    testUtils::testSimpleNotes(std::vector<seqwires::Pitch>{60, 62, 64, 65, 67, 69, 71, 72}, trackA);
}

TEST(ConcatenateProcessorTest, appendFuncGaps) {
    seqwires::Track trackA;
    const std::vector<testUtils::NoteInfo> noteInfosA{
        {60, 1, babelwires::Rational(1, 4)},
        {62, 0, babelwires::Rational(1, 4)},
        {64, 0, babelwires::Rational(1, 4)},
        {65, 0, babelwires::Rational(1, 4)}
    };
    testUtils::addNotes(noteInfosA, trackA);
    trackA.setDuration(3);

    seqwires::Track trackB;
    const std::vector<testUtils::NoteInfo> noteInfosB{
        {67, 1, babelwires::Rational(1, 4)},
        {69, 0, babelwires::Rational(1, 4)},
        {71, 0, babelwires::Rational(1, 4)},
        {72, 0, babelwires::Rational(1, 4)},
    };
    testUtils::addNotes(noteInfosB, trackB);
    trackB.setDuration(3);

    appendTrack(trackA, trackB);

    EXPECT_EQ(trackA.getDuration(), 6);

    const std::vector<testUtils::NoteInfo> expectedNoteInfos{
        {60, 1, babelwires::Rational(1, 4)},
        {62, 0, babelwires::Rational(1, 4)},
        {64, 0, babelwires::Rational(1, 4)},
        {65, 0, babelwires::Rational(1, 4)},
        {67, 2, babelwires::Rational(1, 4)},
        {69, 0, babelwires::Rational(1, 4)},
        {71, 0, babelwires::Rational(1, 4)},
        {72, 0, babelwires::Rational(1, 4)},
    };

    testUtils::testNotes(expectedNoteInfos, trackA);
}

#include <gtest/gtest.h>

#include <SeqWiresLib/Functions/sanitizingFunctions.hpp>
#include <SeqWiresLib/Types/Track/TrackEvents/noteEvents.hpp>

#include <Tests/BabelWiresLib/TestUtils/testEnvironment.hpp>
#include <Tests/TestUtils/seqTestUtils.hpp>

TEST(SanitizingFunctionsTest, removeZeroDurationGroups_Simple) {
    seqwires::Track trackIn;

    testUtils::addNotes(
        {
            {60, babelwires::Rational(0, 4), 0},
            {62, babelwires::Rational(0, 4), babelwires::Rational(1, 4)},
            {64, babelwires::Rational(1, 4), babelwires::Rational(0, 4)},
            {65, babelwires::Rational(1, 4), babelwires::Rational(1, 4)},
            {67, babelwires::Rational(1, 4), babelwires::Rational(0, 4)},
        },
        trackIn);

    auto trackOut = seqwires::removeZeroDurationGroups(trackIn);
    testUtils::testNotes(
        {{62, 0, babelwires::Rational(1, 4)}, {65, babelwires::Rational(1, 2), babelwires::Rational(1, 4)}}, trackOut);
    EXPECT_EQ(trackOut.getDuration(), trackIn.getDuration());
}

TEST(SanitizingFunctionsTest, removeZeroDurationGroup_MultipleGroups) {
    seqwires::Track trackIn;

    testUtils::addNotes(
        {
            {60, babelwires::Rational(0, 4), babelwires::Rational(1, 4)},
            {62, babelwires::Rational(0, 4), babelwires::Rational(0, 4)},
            {64, babelwires::Rational(0, 4), babelwires::Rational(0, 4)},
            {65, babelwires::Rational(0, 4), babelwires::Rational(0, 4)},
            {67, babelwires::Rational(1, 4), babelwires::Rational(1, 4)},
        },
        trackIn);

    auto trackOut = seqwires::removeZeroDurationGroups(trackIn);
    testUtils::testNotes(
        {{60, 0, babelwires::Rational(1, 4)}, {67, babelwires::Rational(1, 4), babelwires::Rational(1, 4)}}, trackOut);
    EXPECT_EQ(trackOut.getDuration(), trackIn.getDuration());
}

TEST(SanitizingFunctionsTest, removeZeroDurationGroup_SameNote) {
    seqwires::Track trackIn;

    testUtils::addNotes(
        {
            {60, babelwires::Rational(0, 4), babelwires::Rational(1, 4)},
            {62, babelwires::Rational(0, 4), babelwires::Rational(1, 4)},
            {62, babelwires::Rational(0, 4), babelwires::Rational(0, 4)},
            {62, babelwires::Rational(0, 4), babelwires::Rational(0, 4)},
            {62, babelwires::Rational(0, 4), babelwires::Rational(1, 4)},
            {65, babelwires::Rational(0, 4), babelwires::Rational(1, 4)},
        },
        trackIn);

    auto trackOut = seqwires::removeZeroDurationGroups(trackIn);
    testUtils::testNotes(
        {
            {60, babelwires::Rational(0, 4), babelwires::Rational(1, 4)},
            {62, babelwires::Rational(0, 4), babelwires::Rational(1, 4)},
            {62, babelwires::Rational(0, 4), babelwires::Rational(1, 4)},
            {65, babelwires::Rational(0, 4), babelwires::Rational(1, 4)},
        },
        trackOut);
    EXPECT_EQ(trackOut.getDuration(), trackIn.getDuration());
}

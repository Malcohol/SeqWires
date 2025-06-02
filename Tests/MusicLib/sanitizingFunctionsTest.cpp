#include <gtest/gtest.h>

#include <MusicLib/Functions/sanitizingFunctions.hpp>
#include <MusicLib/Types/Track/TrackEvents/noteEvents.hpp>

#include <Tests/BabelWiresLib/TestUtils/testEnvironment.hpp>
#include <Tests/TestUtils/seqTestUtils.hpp>

TEST(SanitizingFunctionsTest, removeZeroDurationGroups_Simple) {
    bw_music::Track trackIn;

    testUtils::addNotes(
        {
            {60, babelwires::Rational(0, 4), 0},
            {62, babelwires::Rational(0, 4), babelwires::Rational(1, 4)},
            {64, babelwires::Rational(1, 4), babelwires::Rational(0, 4)},
            {65, babelwires::Rational(1, 4), babelwires::Rational(1, 4)},
            {67, babelwires::Rational(1, 4), babelwires::Rational(0, 4)},
        },
        trackIn);

    auto trackOut = bw_music::removeZeroDurationGroups(trackIn);
    testUtils::testNotes(
        {{62, 0, babelwires::Rational(1, 4)}, {65, babelwires::Rational(1, 2), babelwires::Rational(1, 4)}}, trackOut);
    EXPECT_EQ(trackOut.getDuration(), trackIn.getDuration());
}

TEST(SanitizingFunctionsTest, removeZeroDurationGroup_MultipleGroups) {
    bw_music::Track trackIn;

    testUtils::addNotes(
        {
            {60, babelwires::Rational(0, 4), babelwires::Rational(1, 4)},
            {62, babelwires::Rational(0, 4), babelwires::Rational(0, 4)},
            {64, babelwires::Rational(0, 4), babelwires::Rational(0, 4)},
            {65, babelwires::Rational(0, 4), babelwires::Rational(0, 4)},
            {67, babelwires::Rational(1, 4), babelwires::Rational(1, 4)},
        },
        trackIn);

    auto trackOut = bw_music::removeZeroDurationGroups(trackIn);
    testUtils::testNotes(
        {{60, 0, babelwires::Rational(1, 4)}, {67, babelwires::Rational(1, 4), babelwires::Rational(1, 4)}}, trackOut);
    EXPECT_EQ(trackOut.getDuration(), trackIn.getDuration());
}

TEST(SanitizingFunctionsTest, removeZeroDurationGroup_SameNote) {
    bw_music::Track trackIn;

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

    auto trackOut = bw_music::removeZeroDurationGroups(trackIn);
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

#include <gtest/gtest.h>

#include <SeqWiresLib/Features/trackFeature.hpp>
#include <SeqWiresLib/Functions/quantizeFunction.hpp>
#include <SeqWiresLib/Tracks/noteEvents.hpp>

#include <Tests/BabelWiresLib/TestUtils/testEnvironment.hpp>
#include <Tests/TestUtils/seqTestUtils.hpp>

TEST(QuantizeProcessorTest, funcSimple) {
    seqwires::Track trackIn;

    testUtils::addNotes(
        {
            {60, babelwires::Rational(1, 17), babelwires::Rational(16, 17)},
            {62, babelwires::Rational(0, 17), babelwires::Rational(10, 17)},
            {64, babelwires::Rational(1, 17), babelwires::Rational(18, 17)},
            {65, babelwires::Rational(1, 17), babelwires::Rational(22, 17)},
        },
        trackIn);

    {
        auto trackOut = seqwires::quantize(trackIn, babelwires::Rational(1, 2));
        testUtils::testNotes({{60, 0, babelwires::Rational(1, 1)},
                              {62, 0, babelwires::Rational(1, 2)},
                              {64, 0, babelwires::Rational(1, 1)},
                              {65, babelwires::Rational(1, 2), babelwires::Rational(1, 1)}},
                             trackOut);
        EXPECT_EQ(trackOut.getDuration(), 4);
    }
    {
        auto trackOut = seqwires::quantize(trackIn, babelwires::Rational(1, 4));
        testUtils::testNotes({{60, 0, babelwires::Rational(1, 1)},
                              {62, 0, babelwires::Rational(1, 2)},
                              {64, babelwires::Rational(1, 4), babelwires::Rational(1, 1)},
                              {65, 0, babelwires::Rational(5, 4)}},
                             trackOut);
        EXPECT_EQ(trackOut.getDuration(), 4);
    }
    {
        auto trackOut = seqwires::quantize(trackIn, babelwires::Rational(1, 8));
        testUtils::testNotes({{60, 0, babelwires::Rational(1, 1)},
                              {62, 0, babelwires::Rational(5, 8)},
                              {64, 0, babelwires::Rational(9, 8)},
                              {65, 0, babelwires::Rational(5, 4)}},
                             trackOut);
        EXPECT_EQ(trackOut.getDuration(), 4);
    }
}

TEST(QuantizeProcessorTest, funcCollapsedGroup) {
    seqwires::Track trackIn;

    testUtils::addNotes(
        {
            {60, babelwires::Rational(1, 17), babelwires::Rational(15, 17)},
            // This note shrinks to nothing and so should be removed.
            {62, babelwires::Rational(0, 17), babelwires::Rational(2, 17)},
            {64, babelwires::Rational(0, 17), babelwires::Rational(16, 17)},
            {65, babelwires::Rational(0, 17), babelwires::Rational(17, 17)},
        },
        trackIn);

    auto trackOut = seqwires::quantize(trackIn, babelwires::Rational(1, 8));
    testUtils::testNotes({{60, 0, 1},
                          {64, 0, 1},
                          {65, 0, 1}},
                         trackOut);
}

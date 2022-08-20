#include <gtest/gtest.h>

#include <SeqWiresLib/Features/trackFeature.hpp>
#include <SeqWiresLib/Functions/quantizeFunction.hpp>
#include <SeqWiresLib/Tracks/noteEvents.hpp>

#include <Tests/BabelWiresLib/TestUtils/testEnvironment.hpp>
#include <Tests/TestUtils/seqTestUtils.hpp>

TEST(QuantizeProcessorTest, funcSimple) {
    seqwires::Track trackIn;

    testUtils::addNotes({{60, seqwires::ModelDuration(1, 17), seqwires::ModelDuration(16, 17)},
                         {62, seqwires::ModelDuration(0, 17), seqwires::ModelDuration(10, 17)},
                         {64, seqwires::ModelDuration(1, 17), seqwires::ModelDuration(18, 17)},
                         {65, seqwires::ModelDuration(1, 17), seqwires::ModelDuration(22, 17)},
                         },
                        trackIn);

    {
        auto trackOut = seqwires::quantize(trackIn, seqwires::ModelDuration(1, 2));
        testUtils::testNotes(
            {
                {60, 0, babelwires::Rational(1, 1)},
                {62, 0, babelwires::Rational(1, 2)},
                {64, 0, babelwires::Rational(1, 1)},
                {65, babelwires::Rational(1, 2), babelwires::Rational(1, 1)}
            },
            trackOut);
        EXPECT_EQ(trackOut.getDuration(), 4);
    }
    {
        auto trackOut = seqwires::quantize(trackIn, seqwires::ModelDuration(1, 4));
        testUtils::testNotes(
            {
                {60, 0, babelwires::Rational(1, 1)},
                {62, 0, babelwires::Rational(1, 2)},
                {64, babelwires::Rational(1, 4), babelwires::Rational(1, 1)},
                {65, 0, babelwires::Rational(5, 4)}
            },
            trackOut);
        EXPECT_EQ(trackOut.getDuration(), 4);
    }
    {
        auto trackOut = seqwires::quantize(trackIn, seqwires::ModelDuration(1, 8));
        testUtils::testNotes(
            {
                {60, 0, babelwires::Rational(1, 1)},
                {62, 0, babelwires::Rational(5, 8)},
                {64, 0, babelwires::Rational(9, 8)},
                {65, 0, babelwires::Rational(5, 4)}
            },
            trackOut);
        EXPECT_EQ(trackOut.getDuration(), 4);
    }
}
#include <gtest/gtest.h>

#include <SeqWiresLib/Features/trackFeature.hpp>
#include <SeqWiresLib/Functions/quantizeFunction.hpp>
#include <SeqWiresLib/Processors/quantizeProcessor.hpp>
#include <SeqWiresLib/Tracks/noteEvents.hpp>

#include <BabelWiresLib/Types/Rational/rationalFeature.hpp>

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
    testUtils::testNotes({{60, 0, 1}, {64, 0, 1}, {65, 0, 1}}, trackOut);
}

TEST(QuantizeProcessorTest, processor) {
    testUtils::TestEnvironment testEnvironment;

    seqwires::QuantizeProcessor processor(testEnvironment.m_projectContext);

    processor.getInputFeature()->setToDefault();
    processor.getOutputFeature()->setToDefault();

    auto* beatFeature =
        processor.getInputFeature()->getChildFromStep(babelwires::PathStep("Beat")).as<babelwires::RationalFeature>();
    auto* inputArray =
        processor.getInputFeature()->getChildFromStep(babelwires::PathStep("Tracks")).as<babelwires::ArrayFeature>();
    auto* outputArray =
        processor.getOutputFeature()->getChildFromStep(babelwires::PathStep("Tracks")).as<babelwires::ArrayFeature>();
    ASSERT_NE(beatFeature, nullptr);
    ASSERT_NE(inputArray, nullptr);
    ASSERT_NE(outputArray, nullptr);
    EXPECT_GT(beatFeature->get(), 0);
    beatFeature->set(babelwires::Rational(1, 8));

    EXPECT_EQ(inputArray->getNumFeatures(), 1);
    EXPECT_EQ(outputArray->getNumFeatures(), 1);

    auto getInputTrack = [&inputArray](int i) { return inputArray->getChildFromStep(i).as<seqwires::TrackFeature>(); };
    auto getOutputTrack = [&outputArray](int i) {
        return outputArray->getChildFromStep(i).as<seqwires::TrackFeature>();
    };

    ASSERT_NE(getInputTrack(0), nullptr);
    ASSERT_NE(getOutputTrack(0), nullptr);

    EXPECT_EQ(getInputTrack(0)->get().getDuration(), 0);
    EXPECT_EQ(getOutputTrack(0)->get().getDuration(), 0);

    {
        seqwires::Track track;
        testUtils::addNotes(
            {
                {60, babelwires::Rational(1, 17), babelwires::Rational(16, 17)},
                {62, babelwires::Rational(0, 17), babelwires::Rational(10, 17)},
                {64, babelwires::Rational(1, 17), babelwires::Rational(18, 17)},
                {65, babelwires::Rational(1, 17), babelwires::Rational(22, 17)},
            },
            track);
        getInputTrack(0)->set(std::move(track));
    }
    processor.process(testEnvironment.m_log);
    testUtils::testNotes({{60, 0, babelwires::Rational(1, 1)},
                          {62, 0, babelwires::Rational(5, 8)},
                          {64, 0, babelwires::Rational(9, 8)},
                          {65, 0, babelwires::Rational(5, 4)}},
                         getOutputTrack(0)->get());

    beatFeature->set(babelwires::Rational(1, 4));
    processor.process(testEnvironment.m_log);
    testUtils::testNotes({{60, 0, babelwires::Rational(1, 1)},
                          {62, 0, babelwires::Rational(1, 2)},
                          {64, babelwires::Rational(1, 4), babelwires::Rational(1, 1)},
                          {65, 0, babelwires::Rational(5, 4)}},
                         getOutputTrack(0)->get());

    {
        seqwires::Track track;
        testUtils::addNotes(
            {
                {60, babelwires::Rational(1, 17), babelwires::Rational(16, 17)},
                {62, babelwires::Rational(0, 17), babelwires::Rational(27, 17)},
                {64, babelwires::Rational(1, 17), babelwires::Rational(18, 17)},
                {65, babelwires::Rational(1, 17), babelwires::Rational(22, 17)},
            },
            track);
        getInputTrack(0)->set(std::move(track));
    }

    processor.process(testEnvironment.m_log);
    testUtils::testNotes({{60, 0, babelwires::Rational(1, 1)},
                          {62, 0, babelwires::Rational(3, 2)},
                          {64, babelwires::Rational(1, 4), babelwires::Rational(1, 1)},
                          {65, 0, babelwires::Rational(5, 4)}},
                         getOutputTrack(0)->get());

}
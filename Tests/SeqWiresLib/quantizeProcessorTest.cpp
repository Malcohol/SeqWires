#include <gtest/gtest.h>

#include <SeqWiresLib/Functions/quantizeFunction.hpp>
#include <SeqWiresLib/Processors/quantizeProcessor.hpp>
#include <SeqWiresLib/Types/Track/TrackEvents/noteEvents.hpp>
#include <SeqWiresLib/libRegistration.hpp>

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
    seqwires::registerLib(testEnvironment.m_projectContext);

    seqwires::QuantizeProcessor processor(testEnvironment.m_projectContext);

    processor.getInputFeature().setToDefault();
    processor.getOutputFeature().setToDefault();

    babelwires::ValueFeature& inputValueFeature = processor.getInputFeature();
    const babelwires::ValueFeature& outputValueFeature = processor.getOutputFeature();

    babelwires::ValueFeature& inputArrayFeature =
        inputValueFeature.getChildFromStep(babelwires::PathStep(seqwires::QuantizeProcessor::getCommonArrayId()))
            .is<babelwires::ValueFeature>();
    const babelwires::ValueFeature& outputArrayFeature =
        outputValueFeature.getChildFromStep(babelwires::PathStep(seqwires::QuantizeProcessor::getCommonArrayId()))
            .is<babelwires::ValueFeature>();

    babelwires::ArrayInstanceImpl<babelwires::ValueFeature, seqwires::TrackType> inputArray(inputArrayFeature);
    const babelwires::ArrayInstanceImpl<const babelwires::ValueFeature, seqwires::TrackType> outputArray(
        outputArrayFeature);

    seqwires::QuantizeProcessorInput::Instance input(inputValueFeature);

    EXPECT_EQ(inputArray.getSize(), 1);
    EXPECT_EQ(outputArray.getSize(), 1);

    EXPECT_EQ(inputArray.getEntry(0).get().getDuration(), 0);
    EXPECT_EQ(outputArray.getEntry(0).get().getDuration(), 0);

    {
        babelwires::BackupScope scope(processor.getInputFeature().is<babelwires::SimpleValueFeature>());
        input.getBeat().set(babelwires::Rational(1, 8));
        seqwires::Track track;
        testUtils::addNotes(
            {
                {60, babelwires::Rational(1, 17), babelwires::Rational(16, 17)},
                {62, babelwires::Rational(0, 17), babelwires::Rational(10, 17)},
                {64, babelwires::Rational(1, 17), babelwires::Rational(18, 17)},
                {65, babelwires::Rational(1, 17), babelwires::Rational(22, 17)},
            },
            track);
        inputArray.getEntry(0).set(std::move(track));
    }
    processor.process(testEnvironment.m_log);

    testUtils::testNotes({{60, 0, babelwires::Rational(1, 1)},
                          {62, 0, babelwires::Rational(5, 8)},
                          {64, 0, babelwires::Rational(9, 8)},
                          {65, 0, babelwires::Rational(5, 4)}},
                         outputArray.getEntry(0).get());

    processor.getInputFeature().clearChanges();
    {
        babelwires::BackupScope scope(processor.getInputFeature().is<babelwires::SimpleValueFeature>());
        input.getBeat().set(babelwires::Rational(1, 4));
    }
    processor.process(testEnvironment.m_log);

    testUtils::testNotes({{60, 0, babelwires::Rational(1, 1)},
                          {62, 0, babelwires::Rational(1, 2)},
                          {64, babelwires::Rational(1, 4), babelwires::Rational(1, 1)},
                          {65, 0, babelwires::Rational(5, 4)}},
                         outputArray.getEntry(0).get());

    processor.getInputFeature().clearChanges();
    {
        babelwires::BackupScope scope(processor.getInputFeature().is<babelwires::SimpleValueFeature>());
        seqwires::Track track;
        testUtils::addNotes(
            {
                {60, babelwires::Rational(1, 17), babelwires::Rational(16, 17)},
                {62, babelwires::Rational(0, 17), babelwires::Rational(27, 17)},
                {64, babelwires::Rational(1, 17), babelwires::Rational(18, 17)},
                {65, babelwires::Rational(1, 17), babelwires::Rational(22, 17)},
            },
            track);
        inputArray.getEntry(0).set(std::move(track));
    }
    processor.process(testEnvironment.m_log);

    testUtils::testNotes({{60, 0, babelwires::Rational(1, 1)},
                          {62, 0, babelwires::Rational(3, 2)},
                          {64, babelwires::Rational(1, 4), babelwires::Rational(1, 1)},
                          {65, 0, babelwires::Rational(5, 4)}},
                         outputArray.getEntry(0).get());
}

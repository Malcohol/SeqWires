#include <gtest/gtest.h>

#include <BabelWiresLib/Features/simpleValueFeature.hpp>

#include <SeqWiresLib/Functions/repeatFunction.hpp>
#include <SeqWiresLib/Processors/repeatProcessor.hpp>
#include <SeqWiresLib/Types/Track/TrackEvents/noteEvents.hpp>
#include <SeqWiresLib/Types/Track/trackInstance.hpp>
#include <SeqWiresLib/libRegistration.hpp>

#include <Tests/BabelWiresLib/TestUtils/testEnvironment.hpp>
#include <Tests/TestUtils/seqTestUtils.hpp>

TEST(RepeatProcessorTest, funcSimpleZero) {
    seqwires::Track trackIn;

    testUtils::addSimpleNotes(std::vector<seqwires::Pitch>{60, 62, 64, 65}, trackIn);

    auto trackOut = seqwires::repeatTrack(trackIn, 0);

    EXPECT_EQ(trackOut.getNumEvents(), 0);
    EXPECT_EQ(trackOut.getDuration(), 0);
}

TEST(RepeatProcessorTest, funcSimpleOnce) {
    seqwires::Track trackIn;

    testUtils::addSimpleNotes(std::vector<seqwires::Pitch>{60, 62, 64, 65}, trackIn);

    auto trackOut = seqwires::repeatTrack(trackIn, 1);

    testUtils::testSimpleNotes(std::vector<seqwires::Pitch>{60, 62, 64, 65}, trackOut);
}

TEST(RepeatProcessorTest, funcSimpleTwice) {
    seqwires::Track trackIn;

    testUtils::addSimpleNotes(std::vector<seqwires::Pitch>{60, 62, 64, 65}, trackIn);

    auto trackOut = seqwires::repeatTrack(trackIn, 2);

    testUtils::testSimpleNotes(std::vector<seqwires::Pitch>{60, 62, 64, 65, 60, 62, 64, 65}, trackOut);
}

TEST(RepeatProcessorTest, processor) {
    testUtils::TestEnvironment testEnvironment;
    seqwires::registerLib(testEnvironment.m_projectContext);

    seqwires::RepeatProcessor processor(testEnvironment.m_projectContext);

    processor.getInputFeature().setToDefault();
    processor.getOutputFeature().setToDefault();

    babelwires::Feature& inputValueFeature = processor.getInputFeature();
    const babelwires::Feature& outputValueFeature = processor.getOutputFeature();

    babelwires::Feature& inputArrayFeature =
        inputValueFeature.getChildFromStep(babelwires::PathStep(seqwires::RepeatProcessor::getCommonArrayId()))
            .is<babelwires::Feature>();
    const babelwires::Feature& outputArrayFeature =
        outputValueFeature.getChildFromStep(babelwires::PathStep(seqwires::RepeatProcessor::getCommonArrayId()))
            .is<babelwires::Feature>();

    babelwires::ArrayInstanceImpl<babelwires::Feature, seqwires::TrackType> inputArray(inputArrayFeature);
    const babelwires::ArrayInstanceImpl<const babelwires::Feature, seqwires::TrackType> outputArray(
        outputArrayFeature);

    seqwires::RepeatProcessorInput::Instance input(inputValueFeature);

    EXPECT_EQ(inputArray.getSize(), 1);
    EXPECT_EQ(outputArray.getSize(), 1);

    EXPECT_EQ(inputArray.getEntry(0).get().getDuration(), 0);
    EXPECT_EQ(outputArray.getEntry(0).get().getDuration(), 0);

    {
        babelwires::BackupScope scope(processor.getInputFeature().is<babelwires::SimpleValueFeature>());
        seqwires::Track track;
        testUtils::addSimpleNotes({60, 62, 64, 65}, track);
        inputArray.getEntry(0).set(std::move(track));
    }
    processor.process(testEnvironment.m_log);

    testUtils::testSimpleNotes(std::vector<seqwires::Pitch>{60, 62, 64, 65, 60, 62, 64, 65},
                               outputArray.getEntry(0).get());

    processor.getInputFeature().clearChanges();
    {
        babelwires::BackupScope scope(processor.getInputFeature().is<babelwires::SimpleValueFeature>());
        input.getCount().set(1);
    }
    processor.process(testEnvironment.m_log);

    testUtils::testSimpleNotes(std::vector<seqwires::Pitch>{60, 62, 64, 65}, outputArray.getEntry(0).get());

    processor.getInputFeature().clearChanges();
    {
        babelwires::BackupScope scope(processor.getInputFeature().is<babelwires::SimpleValueFeature>());
        inputArray.setSize(2);
        {
            seqwires::Track track;
            testUtils::addSimpleNotes(std::vector<seqwires::Pitch>{48, 50, 52, 53}, track);
            inputArray.getEntry(1).set(std::move(track));
        }
    }
    processor.process(testEnvironment.m_log);

    ASSERT_EQ(outputArray.getSize(), 2);
    EXPECT_EQ(outputArray.getEntry(0).get().getDuration(), 1);
    EXPECT_EQ(outputArray.getEntry(1).get().getDuration(), 1);
    testUtils::testSimpleNotes(std::vector<seqwires::Pitch>{60, 62, 64, 65}, outputArray.getEntry(0).get());
    testUtils::testSimpleNotes(std::vector<seqwires::Pitch>{48, 50, 52, 53}, outputArray.getEntry(1).get());
}
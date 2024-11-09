#include <gtest/gtest.h>

#include <BabelWiresLib/ValueTree/valueTreeRoot.hpp>

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

    processor.getInput().setToDefault();
    processor.getOutput().setToDefault();

    babelwires::ValueTreeNode& input = processor.getInput();
    const babelwires::ValueTreeNode& output = processor.getOutput();

    babelwires::ValueTreeNode& inputArray =
        input.getChildFromStep(babelwires::PathStep(seqwires::RepeatProcessor::getCommonArrayId()))
            .is<babelwires::ValueTreeNode>();
    const babelwires::ValueTreeNode& outputArray =
        output.getChildFromStep(babelwires::PathStep(seqwires::RepeatProcessor::getCommonArrayId()))
            .is<babelwires::ValueTreeNode>();

    babelwires::ArrayInstanceImpl<babelwires::ValueTreeNode, seqwires::TrackType> inArray(inputArray);
    const babelwires::ArrayInstanceImpl<const babelwires::ValueTreeNode, seqwires::TrackType> outArray(
        outputArray);

    seqwires::RepeatProcessorInput::Instance in(input);

    EXPECT_EQ(inArray.getSize(), 1);
    EXPECT_EQ(outArray.getSize(), 1);

    EXPECT_EQ(inArray.getEntry(0).get().getDuration(), 0);
    EXPECT_EQ(outArray.getEntry(0).get().getDuration(), 0);

    {
        seqwires::Track track;
        testUtils::addSimpleNotes({60, 62, 64, 65}, track);
        inArray.getEntry(0).set(std::move(track));
    }
    processor.process(testEnvironment.m_log);

    testUtils::testSimpleNotes(std::vector<seqwires::Pitch>{60, 62, 64, 65, 60, 62, 64, 65},
                               outArray.getEntry(0).get());

    processor.getInput().clearChanges();
    in.getCount().set(1);
    processor.process(testEnvironment.m_log);

    testUtils::testSimpleNotes(std::vector<seqwires::Pitch>{60, 62, 64, 65}, outArray.getEntry(0).get());

    processor.getInput().clearChanges();
    {
        inArray.setSize(2);
        {
            seqwires::Track track;
            testUtils::addSimpleNotes(std::vector<seqwires::Pitch>{48, 50, 52, 53}, track);
            inArray.getEntry(1).set(std::move(track));
        }
    }
    processor.process(testEnvironment.m_log);

    ASSERT_EQ(outArray.getSize(), 2);
    EXPECT_EQ(outArray.getEntry(0).get().getDuration(), 1);
    EXPECT_EQ(outArray.getEntry(1).get().getDuration(), 1);
    testUtils::testSimpleNotes(std::vector<seqwires::Pitch>{60, 62, 64, 65}, outArray.getEntry(0).get());
    testUtils::testSimpleNotes(std::vector<seqwires::Pitch>{48, 50, 52, 53}, outArray.getEntry(1).get());
}
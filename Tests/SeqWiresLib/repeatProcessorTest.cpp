#include <gtest/gtest.h>

#include <SeqWiresLib/Functions/repeatFunction.hpp>
#include <SeqWiresLib/Processors/repeatProcessor.hpp>
#include <SeqWiresLib/Tracks/noteEvents.hpp>
#include <SeqWiresLib/Features/trackFeature.hpp>

#include <BabelWiresLib/Types/Int/intFeature.hpp>
#include <BabelWiresLib/Features/arrayFeature.hpp>

#include <Tests/TestUtils/seqTestUtils.hpp>
#include <Tests/BabelWiresLib/TestUtils/testEnvironment.hpp>

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

    seqwires::RepeatProcessor processor(testEnvironment.m_projectContext);

    processor.getInputFeature()->setToDefault();
    processor.getOutputFeature()->setToDefault();

    auto* countFeature = processor.getInputFeature()->getChildFromStep(babelwires::PathStep("Count")).as<babelwires::IntFeature2>();
    auto* inputArray = processor.getInputFeature()->getChildFromStep(babelwires::PathStep("Tracks")).as<babelwires::ArrayFeature>();
    auto* outputArray = processor.getOutputFeature()->getChildFromStep(babelwires::PathStep("Tracks")).as<babelwires::ArrayFeature>();
    ASSERT_NE(countFeature, nullptr);
    ASSERT_NE(inputArray, nullptr);
    ASSERT_NE(outputArray, nullptr);
    EXPECT_EQ(countFeature->get(), 2);

    EXPECT_EQ(inputArray->getNumFeatures(), 1);
    EXPECT_EQ(outputArray->getNumFeatures(), 1);

    auto getInputTrack = [&inputArray](int i) { return inputArray->getChildFromStep(i).as<seqwires::TrackFeature>(); };
    auto getOutputTrack = [&outputArray](int i) { return outputArray->getChildFromStep(i).as<seqwires::TrackFeature>(); };

    ASSERT_NE(getInputTrack(0), nullptr);
    ASSERT_NE(getOutputTrack(0), nullptr);

    EXPECT_EQ(getInputTrack(0)->get().getDuration(), 0);
    EXPECT_EQ(getOutputTrack(0)->get().getDuration(), 0);

    {
        seqwires::Track track;
        testUtils::addSimpleNotes({60, 62, 64, 65}, track);
        getInputTrack(0)->set(std::move(track));
    }
    processor.process(testEnvironment.m_log);
    testUtils::testSimpleNotes(std::vector<seqwires::Pitch>{60, 62, 64, 65, 60, 62, 64, 65}, getOutputTrack(0)->get());

    countFeature->set(1);
    processor.process(testEnvironment.m_log);
    testUtils::testSimpleNotes(std::vector<seqwires::Pitch>{60, 62, 64, 65}, getOutputTrack(0)->get());

    // Confirm that adding a track entry does not cause existing entries to be changed.
    processor.getInputFeature()->clearChanges();
    processor.getOutputFeature()->clearChanges();
    inputArray->addEntry(0);
    {
        seqwires::Track track;
        testUtils::addSimpleNotes(std::vector<seqwires::Pitch>{48, 50, 52, 53}, track);
        getInputTrack(0)->set(std::move(track));
    }
    processor.process(testEnvironment.m_log);
    ASSERT_EQ(outputArray->getNumFeatures(), 2);
    ASSERT_NE(getOutputTrack(0), nullptr);
    ASSERT_NE(getOutputTrack(1), nullptr);
    EXPECT_FALSE(getOutputTrack(1)->isChanged(babelwires::Feature::Changes::SomethingChanged));
    EXPECT_EQ(getOutputTrack(0)->get().getDuration(), 1);
    testUtils::testSimpleNotes(std::vector<seqwires::Pitch>{48, 50, 52, 53}, getOutputTrack(0)->get());
    testUtils::testSimpleNotes(std::vector<seqwires::Pitch>{60, 62, 64, 65}, getOutputTrack(1)->get());
}
#include <gtest/gtest.h>

#include <SeqWiresLib/Functions/repeatFunction.hpp>
#include <SeqWiresLib/Processors/repeatProcessor.hpp>
#include <SeqWiresLib/Tracks/noteEvents.hpp>
#include <SeqWiresLib/Features/trackFeature.hpp>

#include <BabelWires/Features/numericFeature.hpp>
#include <BabelWires/Features/arrayFeature.hpp>

#include <Tests/TestUtils/seqTestUtils.hpp>
#include <Tests/TestUtils/testLog.hpp>

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
    testUtils::TestLog log;

    seqwires::RepeatProcessor processor;

    processor.getInputFeature()->setToDefault();
    processor.getOutputFeature()->setToDefault();

    auto* countFeature = dynamic_cast<babelwires::IntFeature*>(&processor.getInputFeature()->getChildFromStep(babelwires::PathStep("Count")));
    auto* inputArray = dynamic_cast<babelwires::ArrayFeature*>(&processor.getInputFeature()->getChildFromStep(babelwires::PathStep("Tracks")));
    auto* outputArray = dynamic_cast<babelwires::ArrayFeature*>(&processor.getOutputFeature()->getChildFromStep(babelwires::PathStep("Tracks")));
    ASSERT_NE(countFeature, nullptr);
    ASSERT_NE(inputArray, nullptr);
    ASSERT_NE(outputArray, nullptr);
    EXPECT_EQ(countFeature->get(), 2);

    EXPECT_EQ(inputArray->getNumFeatures(), 1);
    EXPECT_EQ(outputArray->getNumFeatures(), 1);

    auto getInputTrack = [&inputArray](int i) { return dynamic_cast<seqwires::TrackFeature*>(&inputArray->getChildFromStep(i)); };
    auto getOutputTrack = [&outputArray](int i) { return dynamic_cast<seqwires::TrackFeature*>(&outputArray->getChildFromStep(i)); };

    ASSERT_NE(getInputTrack(0), nullptr);
    ASSERT_NE(getOutputTrack(0), nullptr);

    EXPECT_EQ(getInputTrack(0)->get().getDuration(), 0);
    EXPECT_EQ(getOutputTrack(0)->get().getDuration(), 0);

    {
        seqwires::Track track;
        testUtils::addSimpleNotes({60, 62, 64, 65}, track);
        getInputTrack(0)->set(std::move(track));
    }
    processor.process(log);
    testUtils::testSimpleNotes(std::vector<seqwires::Pitch>{60, 62, 64, 65, 60, 62, 64, 65}, getOutputTrack(0)->get());

    countFeature->set(1);
    processor.process(log);
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
    processor.process(log);
    ASSERT_EQ(outputArray->getNumFeatures(), 2);
    ASSERT_NE(getOutputTrack(0), nullptr);
    ASSERT_NE(getOutputTrack(1), nullptr);
    EXPECT_FALSE(getOutputTrack(1)->isChanged(babelwires::Feature::Changes::SomethingChanged));
    EXPECT_EQ(getOutputTrack(0)->get().getDuration(), 1);
    testUtils::testSimpleNotes(std::vector<seqwires::Pitch>{48, 50, 52, 53}, getOutputTrack(0)->get());
    testUtils::testSimpleNotes(std::vector<seqwires::Pitch>{60, 62, 64, 65}, getOutputTrack(1)->get());
}
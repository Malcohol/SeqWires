#include <gtest/gtest.h>

#include <SeqWiresLib/Functions/excerptFunction.hpp>
#include <SeqWiresLib/Types/Track/TrackEvents/noteEvents.hpp>
#include <SeqWiresLib/Processors/excerptProcessor.hpp>
#include <SeqWiresLib/Types/Track/trackFeature.hpp>

#include <BabelWiresLib/Features/arrayFeature.hpp>
#include <BabelWiresLib/Features/rootFeature.hpp>
#include <BabelWiresLib/Types/Rational/rationalFeature.hpp>

#include <Tests/TestUtils/seqTestUtils.hpp>
#include <Tests/BabelWiresLib/TestUtils/testEnvironment.hpp>

TEST(ExcerptProcessorTest, funcSimple) {
    seqwires::Track trackIn;

    testUtils::addSimpleNotes(std::vector<seqwires::Pitch>{60, 62, 64, 65, 67, 69, 71, 72}, trackIn);

    auto trackOut = seqwires::getTrackExcerpt(trackIn, babelwires::Rational(1, 2), 1);

    testUtils::testSimpleNotes(std::vector<seqwires::Pitch>{64, 65, 67, 69}, trackOut);
}

TEST(ExcerptProcessorTest, funcEmptyBefore) {
    seqwires::Track trackIn;
    seqwires::NoteOnEvent note;
    note.m_pitch = 64;
    note.m_velocity = 100;
    note.setTimeSinceLastEvent(10);
    trackIn.addEvent(note);

    auto trackOut = seqwires::getTrackExcerpt(trackIn, 3, 4);

    EXPECT_EQ(trackOut.getDuration(), 4);
    EXPECT_EQ(trackOut.getNumEvents(), 0);
}

TEST(ExcerptProcessorTest, funcEmptyAfter) {
    seqwires::Track trackIn;

    auto trackOut = seqwires::getTrackExcerpt(trackIn, 2, 2);

    EXPECT_EQ(trackOut.getDuration(), 2);
    EXPECT_EQ(trackOut.getNumEvents(), 0);
}

TEST(ExcerptProcessorTest, funcEmptyBetween) {
    seqwires::Track trackIn;
    seqwires::NoteOnEvent noteOn;
    noteOn.m_pitch = 64;
    noteOn.m_velocity = 100;
    noteOn.setTimeSinceLastEvent(1);
    trackIn.addEvent(noteOn);
    seqwires::NoteOffEvent noteOff;
    noteOff.m_pitch = 64;
    noteOff.m_velocity = 100;
    noteOff.setTimeSinceLastEvent(10);
    trackIn.addEvent(noteOff);

    auto trackOut = seqwires::getTrackExcerpt(trackIn, 2, 2);

    EXPECT_EQ(trackOut.getDuration(), 2);
    EXPECT_EQ(trackOut.getNumEvents(), 0);
}

TEST(ExcerptProcessorTest, funcDropSpanningGroup) {
    seqwires::Track trackIn;
    seqwires::NoteOnEvent noteOn;
    noteOn.m_pitch = 40;
    noteOn.m_velocity = 100;
    noteOn.setTimeSinceLastEvent(1);
    trackIn.addEvent(noteOn);
    testUtils::addSimpleNotes(std::vector<seqwires::Pitch>{60, 62, 64, 65, 67, 69, 71, 72}, trackIn);
    seqwires::NoteOffEvent noteOff;
    noteOff.m_pitch = 40;
    noteOff.m_velocity = 100;
    noteOff.setTimeSinceLastEvent(1);
    trackIn.addEvent(noteOff);

    auto trackOut = seqwires::getTrackExcerpt(trackIn, babelwires::Rational(3, 2), 1);

    testUtils::testSimpleNotes(std::vector<seqwires::Pitch>{64, 65, 67, 69}, trackOut);
}

TEST(ExcerptProcessorTest, funcDropInitialGroup) {
    seqwires::Track trackIn;

    seqwires::NoteOnEvent noteOn;
    noteOn.m_pitch = 40;
    noteOn.m_velocity = 100;
    noteOn.setTimeSinceLastEvent(1);
    trackIn.addEvent(noteOn);
    testUtils::addSimpleNotes(std::vector<seqwires::Pitch>{60, 62, 64, 65}, trackIn);

    seqwires::NoteOffEvent noteOff;
    noteOff.m_pitch = 40;
    noteOff.m_velocity = 100;
    noteOff.setTimeSinceLastEvent(0);
    trackIn.addEvent(noteOff);

    testUtils::addSimpleNotes(std::vector<seqwires::Pitch>{67, 69, 71, 72}, trackIn);

    auto trackOut = seqwires::getTrackExcerpt(trackIn, babelwires::Rational(3, 2), 1);

    testUtils::testSimpleNotes(std::vector<seqwires::Pitch>{64, 65, 67, 69}, trackOut);
}

TEST(ExcerptProcessorTest, funcGaps) {
    seqwires::Track trackIn;

    testUtils::addSimpleNotes(std::vector<seqwires::Pitch>{60, 62, 64, 65, 67, 69, 71, 72}, trackIn);

    auto trackOut = seqwires::getTrackExcerpt(trackIn, babelwires::Rational(3, 8), 1);

    const std::vector<testUtils::NoteInfo> expectedNoteInfos{
        {64, babelwires::Rational(1, 8), babelwires::Rational(1, 4)},
        {65, 0, babelwires::Rational(1, 4)},
        {67, 0, babelwires::Rational(1, 4)},
        {69, 0, babelwires::Rational(1, 8)}
    };

    testUtils::testNotes(expectedNoteInfos, trackOut);
}

TEST(ExcerptProcessorTest, processor) {
    testUtils::TestEnvironment testEnvironment;
    testEnvironment.m_typeSystem.addEntry<seqwires::DefaultTrackType>();

    seqwires::ExcerptProcessor processor(testEnvironment.m_projectContext);

    processor.getInputFeature()->setToDefault();
    processor.getOutputFeature()->setToDefault();

    auto* startFeature = processor.getInputRootFeature()->getChildFromStep(babelwires::PathStep("Start")).as<babelwires::RationalFeature>();
    auto* durationFeature = processor.getInputRootFeature()->getChildFromStep(babelwires::PathStep("Duratn")).as<babelwires::RationalFeature>();
    auto* inputArray = processor.getInputRootFeature()->getChildFromStep(babelwires::PathStep("Tracks")).as<babelwires::ArrayFeature>();
    auto* outputArray = processor.getOutputRootFeature()->getChildFromStep(babelwires::PathStep("Tracks")).as<babelwires::ArrayFeature>();
    ASSERT_NE(startFeature, nullptr);
    ASSERT_NE(durationFeature, nullptr);
    ASSERT_NE(inputArray, nullptr);
    ASSERT_NE(outputArray, nullptr);

    EXPECT_EQ(inputArray->getNumFeatures(), 1);
    EXPECT_EQ(outputArray->getNumFeatures(), 1);

    auto getInputTrack = [&inputArray](int i) { return inputArray->getChildFromStep(i).as<seqwires::TrackFeature>(); };
    auto getOutputTrack = [&outputArray](int i) { return outputArray->getChildFromStep(i).as<seqwires::TrackFeature>(); };

    ASSERT_NE(getInputTrack(0), nullptr);
    ASSERT_NE(getOutputTrack(0), nullptr);

    EXPECT_EQ(getInputTrack(0)->get().getDuration(), 0);
    EXPECT_EQ(getOutputTrack(0)->get().getDuration(), 0);

    durationFeature->set(1);
    processor.process(testEnvironment.m_log);
    EXPECT_EQ(getOutputTrack(0)->get().getDuration(), 1);
    EXPECT_EQ(getOutputTrack(0)->get().getNumEvents(), 0);

    {
        seqwires::Track track;
        testUtils::addSimpleNotes({60, 62, 64, 65, 67, 69, 71, 72}, track);
        getInputTrack(0)->set(std::move(track));
    }
    processor.process(testEnvironment.m_log);
    EXPECT_EQ(getOutputTrack(0)->get().getDuration(), 1);
    testUtils::testSimpleNotes(std::vector<seqwires::Pitch>{60, 62, 64, 65}, getOutputTrack(0)->get());

    startFeature->set(1);
    processor.process(testEnvironment.m_log);
    EXPECT_EQ(getOutputTrack(0)->get().getDuration(), 1);
    testUtils::testSimpleNotes(std::vector<seqwires::Pitch>{67, 69, 71, 72}, getOutputTrack(0)->get());

    // Confirm that adding a track entry does not cause existing entries to be changed.
    processor.getInputFeature()->clearChanges();
    processor.getOutputFeature()->clearChanges();
    inputArray->addEntry(0);
    {
        seqwires::Track track;
        testUtils::addSimpleNotes(std::vector<seqwires::Pitch>{48, 50, 52, 53, 55, 57, 59, 60}, track);
        getInputTrack(0)->set(std::move(track));
    }
    processor.process(testEnvironment.m_log);
    ASSERT_EQ(outputArray->getNumFeatures(), 2);
    ASSERT_NE(getOutputTrack(0), nullptr);
    ASSERT_NE(getOutputTrack(1), nullptr);
    EXPECT_FALSE(getOutputTrack(1)->isChanged(babelwires::Feature::Changes::SomethingChanged));
    EXPECT_EQ(getOutputTrack(0)->get().getDuration(), 1);
    testUtils::testSimpleNotes(std::vector<seqwires::Pitch>{55, 57, 59, 60}, getOutputTrack(0)->get());
    testUtils::testSimpleNotes(std::vector<seqwires::Pitch>{67, 69, 71, 72}, getOutputTrack(1)->get());
}
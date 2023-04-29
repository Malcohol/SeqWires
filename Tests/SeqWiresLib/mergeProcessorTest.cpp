#include <gtest/gtest.h>

#include <SeqWiresLib/Processors/mergeProcessor.hpp>
#include <SeqWiresLib/Functions/mergeFunction.hpp>
#include <SeqWiresLib/Types/Track/TrackEvents/noteEvents.hpp>
#include <SeqWiresLib/Types/Track/trackFeature.hpp>
#include <SeqWiresLib/Types/Track/TrackEvents/trackEventHolder.hpp>

#include <BabelWiresLib/Features/arrayFeature.hpp>
#include <BabelWiresLib/Features/rootFeature.hpp>

#include <Tests/BabelWiresLib/TestUtils/testEnvironment.hpp>
#include <Tests/TestUtils/seqTestUtils.hpp>

TEST(MergeProcessorTest, simpleFunction) {
    seqwires::Track trackA;
    testUtils::addSimpleNotes({72, 74, 76, 77}, trackA);

    seqwires::Track trackB;
    testUtils::addSimpleNotes({48, 50, 52, 53, 55, 57}, trackB);

    seqwires::Track track = seqwires::mergeTracks({&trackA, &trackB});
    ASSERT_EQ(track.getDuration(), babelwires::Rational(3, 2));

    std::vector<seqwires::TrackEventHolder> expectedEvents = {
      seqwires::NoteOnEvent{ 0, 72 },
      seqwires::NoteOnEvent{ 0, 48 },
      seqwires::NoteOffEvent{ babelwires::Rational(1, 4), 72 },
      seqwires::NoteOnEvent{ 0, 74 },
      seqwires::NoteOffEvent{ 0, 48 },
      seqwires::NoteOnEvent{ 0, 50 },
      seqwires::NoteOffEvent{ babelwires::Rational(1, 4), 74 },
      seqwires::NoteOnEvent{ 0, 76 },
      seqwires::NoteOffEvent{ 0, 50 },
      seqwires::NoteOnEvent{ 0, 52 },
      seqwires::NoteOffEvent{ babelwires::Rational(1, 4), 76 },
      seqwires::NoteOnEvent{ 0, 77 },
      seqwires::NoteOffEvent{ 0, 52 },
      seqwires::NoteOnEvent{ 0, 53 },
      seqwires::NoteOffEvent{ babelwires::Rational(1, 4), 77 },
      seqwires::NoteOffEvent{ 0, 53 },
      seqwires::NoteOnEvent{ 0, 55 },
      seqwires::NoteOffEvent{ babelwires::Rational(1, 4), 55 },
      seqwires::NoteOnEvent{ 0, 57 },
      seqwires::NoteOffEvent{ babelwires::Rational(1, 4), 57 }
    };

    auto it = track.begin();
    const auto end = track.end();

    for (auto e : expectedEvents) {
        ASSERT_NE(it, end);
        EXPECT_EQ(it->getTimeSinceLastEvent(), e->getTimeSinceLastEvent());
        EXPECT_NE(it->as<seqwires::NoteEvent>(), nullptr);
        EXPECT_EQ((it->as<seqwires::NoteOnEvent>() == nullptr), (e->as<seqwires::NoteOnEvent>() == nullptr));
        EXPECT_EQ((it->as<seqwires::NoteOffEvent>() == nullptr), (e->as<seqwires::NoteOffEvent>() == nullptr));
        EXPECT_EQ(it->as<seqwires::NoteEvent>()->m_pitch, e->as<seqwires::NoteEvent>()->m_pitch);
        EXPECT_EQ(it->as<seqwires::NoteEvent>()->m_velocity, e->as<seqwires::NoteEvent>()->m_velocity);
        ++it;
    }
    EXPECT_EQ(it, end);
}


TEST(MergeProcessorTest, processor) {
    testUtils::TestEnvironment testEnvironment;
    testEnvironment.m_typeSystem.addEntry<seqwires::DefaultTrackType>();

    seqwires::MergeProcessor processor(testEnvironment.m_projectContext);

    processor.getInputFeature()->setToDefault();
    processor.getOutputFeature()->setToDefault();

    auto* inputArray = processor.getInputFeature()->getChildFromStep(babelwires::PathStep("Input")).as<babelwires::ArrayFeature>();
    auto* outputTrack = processor.getOutputFeature()->getChildFromStep(babelwires::PathStep("Output")).as<seqwires::TrackFeature>();
    ASSERT_NE(inputArray, nullptr);
    ASSERT_NE(outputTrack, nullptr);

    EXPECT_EQ(inputArray->getNumFeatures(), 2);
    EXPECT_EQ(outputTrack->get().getDuration(), 0);

    auto getInputTrack = [&inputArray](int i) { return inputArray->getChildFromStep(i).as<seqwires::TrackFeature>(); };

    ASSERT_NE(getInputTrack(0), nullptr);
    ASSERT_NE(getInputTrack(1), nullptr);

    EXPECT_EQ(getInputTrack(0)->get().getDuration(), 0);
    EXPECT_EQ(getInputTrack(1)->get().getDuration(), 0);

    {
        seqwires::Track track;
        testUtils::addSimpleNotes(std::vector<seqwires::Pitch>{72, 74}, track);
        getInputTrack(0)->set(std::move(track));
    }

    processor.process(testEnvironment.m_log);

    testUtils::testSimpleNotes(std::vector<seqwires::Pitch>{72, 74}, outputTrack->get());

    {
        seqwires::Track track;
        testUtils::addSimpleNotes(std::vector<seqwires::Pitch>{48, 50}, track);
        getInputTrack(1)->set(std::move(track));
    }

    processor.process(testEnvironment.m_log);

    std::vector<seqwires::TrackEventHolder> expectedEvents = {
      seqwires::NoteOnEvent{ 0, 72 },
      seqwires::NoteOnEvent{ 0, 48 },
      seqwires::NoteOffEvent{ babelwires::Rational(1, 4), 72 },
      seqwires::NoteOnEvent{ 0, 74 },
      seqwires::NoteOffEvent{ 0, 48 },
      seqwires::NoteOnEvent{ 0, 50 },
      seqwires::NoteOffEvent{ babelwires::Rational(1, 4), 74 },
      seqwires::NoteOffEvent{ 0, 50 },
    };

    {
        auto it = outputTrack->get().begin();
        const auto end = outputTrack->get().end();

        for (auto e : expectedEvents) {
            ASSERT_NE(it, end);
            EXPECT_EQ(it->getTimeSinceLastEvent(), e->getTimeSinceLastEvent());
            EXPECT_NE(it->as<seqwires::NoteEvent>(), nullptr);
            EXPECT_EQ((it->as<seqwires::NoteOnEvent>() == nullptr), (e->as<seqwires::NoteOnEvent>() == nullptr));
            EXPECT_EQ((it->as<seqwires::NoteOffEvent>() == nullptr), (e->as<seqwires::NoteOffEvent>() == nullptr));
            EXPECT_EQ(it->as<seqwires::NoteEvent>()->m_pitch, e->as<seqwires::NoteEvent>()->m_pitch);
            EXPECT_EQ(it->as<seqwires::NoteEvent>()->m_velocity, e->as<seqwires::NoteEvent>()->m_velocity);
            ++it;
        }
        EXPECT_EQ(it, end);
    }

    inputArray->addEntry(1);

    {
        seqwires::Track track;
        testUtils::addSimpleNotes(std::vector<seqwires::Pitch>{60, 62}, track);
        getInputTrack(1)->set(std::move(track));
    }

    processor.process(testEnvironment.m_log);

    expectedEvents = {
      seqwires::NoteOnEvent{ 0, 72 },
      seqwires::NoteOnEvent{ 0, 60 },
      seqwires::NoteOnEvent{ 0, 48 },
      seqwires::NoteOffEvent{ babelwires::Rational(1, 4), 72 },
      seqwires::NoteOnEvent{ 0, 74 },
      seqwires::NoteOffEvent{ 0, 60 },
      seqwires::NoteOnEvent{ 0, 62 },
      seqwires::NoteOffEvent{ 0, 48 },
      seqwires::NoteOnEvent{ 0, 50 },
      seqwires::NoteOffEvent{ babelwires::Rational(1, 4), 74 },
      seqwires::NoteOffEvent{ 0, 62 },
      seqwires::NoteOffEvent{ 0, 50 },
    };

    {
        auto it = outputTrack->get().begin();
        const auto end = outputTrack->get().end();

        for (auto e : expectedEvents) {
            ASSERT_NE(it, end);
            EXPECT_EQ(it->getTimeSinceLastEvent(), e->getTimeSinceLastEvent());
            EXPECT_NE(it->as<seqwires::NoteEvent>(), nullptr);
            EXPECT_EQ((it->as<seqwires::NoteOnEvent>() == nullptr), (e->as<seqwires::NoteOnEvent>() == nullptr));
            EXPECT_EQ((it->as<seqwires::NoteOffEvent>() == nullptr), (e->as<seqwires::NoteOffEvent>() == nullptr));
            EXPECT_EQ(it->as<seqwires::NoteEvent>()->m_pitch, e->as<seqwires::NoteEvent>()->m_pitch);
            EXPECT_EQ(it->as<seqwires::NoteEvent>()->m_velocity, e->as<seqwires::NoteEvent>()->m_velocity);
            ++it;
        }
        EXPECT_EQ(it, end);
    }
}

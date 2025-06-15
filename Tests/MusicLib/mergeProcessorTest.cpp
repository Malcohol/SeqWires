#include <gtest/gtest.h>

#include <BabelWiresLib/ValueTree/valueTreeRoot.hpp>

#include <MusicLib/Functions/mergeFunction.hpp>
#include <MusicLib/Processors/mergeProcessor.hpp>
#include <MusicLib/Types/Track/TrackEvents/noteEvents.hpp>
#include <MusicLib/libRegistration.hpp>

#include <Tests/BabelWiresLib/TestUtils/testEnvironment.hpp>
#include <Tests/TestUtils/seqTestUtils.hpp>

TEST(MergeProcessorTest, simpleFunction) {
    bw_music::Track trackA;
    testUtils::addSimpleNotes({72, 74, 76, 77}, trackA);

    bw_music::Track trackB;
    testUtils::addSimpleNotes({48, 50, 52, 53, 55, 57}, trackB);

    bw_music::Track track = bw_music::mergeTracks({&trackA, &trackB});
    ASSERT_EQ(track.getDuration(), babelwires::Rational(3, 2));

    std::vector<bw_music::TrackEventHolder> expectedEvents = {bw_music::NoteOnEvent{0, 72},
                                                              bw_music::NoteOnEvent{0, 48},
                                                              bw_music::NoteOffEvent{babelwires::Rational(1, 4), 72},
                                                              bw_music::NoteOnEvent{0, 74},
                                                              bw_music::NoteOffEvent{0, 48},
                                                              bw_music::NoteOnEvent{0, 50},
                                                              bw_music::NoteOffEvent{babelwires::Rational(1, 4), 74},
                                                              bw_music::NoteOnEvent{0, 76},
                                                              bw_music::NoteOffEvent{0, 50},
                                                              bw_music::NoteOnEvent{0, 52},
                                                              bw_music::NoteOffEvent{babelwires::Rational(1, 4), 76},
                                                              bw_music::NoteOnEvent{0, 77},
                                                              bw_music::NoteOffEvent{0, 52},
                                                              bw_music::NoteOnEvent{0, 53},
                                                              bw_music::NoteOffEvent{babelwires::Rational(1, 4), 77},
                                                              bw_music::NoteOffEvent{0, 53},
                                                              bw_music::NoteOnEvent{0, 55},
                                                              bw_music::NoteOffEvent{babelwires::Rational(1, 4), 55},
                                                              bw_music::NoteOnEvent{0, 57},
                                                              bw_music::NoteOffEvent{babelwires::Rational(1, 4), 57}};

    auto it = track.begin();
    const auto end = track.end();

    for (auto e : expectedEvents) {
        ASSERT_NE(it, end);
        EXPECT_EQ(it->getTimeSinceLastEvent(), e->getTimeSinceLastEvent());
        EXPECT_NE(it->as<bw_music::NoteEvent>(), nullptr);
        EXPECT_EQ((it->as<bw_music::NoteOnEvent>() == nullptr), (e->as<bw_music::NoteOnEvent>() == nullptr));
        EXPECT_EQ((it->as<bw_music::NoteOffEvent>() == nullptr), (e->as<bw_music::NoteOffEvent>() == nullptr));
        EXPECT_EQ(it->as<bw_music::NoteEvent>()->m_pitch, e->as<bw_music::NoteEvent>()->m_pitch);
        EXPECT_EQ(it->as<bw_music::NoteEvent>()->m_velocity, e->as<bw_music::NoteEvent>()->m_velocity);
        ++it;
    }
    EXPECT_EQ(it, end);
}

TEST(MergeProcessorTest, processor) {
    testUtils::TestEnvironment testEnvironment;
    bw_music::registerLib(testEnvironment.m_projectContext);

    bw_music::MergeProcessor processor(testEnvironment.m_projectContext);

    processor.getInput().setToDefault();
    processor.getOutput().setToDefault();

    auto input = bw_music::MergeProcessorInput::Instance(processor.getInput());
    const auto output =
        bw_music::MergeProcessorOutput::ConstInstance(processor.getOutput());

    ASSERT_EQ(input.getInput().getSize(), 2);
    EXPECT_EQ(input.getInput().getEntry(0).get().getDuration(), 0);
    EXPECT_EQ(input.getInput().getEntry(1).get().getDuration(), 0);

    {
        bw_music::Track track;
        testUtils::addSimpleNotes(std::vector<bw_music::Pitch>{72, 74}, track);
        input.getInput().getEntry(0).set(std::move(track));
    }

    processor.process(testEnvironment.m_log);

    auto outputTrackInstance = output.getOutput();

    testUtils::testSimpleNotes(std::vector<bw_music::Pitch>{72, 74}, outputTrackInstance.get());

    processor.getInput().clearChanges();
    {
        bw_music::Track track;
        testUtils::addSimpleNotes(std::vector<bw_music::Pitch>{48, 50}, track);
        input.getInput().getEntry(1).set(std::move(track));
    }
    processor.process(testEnvironment.m_log);

    std::vector<bw_music::TrackEventHolder> expectedEvents = {
        bw_music::NoteOnEvent{0, 72},
        bw_music::NoteOnEvent{0, 48},
        bw_music::NoteOffEvent{babelwires::Rational(1, 4), 72},
        bw_music::NoteOnEvent{0, 74},
        bw_music::NoteOffEvent{0, 48},
        bw_music::NoteOnEvent{0, 50},
        bw_music::NoteOffEvent{babelwires::Rational(1, 4), 74},
        bw_music::NoteOffEvent{0, 50},
    };

    {
        auto it = outputTrackInstance.get().begin();
        const auto end = outputTrackInstance.get().end();

        for (auto e : expectedEvents) {
            ASSERT_NE(it, end);
            EXPECT_EQ(it->getTimeSinceLastEvent(), e->getTimeSinceLastEvent());
            EXPECT_NE(it->as<bw_music::NoteEvent>(), nullptr);
            EXPECT_EQ((it->as<bw_music::NoteOnEvent>() == nullptr), (e->as<bw_music::NoteOnEvent>() == nullptr));
            EXPECT_EQ((it->as<bw_music::NoteOffEvent>() == nullptr), (e->as<bw_music::NoteOffEvent>() == nullptr));
            EXPECT_EQ(it->as<bw_music::NoteEvent>()->m_pitch, e->as<bw_music::NoteEvent>()->m_pitch);
            EXPECT_EQ(it->as<bw_music::NoteEvent>()->m_velocity, e->as<bw_music::NoteEvent>()->m_velocity);
            ++it;
        }
        EXPECT_EQ(it, end);
    }

    processor.getInput().clearChanges();
    // Insert a new track at position 1.
    {
        input.getInput().setSize(3);
        input.getInput().getEntry(2).set(input.getInput().getEntry(1)->getValue());

        bw_music::Track track;
        testUtils::addSimpleNotes(std::vector<bw_music::Pitch>{60, 62}, track);
        input.getInput().getEntry(1).set(std::move(track));
    }
    processor.process(testEnvironment.m_log);

    expectedEvents = {
        bw_music::NoteOnEvent{0, 72},  bw_music::NoteOnEvent{0, 60},
        bw_music::NoteOnEvent{0, 48},  bw_music::NoteOffEvent{babelwires::Rational(1, 4), 72},
        bw_music::NoteOnEvent{0, 74},  bw_music::NoteOffEvent{0, 60},
        bw_music::NoteOnEvent{0, 62},  bw_music::NoteOffEvent{0, 48},
        bw_music::NoteOnEvent{0, 50},  bw_music::NoteOffEvent{babelwires::Rational(1, 4), 74},
        bw_music::NoteOffEvent{0, 62}, bw_music::NoteOffEvent{0, 50},
    };

    {
        auto it = outputTrackInstance.get().begin();
        const auto end = outputTrackInstance.get().end();

        for (auto e : expectedEvents) {
            ASSERT_NE(it, end);
            EXPECT_EQ(it->getTimeSinceLastEvent(), e->getTimeSinceLastEvent());
            EXPECT_NE(it->as<bw_music::NoteEvent>(), nullptr);
            EXPECT_EQ((it->as<bw_music::NoteOnEvent>() == nullptr), (e->as<bw_music::NoteOnEvent>() == nullptr));
            EXPECT_EQ((it->as<bw_music::NoteOffEvent>() == nullptr), (e->as<bw_music::NoteOffEvent>() == nullptr));
            EXPECT_EQ(it->as<bw_music::NoteEvent>()->m_pitch, e->as<bw_music::NoteEvent>()->m_pitch);
            EXPECT_EQ(it->as<bw_music::NoteEvent>()->m_velocity, e->as<bw_music::NoteEvent>()->m_velocity);
            ++it;
        }
        EXPECT_EQ(it, end);
    }
}

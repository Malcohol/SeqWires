#include <gtest/gtest.h>

#include <SeqWiresLib/Functions/excerptFunction.hpp>
#include <SeqWiresLib/Tracks/noteEvents.hpp>

#include <Tests/TestUtils/seqTestUtils.hpp>

TEST(ExcerptProcessorTest, funcSimple) {
    seqwires::Track trackIn;

    testUtils::addSimpleNotes(std::vector<seqwires::Pitch>{60, 62, 64, 65, 67, 69, 71, 72}, trackIn);

    auto trackOut = seqwires::getTrackExcerpt(trackIn, babelwires::Rational(1, 2), 1);

    testUtils::testSimpleNotes(std::vector<seqwires::Pitch>{64, 65, 67, 69}, *trackOut);
}

TEST(ExcerptProcessorTest, funcEmptyBefore) {
    seqwires::Track trackIn;
    seqwires::NoteOnEvent note;
    note.m_pitch = 64;
    note.m_velocity = 100;
    note.setTimeSinceLastEvent(10);
    trackIn.addEvent(note);

    auto trackOut = seqwires::getTrackExcerpt(trackIn, 3, 4);

    EXPECT_EQ(trackOut->getDuration(), 4);
    EXPECT_EQ(trackOut->getNumEvents(), 0);
}

TEST(ExcerptProcessorTest, funcEmptyAfter) {
    seqwires::Track trackIn;

    auto trackOut = seqwires::getTrackExcerpt(trackIn, 2, 2);

    EXPECT_EQ(trackOut->getDuration(), 2);
    EXPECT_EQ(trackOut->getNumEvents(), 0);
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

    EXPECT_EQ(trackOut->getDuration(), 2);
    EXPECT_EQ(trackOut->getNumEvents(), 0);
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

    testUtils::testSimpleNotes(std::vector<seqwires::Pitch>{64, 65, 67, 69}, *trackOut);
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

    testUtils::testSimpleNotes(std::vector<seqwires::Pitch>{64, 65, 67, 69}, *trackOut);
}

TEST(ExcerptProcessorTest, funcGaps) {
    seqwires::Track trackIn;

    testUtils::addSimpleNotes(std::vector<seqwires::Pitch>{60, 62, 64, 65, 67, 69, 71, 72}, trackIn);

    auto trackOut = seqwires::getTrackExcerpt(trackIn, babelwires::Rational(3, 8), 1);

    auto noteIterator = trackOut->begin();
    const auto endIterator = trackOut->end();

    {
        EXPECT_NE(noteIterator, endIterator);
        auto noteOn = noteIterator->asA<const seqwires::NoteOnEvent>();
        ASSERT_NE(noteOn, nullptr);
        EXPECT_EQ(noteOn->getTimeSinceLastEvent(), babelwires::Rational(1, 8));
        EXPECT_EQ(noteOn->m_pitch, 64);
        EXPECT_EQ(noteOn->m_velocity, 127);
        ++noteIterator;
    }

    // Pitches
    std::vector<seqwires::Pitch> middlePitches{64, 65, 67, 69};

    for (int i = 0; i < middlePitches.size() - 1; ++i)
    {
        EXPECT_NE(noteIterator, endIterator);
        auto noteOff = noteIterator->asA<seqwires::NoteOffEvent>();
        ASSERT_NE(noteOff, nullptr);
        EXPECT_EQ(noteOff->getTimeSinceLastEvent(), babelwires::Rational(1, 4));
        EXPECT_EQ(noteOff->m_pitch, middlePitches[i]);
        EXPECT_EQ(noteOff->m_velocity, 64);
        ++noteIterator;

        EXPECT_NE(noteIterator, endIterator);
        auto noteOn = noteIterator->asA<const seqwires::NoteOnEvent>();
        ASSERT_NE(noteOn, nullptr);
        EXPECT_EQ(noteOn->getTimeSinceLastEvent(), 0);
        EXPECT_EQ(noteOn->m_pitch, middlePitches[i + 1]);
        EXPECT_EQ(noteOn->m_velocity, 127);
        ++noteIterator;
    }
    {
        EXPECT_NE(noteIterator, endIterator);
        auto noteOff = noteIterator->asA<seqwires::NoteOffEvent>();
        ASSERT_NE(noteOff, nullptr);
        EXPECT_EQ(noteOff->getTimeSinceLastEvent(), babelwires::Rational(1, 8));
        EXPECT_EQ(noteOff->m_pitch, 69);
        EXPECT_EQ(noteOff->m_velocity, 64);
        ++noteIterator;
    }
    EXPECT_EQ(noteIterator, endIterator);
}


#include <gtest/gtest.h>

#include <SeqWiresLib/Functions/excerptFunction.hpp>
#include <SeqWiresLib/Tracks/noteEvents.hpp>

#include <Tests/TestUtils/seqTestUtils.hpp>

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


#include <gtest/gtest.h>

#include <SeqWiresLib/Features/trackFeature.hpp>
#include <SeqWiresLib/Features/pitchFeature.hpp>
#include <SeqWiresLib/Functions/mergeFunction.hpp>
#include <SeqWiresLib/Functions/splitAtPitchFunction.hpp>
#include <SeqWiresLib/Processors/splitAtPitchProcessor.hpp>
#include <SeqWiresLib/Tracks/chordEvents.hpp>
#include <SeqWiresLib/Tracks/noteEvents.hpp>
#include <SeqWiresLib/Tracks/trackEventHolder.hpp>

#include <BabelWiresLib/Features/arrayFeature.hpp>

#include <Tests/TestUtils/seqTestUtils.hpp>
#include <Tests/TestUtils/testLog.hpp>

TEST(SplitAtPitchProcessorTest, monophonicSplit) {
    seqwires::Track track;
    testUtils::addSimpleNotes({60, 62, 64, 65, 67, 69, 71, 72}, track);

    seqwires::SplitAtPitchResult result = seqwires::splitAtPitch(67, track);

    const std::vector<testUtils::NoteInfo> expectedNotesAbove{
        {67, 1, babelwires::Rational(1, 4)},
        {69, 0, babelwires::Rational(1, 4)},
        {71, 0, babelwires::Rational(1, 4)},
        {72, 0, babelwires::Rational(1, 4)},
    };
    const std::vector<testUtils::NoteInfo> expectedNotesBelow{
        {60, 0, babelwires::Rational(1, 4)},
        {62, 0, babelwires::Rational(1, 4)},
        {64, 0, babelwires::Rational(1, 4)},
        {65, 0, babelwires::Rational(1, 4)},
    };

    testUtils::testNotes(expectedNotesAbove, result.m_equalOrAbove);
    EXPECT_EQ(result.m_equalOrAbove.getDuration(), 2);
    testUtils::testNotes(expectedNotesBelow, result.m_below);
    EXPECT_EQ(result.m_below.getDuration(), 2);
    EXPECT_EQ(result.m_other.getNumEvents(), 0);
    EXPECT_EQ(result.m_other.getDuration(), 2);
}

TEST(SplitAtPitchProcessorTest, aboveAndBelowSplit) {
    seqwires::Track track;
    track.addEvent(seqwires::NoteOnEvent{0, 72});
    track.addEvent(seqwires::NoteOnEvent{0, 48});
    track.addEvent(
        seqwires::ChordOnEvent{0, seqwires::PitchClass::PITCH_CLASS_C, seqwires::ChordType::CHORD_TYPE_Maj});
    track.addEvent(seqwires::NoteOffEvent{babelwires::Rational(1, 4), 72});
    track.addEvent(seqwires::NoteOffEvent{0, 48});
    track.addEvent(seqwires::NoteOnEvent{0, 74});
    track.addEvent(seqwires::NoteOnEvent{0, 50});
    track.addEvent(seqwires::NoteOffEvent{babelwires::Rational(1, 4), 74});
    track.addEvent(seqwires::NoteOffEvent{0, 50});
    track.addEvent(seqwires::ChordOffEvent{0});
    track.addEvent(seqwires::NoteOnEvent{0, 76});
    track.addEvent(seqwires::NoteOnEvent{0, 52});
    track.addEvent(
        seqwires::ChordOnEvent{0, seqwires::PitchClass::PITCH_CLASS_D, seqwires::ChordType::CHORD_TYPE_min});
    track.addEvent(seqwires::NoteOffEvent{babelwires::Rational(1, 4), 76});
    track.addEvent(seqwires::NoteOffEvent{0, 52});
    track.addEvent(seqwires::NoteOnEvent{0, 77});
    track.addEvent(seqwires::NoteOnEvent{0, 53});
    track.addEvent(seqwires::NoteOffEvent{babelwires::Rational(1, 4), 77});
    track.addEvent(seqwires::NoteOffEvent{0, 53});
    track.addEvent(seqwires::ChordOffEvent{0});
    ASSERT_EQ(track.getDuration(), 1);

    seqwires::SplitAtPitchResult result = seqwires::splitAtPitch(60, track);

    testUtils::testSimpleNotes({72, 74, 76, 77}, result.m_equalOrAbove);
    EXPECT_EQ(result.m_equalOrAbove.getDuration(), 1);
    testUtils::testSimpleNotes({48, 50, 52, 53}, result.m_below);
    EXPECT_EQ(result.m_below.getDuration(), 1);
    testUtils::testChords({{seqwires::PitchClass::PITCH_CLASS_C, seqwires::ChordType::CHORD_TYPE_Maj},
                           {seqwires::PitchClass::PITCH_CLASS_D, seqwires::ChordType::CHORD_TYPE_min}},
                          result.m_other);
    EXPECT_EQ(result.m_other.getDuration(), 1);
}

TEST(SplitAtPitchProcessorTest, processor) {
    testUtils::TestLog log;

    seqwires::SplitAtPitchProcessor processor;

    processor.getInputFeature()->setToDefault();
    processor.getOutputFeature()->setToDefault();

    auto* pitchFeature = processor.getInputFeature()->getChildFromStep(babelwires::PathStep("Pitch")).as<seqwires::PitchFeature>();
    auto* inputTrack = processor.getInputFeature()->getChildFromStep(babelwires::PathStep("Input")).as<seqwires::TrackFeature>();
    auto* aboveTrack = processor.getOutputFeature()->getChildFromStep(babelwires::PathStep("Above")).as<seqwires::TrackFeature>();
    auto* belowTrack = processor.getOutputFeature()->getChildFromStep(babelwires::PathStep("Below")).as<seqwires::TrackFeature>();
    auto* otherTrack = processor.getOutputFeature()->getChildFromStep(babelwires::PathStep("Other")).as<seqwires::TrackFeature>();
    ASSERT_NE(pitchFeature, nullptr);
    ASSERT_NE(inputTrack, nullptr);
    ASSERT_NE(aboveTrack, nullptr);
    ASSERT_NE(belowTrack, nullptr);
    ASSERT_NE(otherTrack, nullptr);

    pitchFeature->set(67);
    {
        seqwires::Track track;
        testUtils::addSimpleNotes({60, 62, 64, 65, 67, 69, 71, 72}, track);
        inputTrack->set(std::move(track));
    }
    processor.process(log);    

    std::vector<testUtils::NoteInfo> expectedNotesAbove{
        {67, 1, babelwires::Rational(1, 4)},
        {69, 0, babelwires::Rational(1, 4)},
        {71, 0, babelwires::Rational(1, 4)},
        {72, 0, babelwires::Rational(1, 4)},
    };
    std::vector<testUtils::NoteInfo> expectedNotesBelow{
        {60, 0, babelwires::Rational(1, 4)},
        {62, 0, babelwires::Rational(1, 4)},
        {64, 0, babelwires::Rational(1, 4)},
        {65, 0, babelwires::Rational(1, 4)},
    };

    testUtils::testNotes(expectedNotesAbove, aboveTrack->get());
    EXPECT_EQ(aboveTrack->get().getDuration(), 2);
    testUtils::testNotes(expectedNotesBelow, belowTrack->get());
    EXPECT_EQ(belowTrack->get().getDuration(), 2);
    EXPECT_EQ(otherTrack->get().getNumEvents(), 0);
    EXPECT_EQ(otherTrack->get().getDuration(), 2);

    processor.getInputFeature()->clearChanges();
    pitchFeature->set(64);
    processor.process(log);    

    expectedNotesAbove = {
        {64, babelwires::Rational(1, 2), babelwires::Rational(1, 4)},
        {65, 0, babelwires::Rational(1, 4)},
        {67, 0, babelwires::Rational(1, 4)},
        {69, 0, babelwires::Rational(1, 4)},
        {71, 0, babelwires::Rational(1, 4)},
        {72, 0, babelwires::Rational(1, 4)},
    };
    expectedNotesBelow = {
        {60, 0, babelwires::Rational(1, 4)},
        {62, 0, babelwires::Rational(1, 4)},
    };

    testUtils::testNotes(expectedNotesAbove, aboveTrack->get());
    EXPECT_EQ(aboveTrack->get().getDuration(), 2);
    testUtils::testNotes(expectedNotesBelow, belowTrack->get());
    EXPECT_EQ(belowTrack->get().getDuration(), 2);
    EXPECT_EQ(otherTrack->get().getNumEvents(), 0);
    EXPECT_EQ(otherTrack->get().getDuration(), 2);
}

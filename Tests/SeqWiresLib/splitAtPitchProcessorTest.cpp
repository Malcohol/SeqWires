#include <gtest/gtest.h>

#include <SeqWiresLib/Types/Track/trackFeature.hpp>
#include <SeqWiresLib/Functions/mergeFunction.hpp>
#include <SeqWiresLib/Functions/splitAtPitchFunction.hpp>
#include <SeqWiresLib/Processors/splitAtPitchProcessor.hpp>
#include <SeqWiresLib/Types/Track/TrackEvents/chordEvents.hpp>
#include <SeqWiresLib/Types/Track/TrackEvents/noteEvents.hpp>
#include <SeqWiresLib/Types/Track/TrackEvents/trackEventHolder.hpp>
#include <SeqWiresLib/libRegistration.hpp>

#include <BabelWiresLib/Features/arrayFeature.hpp>
#include <BabelWiresLib/Features/rootFeature.hpp>

#include <Tests/TestUtils/seqTestUtils.hpp>
#include <Tests/BabelWiresLib/TestUtils/testEnvironment.hpp>

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
        seqwires::ChordOnEvent{0, {seqwires::PitchClass::PitchClass::Value::C, seqwires::ChordType::ChordType::Value::M}});
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
        seqwires::ChordOnEvent{0, {seqwires::PitchClass::PitchClass::Value::D, seqwires::ChordType::ChordType::Value::m}});
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
    testUtils::testChords({{seqwires::PitchClass::PitchClass::Value::C, seqwires::ChordType::ChordType::Value::M},
                           {seqwires::PitchClass::PitchClass::Value::D, seqwires::ChordType::ChordType::Value::m}},
                          result.m_other);
    EXPECT_EQ(result.m_other.getDuration(), 1);
}

TEST(SplitAtPitchProcessorTest, processor) {
    testUtils::TestEnvironment testEnvironment;
    seqwires::registerLib(testEnvironment.m_projectContext);

    seqwires::SplitAtPitchProcessor processor(testEnvironment.m_projectContext);

    processor.getInputFeature()->setToDefault();
    processor.getOutputFeature()->setToDefault();

    auto* pitchFeature = processor.getInputFeature()->getChildFromStep(babelwires::PathStep("Pitch")).as<babelwires::EnumFeature>();
    auto* inputTrack = processor.getInputFeature()->getChildFromStep(babelwires::PathStep("Input")).as<seqwires::TrackFeature>();
    auto* aboveTrack = processor.getOutputFeature()->getChildFromStep(babelwires::PathStep("Above")).as<seqwires::TrackFeature>();
    auto* belowTrack = processor.getOutputFeature()->getChildFromStep(babelwires::PathStep("Below")).as<seqwires::TrackFeature>();
    auto* otherTrack = processor.getOutputFeature()->getChildFromStep(babelwires::PathStep("Other")).as<seqwires::TrackFeature>();
    ASSERT_NE(pitchFeature, nullptr);
    ASSERT_NE(inputTrack, nullptr);
    ASSERT_NE(aboveTrack, nullptr);
    ASSERT_NE(belowTrack, nullptr);
    ASSERT_NE(otherTrack, nullptr);

    pitchFeature->setToEnumIndex(67);
    {
        seqwires::Track track;
        testUtils::addSimpleNotes({60, 62, 64, 65, 67, 69, 71, 72}, track);
        inputTrack->set(std::move(track));
    }
    processor.process(testEnvironment.m_log);    

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
    pitchFeature->setToEnumIndex(64);
    processor.process(testEnvironment.m_log); 

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

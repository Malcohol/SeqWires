#include <gtest/gtest.h>

#include <BabelWiresLib/ValueTree/valueTreeRoot.hpp>

#include <MusicLib/Functions/monophonicSubtracksFunction.hpp>
#include <MusicLib/Processors/monophonicSubtracksProcessor.hpp>
#include <MusicLib/Types/Track/TrackEvents/chordEvents.hpp>
#include <MusicLib/Types/Track/TrackEvents/noteEvents.hpp>
#include <MusicLib/Types/Track/TrackEvents/trackEventHolder.hpp>
#include <MusicLib/libRegistration.hpp>

#include <Tests/BabelWiresLib/TestUtils/testEnvironment.hpp>
#include <Tests/TestUtils/seqTestUtils.hpp>
#include <Tests/TestUtils/testLog.hpp>

namespace {
    seqwires::Track getSamplePolyphonicTrack() {
        seqwires::Track track;
        track.addEvent(seqwires::NoteOnEvent{0, 72});
        track.addEvent(seqwires::NoteOnEvent{0, 48});
        track.addEvent(
            seqwires::ChordOnEvent{0, {seqwires::PitchClass::Value::C, seqwires::ChordType::ChordType::Value::M}});
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
            seqwires::ChordOnEvent{0, {seqwires::PitchClass::Value::D, seqwires::ChordType::ChordType::Value::m}});
        track.addEvent(seqwires::NoteOffEvent{babelwires::Rational(1, 4), 76});
        track.addEvent(seqwires::NoteOffEvent{0, 52});
        track.addEvent(seqwires::NoteOnEvent{0, 77});
        track.addEvent(seqwires::NoteOnEvent{0, 53});
        track.addEvent(seqwires::NoteOffEvent{babelwires::Rational(1, 4), 77});
        track.addEvent(seqwires::NoteOffEvent{0, 53});
        track.addEvent(seqwires::ChordOffEvent{0});
        return track;
    }

    seqwires::Track getStaggeredPolyphonicTrack() {
        seqwires::Track track;
        track.addEvent(
            seqwires::ChordOnEvent{0, {seqwires::PitchClass::Value::C, seqwires::ChordType::ChordType::Value::M}});
        track.addEvent(seqwires::NoteOnEvent{0, 48});
        track.addEvent(seqwires::NoteOnEvent{babelwires::Rational(1, 4), 60});
        track.addEvent(seqwires::NoteOnEvent{babelwires::Rational(1, 4), 72});
        track.addEvent(seqwires::NoteOffEvent{babelwires::Rational(1, 4), 48});
        track.addEvent(seqwires::NoteOnEvent{0, 50});
        track.addEvent(seqwires::NoteOffEvent{babelwires::Rational(1, 4), 60});
        track.addEvent(seqwires::NoteOnEvent{0, 62});
        track.addEvent(seqwires::NoteOffEvent{babelwires::Rational(1, 4), 72});
        track.addEvent(seqwires::NoteOnEvent{0, 74});
        track.addEvent(seqwires::ChordOffEvent{0});
        track.addEvent(
            seqwires::ChordOnEvent{0, {seqwires::PitchClass::Value::D, seqwires::ChordType::ChordType::Value::m}});
        track.addEvent(seqwires::NoteOffEvent{babelwires::Rational(1, 4), 50});
        track.addEvent(seqwires::NoteOnEvent{0, 48});
        track.addEvent(seqwires::NoteOffEvent{babelwires::Rational(1, 4), 62});
        track.addEvent(seqwires::NoteOnEvent{0, 60});
        track.addEvent(seqwires::NoteOffEvent{babelwires::Rational(1, 4), 74});
        track.addEvent(seqwires::NoteOnEvent{0, 72});
        track.addEvent(seqwires::NoteOffEvent{babelwires::Rational(1, 4), 48});
        track.addEvent(seqwires::NoteOffEvent{babelwires::Rational(1, 4), 60});
        track.addEvent(seqwires::NoteOffEvent{babelwires::Rational(1, 4), 72});
        track.addEvent(seqwires::ChordOffEvent{0});
        return track;
    }

    seqwires::Track getStaggeredPolyphonicTrack2() {
        seqwires::Track track;
        track.addEvent(
            seqwires::ChordOnEvent{0, {seqwires::PitchClass::Value::C, seqwires::ChordType::ChordType::Value::M}});
        track.addEvent(seqwires::NoteOnEvent{0, 72});
        track.addEvent(seqwires::NoteOnEvent{babelwires::Rational(1, 4), 60});
        track.addEvent(seqwires::NoteOnEvent{babelwires::Rational(1, 4), 48});
        track.addEvent(seqwires::NoteOffEvent{babelwires::Rational(1, 4), 72});
        track.addEvent(seqwires::NoteOnEvent{0, 74});
        track.addEvent(seqwires::NoteOffEvent{babelwires::Rational(1, 4), 60});
        track.addEvent(seqwires::NoteOnEvent{0, 62});
        track.addEvent(seqwires::NoteOffEvent{babelwires::Rational(1, 4), 48});
        track.addEvent(seqwires::NoteOnEvent{0, 50});
        track.addEvent(seqwires::ChordOffEvent{0});
        track.addEvent(
            seqwires::ChordOnEvent{0, {seqwires::PitchClass::Value::D, seqwires::ChordType::ChordType::Value::m}});
        track.addEvent(seqwires::NoteOffEvent{babelwires::Rational(1, 4), 74});
        track.addEvent(seqwires::NoteOnEvent{0, 72});
        track.addEvent(seqwires::NoteOffEvent{babelwires::Rational(1, 4), 62});
        track.addEvent(seqwires::NoteOnEvent{0, 60});
        track.addEvent(seqwires::NoteOffEvent{babelwires::Rational(1, 4), 50});
        track.addEvent(seqwires::NoteOnEvent{0, 48});
        track.addEvent(seqwires::NoteOffEvent{babelwires::Rational(1, 4), 72});
        track.addEvent(seqwires::NoteOffEvent{babelwires::Rational(1, 4), 60});
        track.addEvent(seqwires::NoteOffEvent{babelwires::Rational(1, 4), 48});
        track.addEvent(seqwires::ChordOffEvent{0});
        return track;
    }
} // namespace

TEST(MonophonicSubtracksProcessorTest, simpleFunction) {
    seqwires::Track track = getSamplePolyphonicTrack();

    seqwires::MonophonicSubtracksResult result =
        seqwires::getMonophonicSubtracks(track, 2, seqwires::MonophonicSubtracksPolicyEnum::Value::High);

    ASSERT_EQ(result.m_noteTracks.size(), 2);
    EXPECT_EQ(result.m_noteTracks[0].getDuration(), 1);
    EXPECT_EQ(result.m_noteTracks[1].getDuration(), 1);
    EXPECT_EQ(result.m_other.getDuration(), 1);

    testUtils::testSimpleNotes({72, 74, 76, 77}, result.m_noteTracks[0]);
    testUtils::testSimpleNotes({48, 50, 52, 53}, result.m_noteTracks[1]);
    testUtils::testChords({{seqwires::PitchClass::Value::C, seqwires::ChordType::ChordType::Value::M},
                           {seqwires::PitchClass::Value::D, seqwires::ChordType::ChordType::Value::m}},
                          result.m_other);
}

TEST(MonophonicSubtracksProcessorTest, FunctionLower) {
    seqwires::Track track = getSamplePolyphonicTrack();

    seqwires::MonophonicSubtracksResult result =
        seqwires::getMonophonicSubtracks(track, 2, seqwires::MonophonicSubtracksPolicyEnum::Value::Low);

    ASSERT_EQ(result.m_noteTracks.size(), 2);
    EXPECT_EQ(result.m_noteTracks[0].getDuration(), 1);
    EXPECT_EQ(result.m_noteTracks[1].getDuration(), 1);
    EXPECT_EQ(result.m_other.getDuration(), 1);

    testUtils::testSimpleNotes({48, 50, 52, 53}, result.m_noteTracks[0]);
    testUtils::testSimpleNotes({72, 74, 76, 77}, result.m_noteTracks[1]);
    testUtils::testChords({{seqwires::PitchClass::Value::C, seqwires::ChordType::ChordType::Value::M},
                           {seqwires::PitchClass::Value::D, seqwires::ChordType::ChordType::Value::m}},
                          result.m_other);
}

TEST(MonophonicSubtracksProcessorTest, redundantTracks) {
    seqwires::Track track = getSamplePolyphonicTrack();

    seqwires::MonophonicSubtracksResult result =
        seqwires::getMonophonicSubtracks(track, 3, seqwires::MonophonicSubtracksPolicyEnum::Value::High);

    ASSERT_EQ(result.m_noteTracks.size(), 3);
    EXPECT_EQ(result.m_noteTracks[0].getDuration(), 1);
    EXPECT_EQ(result.m_noteTracks[1].getDuration(), 1);
    EXPECT_EQ(result.m_noteTracks[2].getDuration(), 1);
    EXPECT_EQ(result.m_other.getDuration(), 1);

    testUtils::testSimpleNotes({72, 74, 76, 77}, result.m_noteTracks[0]);
    testUtils::testSimpleNotes({48, 50, 52, 53}, result.m_noteTracks[1]);
    EXPECT_EQ(result.m_noteTracks[2].getNumEvents(), 0);
    testUtils::testChords({{seqwires::PitchClass::Value::C, seqwires::ChordType::ChordType::Value::M},
                           {seqwires::PitchClass::Value::D, seqwires::ChordType::ChordType::Value::m}},
                          result.m_other);
}

TEST(MonophonicSubtracksProcessorTest, eventToOther) {
    seqwires::Track track = getSamplePolyphonicTrack();

    seqwires::MonophonicSubtracksResult result =
        seqwires::getMonophonicSubtracks(track, 1, seqwires::MonophonicSubtracksPolicyEnum::Value::High);

    ASSERT_EQ(result.m_noteTracks.size(), 1);
    EXPECT_EQ(result.m_noteTracks[0].getDuration(), 1);
    EXPECT_EQ(result.m_other.getDuration(), 1);

    testUtils::testSimpleNotes({72, 74, 76, 77}, result.m_noteTracks[0]);

    const std::vector<seqwires::TrackEventHolder> expectedEvents = {
        seqwires::ChordOnEvent{0, {seqwires::PitchClass::Value::C, seqwires::ChordType::ChordType::Value::M}},
        seqwires::NoteOnEvent{0, 48},
        seqwires::NoteOffEvent{babelwires::Rational(1, 4), 48},
        seqwires::NoteOnEvent{0, 50},
        seqwires::ChordOffEvent{babelwires::Rational(1, 4)},
        seqwires::ChordOnEvent{0, {seqwires::PitchClass::Value::D, seqwires::ChordType::ChordType::Value::m}},
        seqwires::NoteOffEvent{0, 50},
        seqwires::NoteOnEvent{0, 52},
        seqwires::NoteOffEvent{babelwires::Rational(1, 4), 52},
        seqwires::NoteOnEvent{0, 53},
        seqwires::ChordOffEvent{babelwires::Rational(1, 4)},
        seqwires::NoteOffEvent{0, 53}};

    testUtils::testNotesAndChords(expectedEvents, result.m_other);
}

TEST(MonophonicSubtracksProcessorTest, higherPitchesEvictOneTrack) {
    seqwires::Track track = getStaggeredPolyphonicTrack();

    seqwires::MonophonicSubtracksResult result =
        seqwires::getMonophonicSubtracks(track, 1, seqwires::MonophonicSubtracksPolicyEnum::Value::HighEv);

    ASSERT_EQ(result.m_noteTracks.size(), 1);
    EXPECT_EQ(result.m_noteTracks[0].getDuration(), babelwires::Rational(11, 4));
    EXPECT_EQ(result.m_other.getDuration(), babelwires::Rational(11, 4));

    const std::vector<seqwires::TrackEventHolder> monoEvents = {
        seqwires::NoteOnEvent{0, 48}, seqwires::NoteOffEvent{babelwires::Rational(1, 4), 48},
        seqwires::NoteOnEvent{0, 60}, seqwires::NoteOffEvent{babelwires::Rational(1, 4), 60},
        seqwires::NoteOnEvent{0, 72}, seqwires::NoteOffEvent{babelwires::Rational(3, 4), 72},
        seqwires::NoteOnEvent{0, 74}, seqwires::NoteOffEvent{babelwires::Rational(3, 4), 74},
        seqwires::NoteOnEvent{0, 72}, seqwires::NoteOffEvent{babelwires::Rational(3, 4), 72},
    };

    testUtils::testNotesAndChords(monoEvents, result.m_noteTracks[0]);

    const std::vector<seqwires::TrackEventHolder> otherEvents = {
        seqwires::ChordOnEvent{0, {seqwires::PitchClass::Value::C, seqwires::ChordType::ChordType::Value::M}},
        seqwires::NoteOnEvent{babelwires::Rational(3, 4), 50},
        seqwires::NoteOnEvent{babelwires::Rational(1, 4), 62},
        seqwires::ChordOffEvent{babelwires::Rational(1, 4)},
        seqwires::ChordOnEvent{0, {seqwires::PitchClass::Value::D, seqwires::ChordType::ChordType::Value::m}},
        seqwires::NoteOffEvent{babelwires::Rational(1, 4), 50},
        seqwires::NoteOnEvent{0, 48},
        seqwires::NoteOffEvent{babelwires::Rational(1, 4), 62},
        seqwires::NoteOnEvent{0, 60},
        seqwires::NoteOffEvent{babelwires::Rational(1, 2), 48},
        seqwires::NoteOffEvent{babelwires::Rational(1, 4), 60},
        seqwires::ChordOffEvent{babelwires::Rational(1, 4)},
    };

    testUtils::testNotesAndChords(otherEvents, result.m_other);
}

TEST(MonophonicSubtracksProcessorTest, higherPitchesEvictTwoTracks) {
    seqwires::Track track = getStaggeredPolyphonicTrack();

    seqwires::MonophonicSubtracksResult result =
        seqwires::getMonophonicSubtracks(track, 2, seqwires::MonophonicSubtracksPolicyEnum::Value::HighEv);

    ASSERT_EQ(result.m_noteTracks.size(), 2);
    EXPECT_EQ(result.m_noteTracks[0].getDuration(), babelwires::Rational(11, 4));
    EXPECT_EQ(result.m_noteTracks[1].getDuration(), babelwires::Rational(11, 4));
    EXPECT_EQ(result.m_other.getDuration(), babelwires::Rational(11, 4));

    const std::vector<seqwires::TrackEventHolder> monoEvents0 = {
        seqwires::NoteOnEvent{0, 48}, seqwires::NoteOffEvent{babelwires::Rational(1, 2), 48},
        seqwires::NoteOnEvent{0, 72}, seqwires::NoteOffEvent{babelwires::Rational(3, 4), 72},
        seqwires::NoteOnEvent{0, 74}, seqwires::NoteOffEvent{babelwires::Rational(3, 4), 74},
        seqwires::NoteOnEvent{0, 72}, seqwires::NoteOffEvent{babelwires::Rational(3, 4), 72},
    };

    testUtils::testNotesAndChords(monoEvents0, result.m_noteTracks[0]);

    const std::vector<seqwires::TrackEventHolder> monoEvents1 = {
        seqwires::NoteOnEvent{babelwires::Rational(1, 4), 60},
        seqwires::NoteOffEvent{babelwires::Rational(3, 4), 60},
        seqwires::NoteOnEvent{0, 62},
        seqwires::NoteOffEvent{babelwires::Rational(3, 4), 62},
        seqwires::NoteOnEvent{0, 60},
        seqwires::NoteOffEvent{babelwires::Rational(3, 4), 60}};

    testUtils::testNotesAndChords(monoEvents1, result.m_noteTracks[1]);

    const std::vector<seqwires::TrackEventHolder> otherEvents = {
        seqwires::ChordOnEvent{0, {seqwires::PitchClass::Value::C, seqwires::ChordType::ChordType::Value::M}},
        seqwires::NoteOnEvent{babelwires::Rational(3, 4), 50},
        seqwires::ChordOffEvent{babelwires::Rational(1, 2)},
        seqwires::ChordOnEvent{0, {seqwires::PitchClass::Value::D, seqwires::ChordType::ChordType::Value::m}},
        seqwires::NoteOffEvent{babelwires::Rational(1, 4), 50},
        seqwires::NoteOnEvent{0, 48},
        seqwires::NoteOffEvent{babelwires::Rational(3, 4), 48},
        seqwires::ChordOffEvent{babelwires::Rational(1, 2)},
    };

    testUtils::testNotesAndChords(otherEvents, result.m_other);
}

TEST(MonophonicSubtracksProcessorTest, lowerPitchesEvictOneTrack) {
    seqwires::Track track = getStaggeredPolyphonicTrack2();

    seqwires::MonophonicSubtracksResult result =
        seqwires::getMonophonicSubtracks(track, 1, seqwires::MonophonicSubtracksPolicyEnum::Value::LowEv);

    ASSERT_EQ(result.m_noteTracks.size(), 1);
    EXPECT_EQ(result.m_noteTracks[0].getDuration(), babelwires::Rational(11, 4));
    EXPECT_EQ(result.m_other.getDuration(), babelwires::Rational(11, 4));

    const std::vector<seqwires::TrackEventHolder> monoEvents = {
        seqwires::NoteOnEvent{0, 72}, seqwires::NoteOffEvent{babelwires::Rational(1, 4), 72},
        seqwires::NoteOnEvent{0, 60}, seqwires::NoteOffEvent{babelwires::Rational(1, 4), 60},
        seqwires::NoteOnEvent{0, 48}, seqwires::NoteOffEvent{babelwires::Rational(3, 4), 48},
        seqwires::NoteOnEvent{0, 50}, seqwires::NoteOffEvent{babelwires::Rational(3, 4), 50},
        seqwires::NoteOnEvent{0, 48}, seqwires::NoteOffEvent{babelwires::Rational(3, 4), 48},
    };

    testUtils::testNotesAndChords(monoEvents, result.m_noteTracks[0]);

    const std::vector<seqwires::TrackEventHolder> otherEvents = {
        seqwires::ChordOnEvent{0, {seqwires::PitchClass::Value::C, seqwires::ChordType::ChordType::Value::M}},
        seqwires::NoteOnEvent{babelwires::Rational(3, 4), 74},
        seqwires::NoteOnEvent{babelwires::Rational(1, 4), 62},
        seqwires::ChordOffEvent{babelwires::Rational(1, 4)},
        seqwires::ChordOnEvent{0, {seqwires::PitchClass::Value::D, seqwires::ChordType::ChordType::Value::m}},
        seqwires::NoteOffEvent{babelwires::Rational(1, 4), 74},
        seqwires::NoteOnEvent{0, 72},
        seqwires::NoteOffEvent{babelwires::Rational(1, 4), 62},
        seqwires::NoteOnEvent{0, 60},
        seqwires::NoteOffEvent{babelwires::Rational(1, 2), 72},
        seqwires::NoteOffEvent{babelwires::Rational(1, 4), 60},
        seqwires::ChordOffEvent{babelwires::Rational(1, 4)},
    };

    testUtils::testNotesAndChords(otherEvents, result.m_other);
}

TEST(MonophonicSubtracksProcessorTest, lowerPitchesEvictTwoTracks) {
    seqwires::Track track = getStaggeredPolyphonicTrack2();

    seqwires::MonophonicSubtracksResult result =
        seqwires::getMonophonicSubtracks(track, 2, seqwires::MonophonicSubtracksPolicyEnum::Value::LowEv);

    ASSERT_EQ(result.m_noteTracks.size(), 2);
    EXPECT_EQ(result.m_noteTracks[0].getDuration(), babelwires::Rational(11, 4));
    EXPECT_EQ(result.m_noteTracks[1].getDuration(), babelwires::Rational(11, 4));
    EXPECT_EQ(result.m_other.getDuration(), babelwires::Rational(11, 4));

    const std::vector<seqwires::TrackEventHolder> monoEvents0 = {
        seqwires::NoteOnEvent{0, 72}, seqwires::NoteOffEvent{babelwires::Rational(1, 2), 72},
        seqwires::NoteOnEvent{0, 48}, seqwires::NoteOffEvent{babelwires::Rational(3, 4), 48},
        seqwires::NoteOnEvent{0, 50}, seqwires::NoteOffEvent{babelwires::Rational(3, 4), 50},
        seqwires::NoteOnEvent{0, 48}, seqwires::NoteOffEvent{babelwires::Rational(3, 4), 48},
    };

    testUtils::testNotesAndChords(monoEvents0, result.m_noteTracks[0]);

    const std::vector<seqwires::TrackEventHolder> monoEvents1 = {
        seqwires::NoteOnEvent{babelwires::Rational(1, 4), 60},
        seqwires::NoteOffEvent{babelwires::Rational(3, 4), 60},
        seqwires::NoteOnEvent{0, 62},
        seqwires::NoteOffEvent{babelwires::Rational(3, 4), 62},
        seqwires::NoteOnEvent{0, 60},
        seqwires::NoteOffEvent{babelwires::Rational(3, 4), 60}};

    testUtils::testNotesAndChords(monoEvents1, result.m_noteTracks[1]);

    const std::vector<seqwires::TrackEventHolder> otherEvents = {
        seqwires::ChordOnEvent{0, {seqwires::PitchClass::Value::C, seqwires::ChordType::ChordType::Value::M}},
        seqwires::NoteOnEvent{babelwires::Rational(3, 4), 74},
        seqwires::ChordOffEvent{babelwires::Rational(1, 2)},
        seqwires::ChordOnEvent{0, {seqwires::PitchClass::Value::D, seqwires::ChordType::ChordType::Value::m}},
        seqwires::NoteOffEvent{babelwires::Rational(1, 4), 74},
        seqwires::NoteOnEvent{0, 72},
        seqwires::NoteOffEvent{babelwires::Rational(3, 4), 72},
        seqwires::ChordOffEvent{babelwires::Rational(1, 2)},
    };

    testUtils::testNotesAndChords(otherEvents, result.m_other);
}

TEST(MonophonicSubtracksProcessorTest, processor) {
    testUtils::TestEnvironment testEnvironment;
    seqwires::registerLib(testEnvironment.m_projectContext);

    seqwires::MonophonicSubtracksProcessor processor(testEnvironment.m_projectContext);

    processor.getInput().setToDefault();
    processor.getOutput().setToDefault();

    auto input = seqwires::MonophonicSubtracksProcessorInput::Instance(processor.getInput());
    const auto output = seqwires::MonophonicSubtracksProcessorOutput::ConstInstance(processor.getOutput());

    input.getNumTrk().set(2);
    input.getPolicy().set(seqwires::MonophonicSubtracksPolicyEnum::Value::Low);
    input.getInput().set(getSamplePolyphonicTrack());

    processor.process(testEnvironment.m_log);

    EXPECT_EQ(output.getSbtrks().getSize(), 2);
    testUtils::testSimpleNotes({48, 50, 52, 53}, output.getSbtrks().getEntry(0).get());
    testUtils::testSimpleNotes({72, 74, 76, 77}, output.getSbtrks().getEntry(1).get());
    testUtils::testChords({{seqwires::PitchClass::Value::C, seqwires::ChordType::ChordType::Value::M},
                           {seqwires::PitchClass::Value::D, seqwires::ChordType::ChordType::Value::m}},
                          output.getOther().get());

    processor.getInput().clearChanges();
    {
        input.getNumTrk().set(3);
        input.getPolicy().set(seqwires::MonophonicSubtracksPolicyEnum::Value::High);
    }
    processor.process(testEnvironment.m_log);

    EXPECT_EQ(output.getSbtrks().getSize(), 3);
    testUtils::testSimpleNotes({72, 74, 76, 77}, output.getSbtrks().getEntry(0).get());
    testUtils::testSimpleNotes({48, 50, 52, 53}, output.getSbtrks().getEntry(1).get());
    EXPECT_EQ(output.getSbtrks().getEntry(2).get().getNumEvents(), 0);
    testUtils::testChords({{seqwires::PitchClass::Value::C, seqwires::ChordType::ChordType::Value::M},
                           {seqwires::PitchClass::Value::D, seqwires::ChordType::ChordType::Value::m}},
                          output.getOther().get());
}
#include <gtest/gtest.h>

#include <SeqWiresLib/Features/trackFeature.hpp>
#include <SeqWiresLib/Functions/monophonicSubtracksFunction.hpp>
#include <SeqWiresLib/Tracks/chordEvents.hpp>
#include <SeqWiresLib/Tracks/noteEvents.hpp>
#include <SeqWiresLib/Tracks/trackEventHolder.hpp>

#include <BabelWiresLib/Features/arrayFeature.hpp>

#include <Tests/TestUtils/seqTestUtils.hpp>
#include <Tests/TestUtils/testLog.hpp>

namespace {
    seqwires::Track getSamplePolyphonicTrack() {
        seqwires::Track track;
        track.addEvent(seqwires::NoteOnEvent{0, 72});
        track.addEvent(seqwires::NoteOnEvent{0, 48});
        track.addEvent(
            seqwires::ChordOnEvent{0, seqwires::PitchClass::PITCH_CLASS_C, seqwires::ChordType::CHORD_TYPE_MAJOR});
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
            seqwires::ChordOnEvent{0, seqwires::PitchClass::PITCH_CLASS_D, seqwires::ChordType::CHORD_TYPE_MINOR});
        track.addEvent(seqwires::NoteOffEvent{babelwires::Rational(1, 4), 76});
        track.addEvent(seqwires::NoteOffEvent{0, 52});
        track.addEvent(seqwires::NoteOnEvent{0, 77});
        track.addEvent(seqwires::NoteOnEvent{0, 53});
        track.addEvent(seqwires::NoteOffEvent{babelwires::Rational(1, 4), 77});
        track.addEvent(seqwires::NoteOffEvent{0, 53});
        track.addEvent(seqwires::ChordOffEvent{0});
        return track;
    }
} // namespace

TEST(MonophonicSubtracksProcessorTest, simpleFunction) {
    seqwires::Track track = getSamplePolyphonicTrack();

    seqwires::MonophonicSubtracksResult result = seqwires::getMonophonicSubtracks(track, 2);

    ASSERT_EQ(result.m_noteTracks.size(), 2);
    EXPECT_EQ(result.m_noteTracks[0].getDuration(), 1);
    EXPECT_EQ(result.m_noteTracks[1].getDuration(), 1);
    EXPECT_EQ(result.m_other.getDuration(), 1);

    testUtils::testSimpleNotes({72, 74, 76, 77}, result.m_noteTracks[0]);
    testUtils::testSimpleNotes({48, 50, 52, 53}, result.m_noteTracks[1]);
    testUtils::testChords({{seqwires::PitchClass::PITCH_CLASS_C, seqwires::ChordType::CHORD_TYPE_MAJOR},
                           {seqwires::PitchClass::PITCH_CLASS_D, seqwires::ChordType::CHORD_TYPE_MINOR}},
                          result.m_other);
}

TEST(MonophonicSubtracksProcessorTest, redundantTracks) {
    seqwires::Track track = getSamplePolyphonicTrack();

    seqwires::MonophonicSubtracksResult result = seqwires::getMonophonicSubtracks(track, 3);

    ASSERT_EQ(result.m_noteTracks.size(), 3);
    EXPECT_EQ(result.m_noteTracks[0].getDuration(), 1);
    EXPECT_EQ(result.m_noteTracks[1].getDuration(), 1);
    EXPECT_EQ(result.m_noteTracks[2].getDuration(), 1);
    EXPECT_EQ(result.m_other.getDuration(), 1);

    testUtils::testSimpleNotes({72, 74, 76, 77}, result.m_noteTracks[0]);
    testUtils::testSimpleNotes({48, 50, 52, 53}, result.m_noteTracks[1]);
    EXPECT_EQ(result.m_noteTracks[2].getNumEvents(), 0);
    testUtils::testChords({{seqwires::PitchClass::PITCH_CLASS_C, seqwires::ChordType::CHORD_TYPE_MAJOR},
                           {seqwires::PitchClass::PITCH_CLASS_D, seqwires::ChordType::CHORD_TYPE_MINOR}},
                          result.m_other);
}

TEST(MonophonicSubtracksProcessorTest, eventToOther) {
    seqwires::Track track = getSamplePolyphonicTrack();

    seqwires::MonophonicSubtracksResult result = seqwires::getMonophonicSubtracks(track, 1);

    ASSERT_EQ(result.m_noteTracks.size(), 1);
    EXPECT_EQ(result.m_noteTracks[0].getDuration(), 1);
    EXPECT_EQ(result.m_other.getDuration(), 1);

    testUtils::testSimpleNotes({72, 74, 76, 77}, result.m_noteTracks[0]);

    std::vector<seqwires::TrackEventHolder> expectedEvents = {seqwires::ChordOnEvent{0, seqwires::PitchClass::PITCH_CLASS_C, seqwires::ChordType::CHORD_TYPE_MAJOR},
                                                              seqwires::NoteOnEvent{0, 48},
                                                              seqwires::NoteOffEvent{babelwires::Rational(1, 4), 48},
                                                              seqwires::NoteOnEvent{0, 50},
                                                              seqwires::ChordOffEvent{babelwires::Rational(1, 4)},
                                                              seqwires::ChordOnEvent{0, seqwires::PitchClass::PITCH_CLASS_D, seqwires::ChordType::CHORD_TYPE_MINOR},
                                                              seqwires::NoteOffEvent{0, 50},
                                                              seqwires::NoteOnEvent{0, 52},
                                                              seqwires::NoteOffEvent{babelwires::Rational(1, 4), 52},
                                                              seqwires::NoteOnEvent{0, 53},
                                                              seqwires::ChordOffEvent{babelwires::Rational(1, 4)},
                                                              seqwires::NoteOffEvent{0, 53}};

    auto it = result.m_other.begin();
    const auto end = result.m_other.end();

    for (auto e : expectedEvents) {
        ASSERT_NE(it, end);
        EXPECT_EQ(it->getTimeSinceLastEvent(), e->getTimeSinceLastEvent());
        EXPECT_EQ((it->as<seqwires::NoteOnEvent>() == nullptr), (e->as<seqwires::NoteOnEvent>() == nullptr));
        EXPECT_EQ((it->as<seqwires::NoteOffEvent>() == nullptr), (e->as<seqwires::NoteOffEvent>() == nullptr));
        EXPECT_EQ((it->as<seqwires::ChordOnEvent>() == nullptr), (e->as<seqwires::ChordOnEvent>() == nullptr));
        EXPECT_EQ((it->as<seqwires::ChordOffEvent>() == nullptr), (e->as<seqwires::ChordOffEvent>() == nullptr));
        if (it->as<seqwires::NoteEvent>() != nullptr) {
            EXPECT_EQ(it->as<seqwires::NoteEvent>()->m_pitch, e->as<seqwires::NoteEvent>()->m_pitch);
            EXPECT_EQ(it->as<seqwires::NoteEvent>()->m_velocity, e->as<seqwires::NoteEvent>()->m_velocity);
        }
        if (it->as<seqwires::ChordOnEvent>() != nullptr) {
            EXPECT_EQ(it->as<seqwires::ChordOnEvent>()->m_root, e->as<seqwires::ChordOnEvent>()->m_root);
            EXPECT_EQ(it->as<seqwires::ChordOnEvent>()->m_chordType, e->as<seqwires::ChordOnEvent>()->m_chordType);
        }
        ++it;
    }
    EXPECT_EQ(it, end);
}

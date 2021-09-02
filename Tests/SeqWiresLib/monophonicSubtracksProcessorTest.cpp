#include <gtest/gtest.h>

#include <SeqWiresLib/Features/trackFeature.hpp>
#include <SeqWiresLib/Functions/monophonicSubtracksFunction.hpp>
#include <SeqWiresLib/Tracks/chordEvents.hpp>
#include <SeqWiresLib/Tracks/noteEvents.hpp>
#include <SeqWiresLib/Tracks/trackEventHolder.hpp>

#include <BabelWiresLib/Features/arrayFeature.hpp>

#include <Tests/TestUtils/seqTestUtils.hpp>
#include <Tests/TestUtils/testLog.hpp>

TEST(MonophonicSubtracksProcessorTest, simpleFunction) {
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
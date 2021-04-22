#include <gtest/gtest.h>

#include "SeqWiresLib/Utilities/monophonicNoteIterator.hpp"

namespace {
    /// A non-note event.
    struct TestEvent : seqwires::TrackEvent {
        STREAM_EVENT(TestEvent);
        TestEvent(seqwires::ModelDuration d) { setTimeSinceLastEvent(d); }
    };

    seqwires::Track createTestTrack() {
        seqwires::Track track;

        seqwires::NoteOnEvent noteOn;
        seqwires::NoteOffEvent noteOff;

        // Expect this note.
        noteOn.setTimeSinceLastEvent(0);
        noteOn.m_pitch = 40;
        track.addEvent(noteOn);

        noteOff.setTimeSinceLastEvent(1);
        noteOff.m_pitch = 40;
        track.addEvent(noteOff);
        // End of note

        // Expect this note.
        noteOn.setTimeSinceLastEvent(0);
        noteOn.m_pitch = 40;
        track.addEvent(noteOn);

        track.addEvent(TestEvent(1));

        noteOff.setTimeSinceLastEvent(1);
        noteOff.m_pitch = 40;
        track.addEvent(noteOff);
        // End of note

        track.addEvent(TestEvent(5));

        // Expect this note.
        noteOn.setTimeSinceLastEvent(0);
        noteOn.m_pitch = 50;
        track.addEvent(noteOn);

        track.addEvent(TestEvent(1));

        noteOn.setTimeSinceLastEvent(1);
        noteOn.m_pitch = 40;
        track.addEvent(noteOn);

        track.addEvent(TestEvent(1));
        track.addEvent(TestEvent(1));

        noteOff.setTimeSinceLastEvent(0);
        noteOff.m_pitch = 40;
        track.addEvent(noteOff);

        track.addEvent(TestEvent(1));

        noteOn.setTimeSinceLastEvent(1);
        noteOn.m_pitch = 40;
        track.addEvent(noteOn);

        noteOff.setTimeSinceLastEvent(1);
        noteOff.m_pitch = 50;
        track.addEvent(noteOff);
        // End of note

        track.addEvent(TestEvent(1));

        noteOff.setTimeSinceLastEvent(1);
        noteOff.m_pitch = 40;
        track.addEvent(noteOff);

        track.addEvent(TestEvent(1));

        return track;
    }
} // namespace

TEST(MonophonicNoteIterator, OnOffOnly) {
    seqwires::Track track = createTestTrack();

    auto [it, end] = seqwires::iterateOverMonotonicNotes(track, seqwires::MonophonicNoteIterator::OnOffOnly);

    ASSERT_NE(it, end);
    ASSERT_NE(it->asA<seqwires::NoteOnEvent>(), nullptr);
    EXPECT_EQ(it->asA<seqwires::NoteOnEvent>()->m_pitch, 40);
    EXPECT_EQ(it->asA<seqwires::NoteOnEvent>()->getTimeSinceLastEvent(), 0);

    ++it;
    ASSERT_NE(it, end);
    ASSERT_NE(it->asA<seqwires::NoteOffEvent>(), nullptr);
    EXPECT_EQ(it->asA<seqwires::NoteOffEvent>()->m_pitch, 40);
    EXPECT_EQ(it->asA<seqwires::NoteOffEvent>()->getTimeSinceLastEvent(), 1);

    ++it;
    ASSERT_NE(it, end);
    ASSERT_NE(it->asA<seqwires::NoteOnEvent>(), nullptr);
    EXPECT_EQ(it->asA<seqwires::NoteOnEvent>()->m_pitch, 40);
    EXPECT_EQ(it->asA<seqwires::NoteOnEvent>()->getTimeSinceLastEvent(), 0);

    ++it;
    ASSERT_NE(it, end);
    ASSERT_NE(it->asA<seqwires::NoteOffEvent>(), nullptr);
    EXPECT_EQ(it->asA<seqwires::NoteOffEvent>()->m_pitch, 40);
    EXPECT_EQ(it->asA<seqwires::NoteOffEvent>()->getTimeSinceLastEvent(), 2);

    ++it;
    ASSERT_NE(it, end);
    ASSERT_NE(it->asA<seqwires::NoteOnEvent>(), nullptr);
    EXPECT_EQ(it->asA<seqwires::NoteOnEvent>()->m_pitch, 50);
    EXPECT_EQ(it->asA<seqwires::NoteOnEvent>()->getTimeSinceLastEvent(), 5);

    ++it;
    ASSERT_NE(it, end);
    ASSERT_NE(it->asA<seqwires::NoteOffEvent>(), nullptr);
    EXPECT_EQ(it->asA<seqwires::NoteOffEvent>()->m_pitch, 50);
    EXPECT_EQ(it->asA<seqwires::NoteOffEvent>()->getTimeSinceLastEvent(), 7);

    ++it;
    ASSERT_EQ(it, end);
}

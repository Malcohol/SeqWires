#include <Tests/TestUtils/seqTestUtils.hpp>

#include <SeqWiresLib/Features/trackFeature.hpp>
#include <SeqWiresLib/Tracks/noteEvents.hpp>
#include <SeqWiresLib/Tracks/chordEvents.hpp>

#include <gtest/gtest.h>

void testUtils::addSimpleNotes(const std::vector<seqwires::Pitch>& expectedPitches, seqwires::Track& track) {
    for (auto pitch : expectedPitches) {
        seqwires::NoteOnEvent noteOn;
        noteOn.setTimeSinceLastEvent(0);
        noteOn.m_pitch = pitch;
        noteOn.m_velocity = 127;
        track.addEvent(noteOn);

        seqwires::NoteOffEvent noteOff;
        noteOff.setTimeSinceLastEvent(babelwires::Rational(1, 4));
        noteOff.m_pitch = pitch;
        noteOff.m_velocity = 64;
        track.addEvent(noteOff);
    }
}

void testUtils::testSimpleNotes(const std::vector<seqwires::Pitch>& expectedPitches, const seqwires::Track& track) {
    const seqwires::ModelDuration expectedNoteDuration = babelwires::Rational(1, 4);

    // Note: right now, we only parse notes. As we add parsing of other event types, this test will fail.
    // (A temporarily fix would be to use a FilteredTrackIterator.)
    auto noteIterator = track.begin();
    const auto endIterator = track.end();

    for (auto pitch : expectedPitches) {
        EXPECT_NE(noteIterator, endIterator);
        auto noteOn = noteIterator->as<const seqwires::NoteOnEvent>();
        ASSERT_NE(noteOn, nullptr);
        EXPECT_EQ(noteOn->getTimeSinceLastEvent(), 0);
        EXPECT_EQ(noteOn->m_pitch, pitch);
        EXPECT_EQ(noteOn->m_velocity, 127);
        ++noteIterator;

        EXPECT_NE(noteIterator, endIterator);
        auto noteOff = noteIterator->as<seqwires::NoteOffEvent>();
        ASSERT_NE(noteOff, nullptr);
        EXPECT_EQ(noteOff->getTimeSinceLastEvent(), expectedNoteDuration);
        EXPECT_EQ(noteOff->m_pitch, pitch);
        EXPECT_EQ(noteOff->m_velocity, 64);
        ++noteIterator;
    }
    EXPECT_EQ(noteIterator, endIterator);
    EXPECT_EQ(track.getDuration(), expectedNoteDuration * expectedPitches.size());
}

void testUtils::addNotes(const std::vector<NoteInfo>& notes, seqwires::Track& track) {
    for (auto note : notes) {
        seqwires::NoteOnEvent noteOn;
        noteOn.setTimeSinceLastEvent(note.m_noteOnTime);
        noteOn.m_pitch = note.m_pitch;
        noteOn.m_velocity = 127;
        track.addEvent(noteOn);

        seqwires::NoteOffEvent noteOff;
        noteOff.setTimeSinceLastEvent(note.m_noteOffTime);
        noteOff.m_pitch = note.m_pitch;
        noteOff.m_velocity = 64;
        track.addEvent(noteOff);
    }
}

void testUtils::testNotes(const std::vector<NoteInfo>& expectedNotes, const seqwires::Track& track) {
    auto noteIterator = track.begin();
    const auto endIterator = track.end();

    for (auto note : expectedNotes)
    {
        EXPECT_NE(noteIterator, endIterator);
        auto noteOn = noteIterator->as<const seqwires::NoteOnEvent>();
        ASSERT_NE(noteOn, nullptr);
        EXPECT_EQ(noteOn->getTimeSinceLastEvent(), note.m_noteOnTime);
        EXPECT_EQ(noteOn->m_pitch, note.m_pitch);
        EXPECT_EQ(noteOn->m_velocity, 127);
        ++noteIterator;

        EXPECT_NE(noteIterator, endIterator);
        auto noteOff = noteIterator->as<seqwires::NoteOffEvent>();
        ASSERT_NE(noteOff, nullptr);
        EXPECT_EQ(noteOff->getTimeSinceLastEvent(), note.m_noteOffTime);
        EXPECT_EQ(noteOff->m_pitch, note.m_pitch);
        EXPECT_EQ(noteOff->m_velocity, 64);
        ++noteIterator;
    }
    EXPECT_EQ(noteIterator, endIterator);
}

void testUtils::addChords(const std::vector<ChordInfo>& chords, seqwires::Track& track) {
    for (auto chord : chords) {
        seqwires::ChordOnEvent chordOn;
        chordOn.setTimeSinceLastEvent(chord.m_chordOnTime);
        chordOn.m_root = chord.m_root;
        chordOn.m_chordType = chord.m_chordType;
        track.addEvent(chordOn);

        seqwires::ChordOffEvent chordOff;
        chordOff.setTimeSinceLastEvent(chord.m_chordOffTime);
        track.addEvent(chordOff);
    }
}

void testUtils::testChords(const std::vector<ChordInfo>& expectedChords, const seqwires::Track& track) {
    // Note: right now, we only parse notes. As we add parsing of other event types, this test will fail.
    // (A temporarily fix would be to use a FilteredTrackIterator.)
    auto chordIterator = track.begin();
    const auto endIterator = track.end();

    for (auto chord : expectedChords) {
        EXPECT_NE(chordIterator, endIterator);
        auto chordOn = chordIterator->as<const seqwires::ChordOnEvent>();
        ASSERT_NE(chordOn, nullptr);
        EXPECT_EQ(chordOn->getTimeSinceLastEvent(), chord.m_chordOnTime);
        EXPECT_EQ(chordOn->m_chordType, chord.m_chordType);
        EXPECT_EQ(chordOn->m_root, chord.m_root);
        ++chordIterator;

        EXPECT_NE(chordIterator, endIterator);
        auto chordOff = chordIterator->as<seqwires::ChordOffEvent>();
        ASSERT_NE(chordOff, nullptr);
        EXPECT_EQ(chordOff->getTimeSinceLastEvent(), chord.m_chordOffTime);
        ++chordIterator;
    }
    EXPECT_EQ(chordIterator, endIterator);
}

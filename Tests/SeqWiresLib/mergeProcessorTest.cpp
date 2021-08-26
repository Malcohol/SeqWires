#include <gtest/gtest.h>

#include <SeqWiresLib/Functions/splitAtPitchFunction.hpp>
#include <SeqWiresLib/Processors/splitAtPitchProcessor.hpp>
#include <SeqWiresLib/Functions/mergeFunction.hpp>
#include <SeqWiresLib/Tracks/noteEvents.hpp>
#include <SeqWiresLib/Features/trackFeature.hpp>
#include <SeqWiresLib/Tracks/trackEventHolder.hpp>

#include <BabelWiresLib/Features/arrayFeature.hpp>

#include <Tests/TestUtils/testLog.hpp>
#include <Tests/TestUtils/seqTestUtils.hpp>

TEST(MergeProcessorTest, simpleFunction) {
    seqwires::Track trackA;
    testUtils::addSimpleNotes({72, 74, 76, 77}, trackA);

    seqwires::Track trackB;
    testUtils::addSimpleNotes({48, 50, 52, 53}, trackB);

    seqwires::Track track = seqwires::mergeTracks({&trackA, &trackB});
    ASSERT_EQ(track.getDuration(), 1);

    std::vector<seqwires::TrackEventHolder> expectedEvents = {
      seqwires::NoteOnEvent{ 0, 72 },
      seqwires::NoteOnEvent{ 0, 48 },
      seqwires::NoteOffEvent{ babelwires::Rational(1, 4), 72 },
      seqwires::NoteOnEvent{ 0, 74 },
      seqwires::NoteOffEvent{ 0, 48 },
      seqwires::NoteOnEvent{ 0, 50 },
      seqwires::NoteOffEvent{ babelwires::Rational(1, 4), 74 },
      seqwires::NoteOnEvent{ 0, 76 },
      seqwires::NoteOffEvent{ 0, 50 },
      seqwires::NoteOnEvent{ 0, 52 },
      seqwires::NoteOffEvent{ babelwires::Rational(1, 4), 76 },
      seqwires::NoteOnEvent{ 0, 77 },
      seqwires::NoteOffEvent{ 0, 52 },
      seqwires::NoteOnEvent{ 0, 53 },
      seqwires::NoteOffEvent{ babelwires::Rational(1, 4), 77 },
      seqwires::NoteOffEvent{ 0, 53 }
    };

    auto it = track.begin();
    const auto end = track.end();

    for (auto e : expectedEvents) {
        ASSERT_NE(it, end);
        EXPECT_EQ(it->getTimeSinceLastEvent(), e->getTimeSinceLastEvent());
        EXPECT_NE(it->as<seqwires::NoteEvent>(), nullptr);
        EXPECT_EQ((it->as<seqwires::NoteOnEvent>() == nullptr), (e->as<seqwires::NoteOnEvent>() == nullptr));
        EXPECT_EQ((it->as<seqwires::NoteOffEvent>() == nullptr), (e->as<seqwires::NoteOffEvent>() == nullptr));
        EXPECT_EQ(it->as<seqwires::NoteEvent>()->m_pitch, e->as<seqwires::NoteEvent>()->m_pitch);
        EXPECT_EQ(it->as<seqwires::NoteEvent>()->m_velocity, e->as<seqwires::NoteEvent>()->m_velocity);
        ++it;
    }
    EXPECT_EQ(it, end);
}

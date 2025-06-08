#include <gtest/gtest.h>

#include <MusicLib/Utilities/trackValidator.hpp>
#include <MusicLib/Utilities/trackBuilder.hpp>
#include <MusicLib/Types/Track/TrackEvents/noteEvents.hpp>

#include <Tests/BabelWiresLib/TestUtils/testEnvironment.hpp>
#include <Tests/TestUtils/seqTestUtils.hpp>

#include <array>

TEST(TrackBuilderTest, validSimple) {
    bw_music::Track track;

    track.addEvent(bw_music::NoteOnEvent(0, 60));
    track.addEvent(bw_music::NoteOffEvent(babelwires::Rational(1, 4), 60));
    track.addEvent(bw_music::NoteOnEvent(0, 62));
    track.addEvent(bw_music::NoteOffEvent(babelwires::Rational(1, 4), 62));
    track.addEvent(bw_music::NoteOnEvent(0, 64));
    track.addEvent(bw_music::NoteOffEvent(babelwires::Rational(1, 4), 64));
    track.addEvent(bw_music::NoteOnEvent(0, 67));
    track.addEvent(bw_music::NoteOffEvent(babelwires::Rational(1, 4), 67));

    EXPECT_TRUE(bw_music::isTrackValid(track));
}

TEST(TrackBuilderTest, invalidSimpleZeroLengthNote) {
    bw_music::Track track;

    track.addEvent(bw_music::NoteOnEvent(0, 60));
    track.addEvent(bw_music::NoteOffEvent(babelwires::Rational(1, 4), 60));
    track.addEvent(bw_music::NoteOnEvent(0, 62));
    track.addEvent(bw_music::NoteOffEvent(0, 62));
    track.addEvent(bw_music::NoteOnEvent(0, 64));
    track.addEvent(bw_music::NoteOffEvent(babelwires::Rational(1, 4), 64));
    track.addEvent(bw_music::NoteOnEvent(0, 67));
    track.addEvent(bw_music::NoteOffEvent(babelwires::Rational(1, 4), 67));

    EXPECT_FALSE(bw_music::isTrackValid(track));
}

TEST(TrackBuilderTest, invalidSimpleEndEventOutsideGroup) {
    bw_music::Track track;

    track.addEvent(bw_music::NoteOnEvent(0, 60));
    track.addEvent(bw_music::NoteOffEvent(babelwires::Rational(1, 4), 60));
    track.addEvent(bw_music::NoteOnEvent(0, 62));
    track.addEvent(bw_music::NoteOffEvent(babelwires::Rational(1, 4), 62));
    track.addEvent(bw_music::NoteOnEvent(0, 64));
    track.addEvent(bw_music::NoteOffEvent(babelwires::Rational(1, 4), 64));
    track.addEvent(bw_music::NoteOffEvent(babelwires::Rational(1, 4), 62));
    track.addEvent(bw_music::NoteOnEvent(0, 67));
    track.addEvent(bw_music::NoteOffEvent(babelwires::Rational(1, 4), 67));

    EXPECT_FALSE(bw_music::isTrackValid(track));
}

TEST(TrackBuilderTest, invalidStartEventInsideGroup) {
    bw_music::Track track;

    track.addEvent(bw_music::NoteOnEvent(0, 60));
    track.addEvent(bw_music::NoteOffEvent(babelwires::Rational(1, 4), 60));
    track.addEvent(bw_music::NoteOnEvent(0, 62));
    track.addEvent(bw_music::NoteOffEvent(babelwires::Rational(1, 4), 62));
    track.addEvent(bw_music::NoteOnEvent(0, 64));
    track.addEvent(bw_music::NoteOnEvent(babelwires::Rational(1, 4), 64));
    track.addEvent(bw_music::NoteOffEvent(babelwires::Rational(1, 4), 64));
    track.addEvent(bw_music::NoteOnEvent(0, 67));
    track.addEvent(bw_music::NoteOffEvent(babelwires::Rational(1, 4), 67));

    EXPECT_FALSE(bw_music::isTrackValid(track));
}

TEST(TrackBuilderTest, builderValidSimple) {
    bw_music::Track track;
    bw_music::TrackBuilder trackBuilder;

    auto addEvent = [&track, &trackBuilder](auto&& event) {
        track.addEvent(event);
        trackBuilder.addEvent(std::forward<decltype(event)>(event));
    };

    addEvent(bw_music::NoteOnEvent(0, 60));
    addEvent(bw_music::NoteOffEvent(babelwires::Rational(1, 4), 60));
    addEvent(bw_music::NoteOnEvent(0, 62));
    addEvent(bw_music::NoteOffEvent(babelwires::Rational(1, 4), 62));
    addEvent(bw_music::NoteOnEvent(0, 64));
    addEvent(bw_music::NoteOffEvent(babelwires::Rational(1, 4), 64));
    addEvent(bw_music::NoteOnEvent(0, 67));
    addEvent(bw_music::NoteOffEvent(babelwires::Rational(1, 4), 67));

    auto builtTrack = trackBuilder.finishAndGetTrack();
    EXPECT_TRUE(bw_music::isTrackValid(track));
    EXPECT_TRUE(bw_music::isTrackValid(builtTrack));
    EXPECT_EQ(track.getDuration(), builtTrack.getDuration());
    EXPECT_EQ(track.getTotalEventDuration(), builtTrack.getTotalEventDuration());
}

TEST(TrackBuilderTest, builderInvalidSimpleZeroLengthNote) {
    bw_music::Track track;
    bw_music::TrackBuilder trackBuilder;

    auto addEvent = [&track, &trackBuilder](auto&& event) {
        track.addEvent(event);
        trackBuilder.addEvent(std::forward<decltype(event)>(event));
    };

    addEvent(bw_music::NoteOnEvent(0, 60));
    addEvent(bw_music::NoteOffEvent(babelwires::Rational(1, 4), 60));
    addEvent(bw_music::NoteOnEvent(0, 62));
    addEvent(bw_music::NoteOffEvent(0, 62));
    addEvent(bw_music::NoteOnEvent(0, 64));
    addEvent(bw_music::NoteOffEvent(babelwires::Rational(1, 4), 64));
    addEvent(bw_music::NoteOnEvent(0, 67));
    addEvent(bw_music::NoteOffEvent(babelwires::Rational(1, 4), 67));

    auto builtTrack = trackBuilder.finishAndGetTrack();
    EXPECT_FALSE(bw_music::isTrackValid(track));
    EXPECT_TRUE(bw_music::isTrackValid(builtTrack));
    EXPECT_EQ(track.getDuration(), builtTrack.getDuration());
    EXPECT_EQ(track.getTotalEventDuration(), builtTrack.getTotalEventDuration());
}

TEST(TrackBuilderTest, builderInvalidSimpleEndEventOutsideGroup) {
    bw_music::Track track;
    bw_music::TrackBuilder trackBuilder;

    auto addEvent = [&track, &trackBuilder](auto&& event) {
        track.addEvent(event);
        trackBuilder.addEvent(std::forward<decltype(event)>(event));
    };

    addEvent(bw_music::NoteOnEvent(0, 60));
    addEvent(bw_music::NoteOffEvent(babelwires::Rational(1, 4), 60));
    addEvent(bw_music::NoteOnEvent(0, 62));
    addEvent(bw_music::NoteOffEvent(babelwires::Rational(1, 4), 62));
    addEvent(bw_music::NoteOnEvent(0, 64));
    addEvent(bw_music::NoteOffEvent(babelwires::Rational(1, 4), 64));
    addEvent(bw_music::NoteOffEvent(babelwires::Rational(1, 4), 62));
    addEvent(bw_music::NoteOnEvent(0, 67));
    addEvent(bw_music::NoteOffEvent(babelwires::Rational(1, 4), 67));

    auto builtTrack = trackBuilder.finishAndGetTrack();
    EXPECT_FALSE(bw_music::isTrackValid(track));
    EXPECT_TRUE(bw_music::isTrackValid(builtTrack));
    EXPECT_EQ(track.getDuration(), builtTrack.getDuration());
    EXPECT_EQ(track.getTotalEventDuration(), builtTrack.getTotalEventDuration());
}

TEST(TrackBuilderTest, builderInvalidStartEventInsideGroup) {
    bw_music::Track track;
    bw_music::TrackBuilder trackBuilder;

    auto addEvent = [&track, &trackBuilder](auto&& event) {
        track.addEvent(event);
        trackBuilder.addEvent(std::forward<decltype(event)>(event));
    };

    addEvent(bw_music::NoteOnEvent(0, 60));
    addEvent(bw_music::NoteOffEvent(babelwires::Rational(1, 4), 60));
    addEvent(bw_music::NoteOnEvent(0, 62));
    addEvent(bw_music::NoteOffEvent(babelwires::Rational(1, 4), 62));
    addEvent(bw_music::NoteOnEvent(0, 64));
    addEvent(bw_music::NoteOnEvent(babelwires::Rational(1, 4), 64));
    addEvent(bw_music::NoteOffEvent(babelwires::Rational(1, 4), 64));
    addEvent(bw_music::NoteOnEvent(0, 67));
    addEvent(bw_music::NoteOffEvent(babelwires::Rational(1, 4), 67));

    auto builtTrack = trackBuilder.finishAndGetTrack();
    EXPECT_FALSE(bw_music::isTrackValid(track));
    EXPECT_TRUE(bw_music::isTrackValid(builtTrack));
    EXPECT_EQ(track.getDuration(), builtTrack.getDuration());
    EXPECT_EQ(track.getTotalEventDuration(), builtTrack.getTotalEventDuration());
}


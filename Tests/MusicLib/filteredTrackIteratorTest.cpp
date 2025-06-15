#include <gtest/gtest.h>

#include <MusicLib/Utilities/filteredTrackIterator.hpp>

#include <MusicLib/Types/Track/TrackEvents/trackEvent.hpp>

#include <Tests/TestUtils/testTrackEvents.hpp>

TEST(FilteredTrackIterator, Basic) {
    bw_music::Track track;

    for (int i = 0; i < 100; ++i) {
        track.addEvent(testUtils::TestTrackEvent(1, 2 * i));
        track.addEvent(testUtils::TestTrackEvent(0, (2 * i) + 1));
        track.addEvent(testUtils::TestTrackEvent2(0, 2 * i));
        track.addEvent(testUtils::TestTrackEvent2(1, (2 * i) + 1));
    }

    {
        auto [testEventsBegin, testEventsEnd] = bw_music::iterateOver<testUtils::TestTrackEvent>(track);

        int count = 0;
        bw_music::ModelDuration d = 0;
        for (auto t = testEventsBegin; t != testEventsEnd; ++t) {
            EXPECT_EQ(t->m_value, count);
            ++count;
            d += t->getTimeSinceLastEvent();
        }
        EXPECT_EQ(count, 200);
        EXPECT_EQ(d, 199);
    }

    {
        auto [testEventsBegin, testEventsEnd] = bw_music::iterateOver<testUtils::TestTrackEvent2>(track);

        int count = 0;
        bw_music::ModelDuration d = 0;
        for (auto t = testEventsBegin; t != testEventsEnd; ++t) {
            EXPECT_EQ(t->m_value, count);
            ++count;
            d += t->getTimeSinceLastEvent();
        }
        EXPECT_EQ(count, 200);
        EXPECT_EQ(d, 200);
    }
}

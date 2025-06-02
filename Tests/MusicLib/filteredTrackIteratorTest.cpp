#include <gtest/gtest.h>

#include <MusicLib/Utilities/filteredTrackIterator.hpp>

#include <MusicLib/Types/Track/TrackEvents/trackEvent.hpp>

namespace {
    struct TestEvent : bw_music::TrackEvent {
        STREAM_EVENT(TestEvent);
        TestEvent(bw_music::ModelDuration d, int value)
            : m_value(value) {
            setTimeSinceLastEvent(d);
        }

        int m_value;
    };

    struct TestEvent2 : bw_music::TrackEvent {
        STREAM_EVENT(TestEvent2);
        TestEvent2(bw_music::ModelDuration d, float value)
            : m_value(value) {
            setTimeSinceLastEvent(d);
        }

        float m_value;
    };
} // namespace

TEST(FilteredTrackIterator, Basic) {
    bw_music::Track track;

    for (int i = 0; i < 100; ++i) {
        track.addEvent(TestEvent(1, 2 * i));
        track.addEvent(TestEvent(0, (2 * i) + 1));
        track.addEvent(TestEvent2(0, 2 * i));
        track.addEvent(TestEvent2(1, (2 * i) + 1));
    }

    {
        auto [testEventsBegin, testEventsEnd] = bw_music::iterateOver<TestEvent>(track);

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
        auto [testEventsBegin, testEventsEnd] = bw_music::iterateOver<TestEvent2>(track);

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

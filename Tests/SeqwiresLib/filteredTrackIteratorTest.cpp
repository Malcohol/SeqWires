#include <gtest/gtest.h>

#include "SeqwiresLib/Utilities/filteredTrackIterator.hpp"

#include "SeqwiresLib/Tracks/trackEvent.hpp"

namespace {
    struct TestEvent : seqwires::TrackEvent {
        STREAM_EVENT(TestEvent);
        TestEvent(seqwires::ModelDuration d, int value)
            : m_value(value) {
            setTimeSinceLastEvent(d);
        }

        int m_value;
    };

    struct TestEvent2 : seqwires::TrackEvent {
        STREAM_EVENT(TestEvent2);
        TestEvent2(seqwires::ModelDuration d, float value)
            : m_value(value) {
            setTimeSinceLastEvent(d);
        }

        float m_value;
    };
} // namespace

TEST(FilteredTrackIterator, Basic) {
    seqwires::Track track;

    for (int i = 0; i < 100; ++i) {
        track.addEvent(TestEvent(1, 2 * i));
        track.addEvent(TestEvent(0, (2 * i) + 1));
        track.addEvent(TestEvent2(0, 2 * i));
        track.addEvent(TestEvent2(1, (2 * i) + 1));
    }

    {
        auto [testEventsBegin, testEventsEnd] = seqwires::iterateOver<TestEvent>(track);

        int count = 0;
        seqwires::ModelDuration d = 0;
        for (auto t = testEventsBegin; t != testEventsEnd; ++t) {
            EXPECT_EQ(t->m_value, count);
            ++count;
            d += t->getTimeSinceLastEvent();
        }
        EXPECT_EQ(count, 200);
        EXPECT_EQ(d, 199);
    }

    {
        auto [testEventsBegin, testEventsEnd] = seqwires::iterateOver<TestEvent2>(track);

        int count = 0;
        seqwires::ModelDuration d = 0;
        for (auto t = testEventsBegin; t != testEventsEnd; ++t) {
            EXPECT_EQ(t->m_value, count);
            ++count;
            d += t->getTimeSinceLastEvent();
        }
        EXPECT_EQ(count, 200);
        EXPECT_EQ(d, 200);
    }
}

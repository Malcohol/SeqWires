#include <gtest/gtest.h>

#include <MusicLib/Types/Track/TrackEvents/noteEvents.hpp>
#include <MusicLib/Types/Track/track.hpp>

#include <Tests/TestUtils/seqTestUtils.hpp>

TEST(Track, Simple) {
    bw_music::Track track;
    const size_t hashWhenEmpty = track.getHash();

    EXPECT_EQ(track.getNumEvents(), 0);
    EXPECT_EQ(track.getDuration(), 0);

    bw_music::TrackEvent event0;
    track.addEvent(event0);

    EXPECT_EQ(track.getNumEvents(), 1);
    EXPECT_EQ(track.getDuration(), 0);
    const size_t hashWithOneEvent = track.getHash();
    EXPECT_NE(hashWhenEmpty, hashWithOneEvent);

    bw_music::TrackEvent event1;
    event1.setTimeSinceLastEvent(1);
    track.addEvent(event1);

    EXPECT_EQ(track.getNumEvents(), 2);
    EXPECT_EQ(track.getDuration(), 1);
    const size_t hashWithTwoEvents = track.getHash();
    EXPECT_NE(hashWhenEmpty, hashWithTwoEvents);
    EXPECT_NE(hashWithOneEvent, hashWithTwoEvents);

    int count = 0;
    for (const auto& event : static_cast<const bw_music::Track&>(track)) {
        ++count;
    }
    EXPECT_EQ(count, 2);
}

namespace {
    struct alignas(16) TestEvent : bw_music::TrackEvent {
        STREAM_EVENT(TestEvent);
        TestEvent(bw_music::ModelDuration d) { setTimeSinceLastEvent(d); }

        std::uint64_t m_big[5] = {};
    };
} // namespace

TEST(Track, BlocksAndAlignment) {
    EXPECT_NE(bw_music::TrackEvent().getSize(), TestEvent(1).getSize());
    EXPECT_NE(bw_music::TrackEvent().getAlignment(), TestEvent(1).getAlignment());

    bw_music::Track track;

    for (int i = 0; i < 100; ++i) {
        bw_music::TrackEvent newEvent;
        newEvent.setTimeSinceLastEvent(1);
        track.addEvent(std::move(newEvent));
        TestEvent newEvent2(1);
        newEvent2.m_big[2] = i;
        track.addEvent(std::move(newEvent2));
    }
    EXPECT_EQ(track.getNumEvents(), 200);
    EXPECT_EQ(track.getDuration(), 200);

    int count = 0;
    for (const auto& event : track) {
        EXPECT_EQ(event.getTimeSinceLastEvent(), 1);
        if (count % 2) {
            EXPECT_NE(event.as<TestEvent>(), nullptr);
            const TestEvent& e = static_cast<const TestEvent&>(event);
            EXPECT_EQ(e.m_big[2], (count / 2));
        }
        ++count;
    }
    EXPECT_EQ(count, 200);
}

namespace {
    struct Payload {
        Payload(int& payloadDestructionCounter)
            : m_payloadDestructionCounter(payloadDestructionCounter) {}

        ~Payload() { ++m_payloadDestructionCounter; }

        int& m_payloadDestructionCounter;
    };

    struct EventWithPayload : bw_music::TrackEvent {
        STREAM_EVENT(EventWithPayload);

        EventWithPayload(bw_music::ModelDuration d, int& payloadDestructionCounter)
            : m_payload(std::make_unique<Payload>(payloadDestructionCounter)) {
            setTimeSinceLastEvent(d);
        }

        EventWithPayload(EventWithPayload&& other)
            : TrackEvent(other) {
            m_payload = std::move(other.m_payload);
        }

        EventWithPayload(const EventWithPayload& other)
            : bw_music::TrackEvent(other)
            , m_payload(std::make_unique<Payload>(*other.m_payload)) {}

        ~EventWithPayload() {
            m_payload.reset();
        }

        std::unique_ptr<Payload> m_payload;
    };
} // namespace

TEST(Track, PayloadTest) {
    int counter = 0;

    {
        bw_music::Track track;

        for (int i = 0; i < 100; ++i) {
            track.addEvent(EventWithPayload(1, counter));
        }

        EXPECT_EQ(counter, 0);
    }

    EXPECT_EQ(counter, 100);
}

TEST(Track, equality) {
    bw_music::Track emptyTrack0;
    bw_music::Track emptyTrack1;
    bw_music::Track emptyTrackWithPositiveDuration;
    bw_music::Track trackWithNotes;
    bw_music::Track trackWithSameNotes;
    bw_music::Track trackWithMoreNotes;
    bw_music::Track trackWithDifferentNotes;
    bw_music::Track trackWithSameNotesLongerDuration;
    
    emptyTrackWithPositiveDuration.setDuration(1);
    testUtils::addSimpleNotes(std::vector<bw_music::Pitch>{60, 62, 64, 65}, trackWithNotes);
    testUtils::addSimpleNotes(std::vector<bw_music::Pitch>{60, 62, 64, 65}, trackWithSameNotes);
    testUtils::addSimpleNotes(std::vector<bw_music::Pitch>{60, 62, 64, 65, 67}, trackWithMoreNotes);
    testUtils::addSimpleNotes(std::vector<bw_music::Pitch>{60, 62, 63, 65}, trackWithDifferentNotes);
    testUtils::addSimpleNotes(std::vector<bw_music::Pitch>{60, 62, 64, 65}, trackWithSameNotesLongerDuration);
    trackWithSameNotesLongerDuration.setDuration(16);

    EXPECT_EQ(emptyTrack0, emptyTrack0);
    EXPECT_EQ(emptyTrack0, emptyTrack1);
    EXPECT_NE(emptyTrack0, emptyTrackWithPositiveDuration);
    EXPECT_NE(emptyTrack0, trackWithNotes);
    EXPECT_EQ(trackWithNotes, trackWithSameNotes);
    EXPECT_NE(trackWithDifferentNotes, trackWithNotes);
    EXPECT_NE(trackWithNotes, trackWithMoreNotes);
    EXPECT_NE(trackWithNotes, trackWithSameNotesLongerDuration);
}
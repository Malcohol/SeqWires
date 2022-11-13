/**
 * PercussionEvents describe musical notes.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <SeqWiresLib/Tracks/trackEvent.hpp>

namespace seqwires {

    /// Base of type for percussion events.
    struct PercussionEvent : public TrackEvent {
        STREAM_EVENT_ABSTRACT(PercussionEvent);
        PercussionEvent() = default;
        PercussionEvent(ModelDuration timeSinceLastEvent, Pitch pitch, Velocity velocity)
            : TrackEvent(timeSinceLastEvent)
            , m_pitch(pitch)
            , m_velocity(velocity) {}

        static GroupingInfo::Category s_percussionEventCategory;

        void setPitch(Pitch pitch) { m_pitch = pitch; }
        Pitch getPitch() const { return m_pitch; }

        void setVelocity(Velocity velocity) { m_velocity = velocity; }
        Velocity getVelocity() const { return m_velocity; }

      protected:
        // TODO Replace pitch by Identifier m_instrument;
        Pitch m_pitch;
        Velocity m_velocity;
    };

    /// The start of a percussion event.
    struct PercussionOnEvent : public PercussionEvent {
        STREAM_EVENT(PercussionOnEvent);
        PercussionOnEvent() = default;
        PercussionOnEvent(ModelDuration timeSinceLastEvent, Pitch pitch, Velocity velocity = 127)
            : PercussionEvent(timeSinceLastEvent, pitch, velocity) {}

        virtual bool operator==(const TrackEvent& other) const override;
        virtual std::size_t getHash() const override;
        virtual GroupingInfo getGroupingInfo() const override;
    };

    /// The end of a percussion event.
    struct PercussionOffEvent : public PercussionEvent {
        STREAM_EVENT(PercussionOffEvent);
        PercussionOffEvent() = default;
        PercussionOffEvent(ModelDuration timeSinceLastEvent, Pitch pitch, Velocity velocity = 64)
            : PercussionEvent(timeSinceLastEvent, pitch, velocity) {}

        virtual bool operator==(const TrackEvent& other) const override;
        virtual std::size_t getHash() const override;
        virtual GroupingInfo getGroupingInfo() const override;
    };

} // namespace seqwires

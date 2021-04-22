#pragma once

#include "SeqwiresLib/Tracks/trackEvent.hpp"

namespace seqwires {

    /// Base of potentially set of note track events.
    struct NoteEvent : public TrackEvent {
        STREAM_EVENT_ABSTRACT(NoteEvent);

        static GroupingInfo::Category s_noteEventCategory;

        /// Common to all note events. All events of a group share this value.
        Pitch m_pitch;
    };

    struct NoteOnEvent : public NoteEvent {
        STREAM_EVENT(NoteOnEvent);
        virtual bool operator==(const TrackEvent& other) const override;
        virtual std::size_t getHash() const override;
        virtual GroupingInfo getGroupingInfo() const override;

        Velocity m_velocity;
    };

    struct NoteOffEvent : public NoteEvent {
        STREAM_EVENT(NoteOffEvent);
        virtual bool operator==(const TrackEvent& other) const override;
        virtual std::size_t getHash() const override;
        virtual GroupingInfo getGroupingInfo() const override;

        Velocity m_velocity;
    };

} // namespace seqwires

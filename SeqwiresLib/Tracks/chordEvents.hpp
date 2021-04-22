#pragma once

#include "SeqwiresLib/Tracks/trackEvent.hpp"

namespace seqwires {

    struct ChordEvent : public TrackEvent {
        STREAM_EVENT_ABSTRACT(ChordEvent);

        static GroupingInfo::Category s_chordEventCategory;
    };

    struct ChordOnEvent : public ChordEvent {
        STREAM_EVENT(ChordOnEvent);
        virtual bool operator==(const TrackEvent& other) const override;
        virtual std::size_t getHash() const override;
        virtual GroupingInfo getGroupingInfo() const override;
        PitchClass m_root;
        ChordType m_chordType;
    };

    struct ChordOffEvent : public ChordEvent {
        STREAM_EVENT(ChordOffEvent);
        virtual bool operator==(const TrackEvent& other) const override;
        virtual std::size_t getHash() const override;
        virtual GroupingInfo getGroupingInfo() const override;
    };

} // namespace seqwires

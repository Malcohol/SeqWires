/**
 * ChordEvents describe musical chords.
 * 
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include "SeqWiresLib/Tracks/trackEvent.hpp"

namespace seqwires {

    /// A ChordEvent describes a musical chord. 
    struct ChordEvent : public TrackEvent {
        STREAM_EVENT_ABSTRACT(ChordEvent);

        static GroupingInfo::Category s_chordEventCategory;
    };

    /// Describes the start of a chord.
    struct ChordOnEvent : public ChordEvent {
        STREAM_EVENT(ChordOnEvent);
        virtual bool operator==(const TrackEvent& other) const override;
        virtual std::size_t getHash() const override;
        virtual GroupingInfo getGroupingInfo() const override;
        virtual void transpose(int pitchOffset) override;
        PitchClass m_root;
        ChordType m_chordType;
    };

    /// The end of a chord.
    struct ChordOffEvent : public ChordEvent {
        STREAM_EVENT(ChordOffEvent);
        virtual bool operator==(const TrackEvent& other) const override;
        virtual std::size_t getHash() const override;
        virtual GroupingInfo getGroupingInfo() const override;
    };

} // namespace seqwires

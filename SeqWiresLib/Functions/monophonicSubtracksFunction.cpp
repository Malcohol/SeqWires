/**
 * A function which extracts a list of monophonic tracks from a track.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include "SeqWiresLib/Functions/monophonicSubtracksFunction.hpp"

#include "SeqWiresLib/Tracks/noteEvents.hpp"
#include "SeqWiresLib/Tracks/trackEventHolder.hpp"

seqwires::MonophonicSubtracksResult seqwires::getMonophonicSubtracks(const Track& trackIn, int numTracks) {
    assert(numTracks > 0);
    seqwires::MonophonicSubtracksResult result;

    struct TrackInfo {
        ModelDuration m_timeSinceLastEvent;
        TrackEvent::GroupingInfo::GroupValue m_activeValue = TrackEvent::GroupingInfo::c_notAValue;
    };
    std::vector<TrackInfo> trackInfos;
    trackInfos.resize(numTracks);
    result.m_noteTracks.resize(numTracks);
    ModelDuration timeSinceLastEventOther;

    using Group = std::tuple<TrackEvent::GroupingInfo::Category, TrackEvent::GroupingInfo::GroupValue>;
    const TrackEvent::GroupingInfo::Category noteCategory = NoteEvent::s_noteEventCategory;

    for (const auto& event : trackIn) {
        const TrackEvent::GroupingInfo info = event.getGroupingInfo();

        for (auto& t : trackInfos) {
            t.m_timeSinceLastEvent += event.getTimeSinceLastEvent();
        }
        timeSinceLastEventOther += event.getTimeSinceLastEvent();

        int trackToUse = -1;
        if (info.m_category == noteCategory) {
            // Always pick the earliest available slot.
            for (int i = 0; i < numTracks; ++i) {
                auto& t = trackInfos[i];
                if (t.m_activeValue == info.m_groupValue) {
                    trackToUse = i;
                    break;
                } else if (t.m_activeValue == TrackEvent::GroupingInfo::c_notAValue) {
                    if (trackToUse == -1) {
                        trackToUse = i;
                    }
                }
            }
        }

        if (trackToUse != -1) {
            auto& t = trackInfos[trackToUse];
            {
                TrackEventHolder newEvent = event;
                newEvent->setTimeSinceLastEvent(t.m_timeSinceLastEvent);
                result.m_noteTracks[trackToUse].addEvent(newEvent.release());
            }
            t.m_timeSinceLastEvent = 0;
            if (info.m_grouping == TrackEvent::GroupingInfo::Grouping::StartOfGroup) {
                // Too strong?
                assert(t.m_activeValue == TrackEvent::GroupingInfo::c_notAValue);
                t.m_activeValue = info.m_groupValue;
            } else if (info.m_grouping == TrackEvent::GroupingInfo::Grouping::EndOfGroup) {
                t.m_activeValue = TrackEvent::GroupingInfo::c_notAValue;
            }
        } else {
            TrackEventHolder newEvent = event;
            newEvent->setTimeSinceLastEvent(timeSinceLastEventOther);
            result.m_other.addEvent(newEvent.release());
            timeSinceLastEventOther = 0;
        }
    }

    for (int i = 0; i < numTracks; ++i) {
        result.m_noteTracks[i].setDuration(trackIn.getDuration());
    }
    result.m_other.setDuration(trackIn.getDuration());

    return result;
}

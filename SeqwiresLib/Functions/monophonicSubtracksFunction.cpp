/**
 * A function which extracts a list of monophonic tracks from a track.
 * 
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include "SeqwiresLib/Functions/monophonicSubtracksFunction.hpp"

#include "SeqwiresLib/Tracks/noteEvents.hpp"
#include "SeqwiresLib/Tracks/trackEventHolder.hpp"

std::vector<seqwires::Track> seqwires::getMonophonicSubtracks(const Track& trackIn, int maxNumTracks,
                                                              MonophonicSubtrackPolicy policy) {
    // TODO Policy

    struct TrackInfo {
        ModelDuration m_timeSinceLastEvent;
        TrackEvent::GroupingInfo::GroupValue m_activeValue = TrackEvent::GroupingInfo::c_notAValue;
    };
    std::vector<TrackInfo> trackInfos;
    std::vector<Track> tracksOut;
    ModelDuration durationSinceStart;

    using Group = std::tuple<TrackEvent::GroupingInfo::Category, TrackEvent::GroupingInfo::GroupValue>;
    const TrackEvent::GroupingInfo::Category noteCategory = NoteEvent::s_noteEventCategory;

    for (const auto& event : trackIn) {
        const TrackEvent::GroupingInfo info = event.getGroupingInfo();

        durationSinceStart += event.getTimeSinceLastEvent();
        for (auto& t : trackInfos) {
            t.m_timeSinceLastEvent += event.getTimeSinceLastEvent();
        }

        if (info.m_category != noteCategory) {
            continue;
        }

        int trackToUse = -1;
        int firstTrackWithNoActiveEvent = -1;
        for (int i = 0; i < trackInfos.size(); ++i) {
            auto& t = trackInfos[i];
            if (t.m_activeValue == info.m_groupValue) {
                trackToUse = i;
                break;
            } else if ((firstTrackWithNoActiveEvent == -1) &&
                       (t.m_activeValue == TrackEvent::GroupingInfo::c_notAValue)) {
                firstTrackWithNoActiveEvent = i;
            }
        }

        if (trackToUse == -1) {
            if (firstTrackWithNoActiveEvent == -1) {
                trackInfos.emplace_back();
                firstTrackWithNoActiveEvent = trackInfos.size() - 1;
                if (firstTrackWithNoActiveEvent < maxNumTracks) {
                    tracksOut.emplace_back();
                }
            }
            trackToUse = firstTrackWithNoActiveEvent;
        }

        if (trackToUse != -1) {
            auto& t = trackInfos[trackToUse];
            if (trackToUse < maxNumTracks) {
                TrackEventHolder newEvent = event;
                newEvent->setTimeSinceLastEvent(t.m_timeSinceLastEvent);
                tracksOut[trackToUse].addEvent(newEvent.release());
            }
            t.m_timeSinceLastEvent = 0;
            if (info.m_grouping == TrackEvent::GroupingInfo::Grouping::StartOfGroup) {
                // Too strong?
                assert(t.m_activeValue == (t.m_activeValue == TrackEvent::GroupingInfo::c_notAValue));
                t.m_activeValue = info.m_groupValue;
            } else if (info.m_grouping == TrackEvent::GroupingInfo::Grouping::EndOfGroup) {
                t.m_activeValue = TrackEvent::GroupingInfo::c_notAValue;
            }
        }
    }
    return tracksOut;
}

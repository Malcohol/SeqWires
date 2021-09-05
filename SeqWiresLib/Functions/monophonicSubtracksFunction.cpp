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

#include <algorithm>

namespace {
    struct TrackInfo {
        seqwires::ModelDuration m_timeSinceLastEvent;
        seqwires::TrackEvent::GroupingInfo::GroupValue m_activeValue = seqwires::TrackEvent::GroupingInfo::c_notAValue;
    };

    struct NoteEventInfo {
        /// Notes only.
        seqwires::TrackEventHolder m_event;
        /// This is used to ensure sorting is stable when other factors compare equal.
        int m_originalIndex;
    };

    const seqwires::TrackEvent::GroupingInfo::Category c_noteCategory = seqwires::NoteEvent::s_noteEventCategory;

    void moveEventToOtherTrack(seqwires::ModelDuration& timeSinceLastEventOther, seqwires::TrackEventHolder& event,
                               seqwires::MonophonicSubtracksResult& result) {
        event->setTimeSinceLastEvent(timeSinceLastEventOther);
        result.m_other.addEvent(event.release());
        timeSinceLastEventOther = 0;
    }

    void moveNoteEventToTrack(std::vector<TrackInfo>& trackInfos, NoteEventInfo& noteEvent, int trackToUse,
                              seqwires::MonophonicSubtracksResult& result) {
        const seqwires::TrackEvent::GroupingInfo groupInfo = noteEvent.m_event->getGroupingInfo();
        auto& t = trackInfos[trackToUse];
        noteEvent.m_event->setTimeSinceLastEvent(t.m_timeSinceLastEvent);
        result.m_noteTracks[trackToUse].addEvent(noteEvent.m_event.release());
        if (groupInfo.m_grouping == seqwires::TrackEvent::GroupingInfo::Grouping::StartOfGroup) {
            // Too strong?
            assert(t.m_activeValue == seqwires::TrackEvent::GroupingInfo::c_notAValue);
            t.m_activeValue = groupInfo.m_groupValue;
        } else if (groupInfo.m_grouping == seqwires::TrackEvent::GroupingInfo::Grouping::EndOfGroup) {
            t.m_activeValue = seqwires::TrackEvent::GroupingInfo::c_notAValue;
        }
        t.m_timeSinceLastEvent = 0;
    }

    int getTrackToUse(const std::vector<TrackInfo>& trackInfos, const seqwires::TrackEventHolder& event) {
        int trackToUse = -1;
        const auto& groupInfo = event->getGroupingInfo();
        if (groupInfo.m_category == c_noteCategory) {
            for (int i = 0; i < trackInfos.size(); ++i) {
                auto& t = trackInfos[i];
                if (t.m_activeValue == groupInfo.m_groupValue) {
                    trackToUse = i;
                    break;
                } else if ((trackToUse == -1) && (t.m_activeValue == seqwires::TrackEvent::GroupingInfo::c_notAValue) &&
                           (groupInfo.m_grouping == seqwires::TrackEvent::GroupingInfo::Grouping::StartOfGroup)) {
                    trackToUse = i;
                }
            }
        }
        return trackToUse;
    }

    void assignNoteEventsToTracks(std::vector<TrackInfo>& trackInfos, seqwires::ModelDuration& timeSinceLastEventOther,
                                  std::vector<NoteEventInfo>& noteEvents, seqwires::MonophonicSubtracksPolicy policy, seqwires::MonophonicSubtracksResult& result) {
        const bool preferHigherPitches = (policy == seqwires::MonophonicSubtracksPolicy::PreferHigherPitches);
        const auto lessThan = [preferHigherPitches](NoteEventInfo& a, NoteEventInfo& b) {
            const auto& groupInfoA = a.m_event->getGroupingInfo();
            const auto& groupInfoB = b.m_event->getGroupingInfo();
            if ((groupInfoA.m_grouping == seqwires::TrackEvent::GroupingInfo::Grouping::EndOfGroup) &&
                (groupInfoB.m_grouping == seqwires::TrackEvent::GroupingInfo::Grouping::StartOfGroup)) {
                return true;
            }
            if ((groupInfoA.m_grouping == seqwires::TrackEvent::GroupingInfo::Grouping::StartOfGroup) &&
                (groupInfoB.m_grouping == seqwires::TrackEvent::GroupingInfo::Grouping::EndOfGroup)) {
                return false;
            }
            if (groupInfoA.m_groupValue > groupInfoB.m_groupValue) {
                return preferHigherPitches;
            }
            if (groupInfoA.m_groupValue < groupInfoB.m_groupValue) {
                return !preferHigherPitches;
            }
            // Preserve the original order in other cases.
            return a.m_originalIndex < b.m_originalIndex;
        };

        std::sort(noteEvents.begin(), noteEvents.end(), lessThan);

        for (auto& noteEvent : noteEvents) {
            const int trackToUse = getTrackToUse(trackInfos, noteEvent.m_event);
            if (trackToUse != -1) {
                moveNoteEventToTrack(trackInfos, noteEvent, trackToUse, result);
            } else {
                moveEventToOtherTrack(timeSinceLastEventOther, noteEvent.m_event, result);
            }
        }
    }
} // namespace

seqwires::MonophonicSubtracksResult seqwires::getMonophonicSubtracks(const Track& trackIn, int numTracks, MonophonicSubtracksPolicy policy) {
    assert(numTracks > 0);
    seqwires::MonophonicSubtracksResult result;

    std::vector<TrackInfo> trackInfos;
    trackInfos.resize(numTracks);
    result.m_noteTracks.resize(numTracks);
    ModelDuration timeSinceLastEventOther;

    using Group = std::tuple<TrackEvent::GroupingInfo::Category, TrackEvent::GroupingInfo::GroupValue>;

    std::vector<NoteEventInfo> noteEventsNow;

    for (auto& event : trackIn) {
        if (event.getTimeSinceLastEvent() != 0) {
            assignNoteEventsToTracks(trackInfos, timeSinceLastEventOther, noteEventsNow, policy, result);
            noteEventsNow.clear();

            for (auto& t : trackInfos) {
                t.m_timeSinceLastEvent += event.getTimeSinceLastEvent();
            }
            timeSinceLastEventOther += event.getTimeSinceLastEvent();
        }

        const TrackEvent::GroupingInfo groupInfo = event.getGroupingInfo();
        if (groupInfo.m_category == c_noteCategory) {
            NoteEventInfo& noteInfo = noteEventsNow.emplace_back();
            noteInfo.m_event = event;
            noteInfo.m_originalIndex = noteEventsNow.size() - 1;
        } else {
            TrackEventHolder otherEvent = event;
            moveEventToOtherTrack(timeSinceLastEventOther, otherEvent, result);
        }
    }
    assignNoteEventsToTracks(trackInfos, timeSinceLastEventOther, noteEventsNow, policy, result);

    for (int i = 0; i < numTracks; ++i) {
        result.m_noteTracks[i].setDuration(trackIn.getDuration());
    }
    result.m_other.setDuration(trackIn.getDuration());

    return result;
}

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
#include "BabelWiresLib/Features/Path/fieldName.hpp"

#include <algorithm>
#include <set>


namespace {
    // TODO Encorporate this in boilerplate macros, somehow.
    const babelwires::FieldIdentifiersSource s_enums = {
        MONOPHONIC_SUBTRACK_POLICY(ENUM_ARGUMENTS_AS_INITIALIZERS)        
    };    
}

seqwires::MonophonicSubtracksPolicyEnum::MonophonicSubtracksPolicyEnum()
    : babelwires::RegisteredEnum<MonophonicSubtracksPolicyEnum>("MonophonicSubtracksPolicy", "Monophonic Subtracks Policy", 1, FIELD_NAME_VECTOR(s_enums), 0) {}


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

    using PitchSet = std::set<seqwires::TrackEvent::GroupingInfo::GroupValue>;

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

    void evictEvent(std::vector<TrackInfo>& trackInfos, int trackToUse, PitchSet& evictedPitches,
                    seqwires::MonophonicSubtracksResult& result) {
        auto& t = trackInfos[trackToUse];
        const seqwires::Pitch pitchToEvict = trackInfos[trackToUse].m_activeValue;
        assert((evictedPitches.find(pitchToEvict) == evictedPitches.end()) && "Evicting an already evicted pitch");
        evictedPitches.insert(pitchToEvict);
        result.m_noteTracks[trackToUse].addEvent(seqwires::NoteOffEvent{t.m_timeSinceLastEvent, pitchToEvict});
        t.m_activeValue = seqwires::TrackEvent::GroupingInfo::c_notAValue;
        t.m_timeSinceLastEvent = 0;
    }

    /// Returns [trackToUse, shouldEvict]
    std::tuple<int, bool> getTrackToUse(const std::vector<TrackInfo>& trackInfos,
                                        const seqwires::TrackEventHolder& event,
                                        seqwires::MonophonicSubtracksPolicyEnum::Value policy) {
        const bool isEvicting = (policy == seqwires::MonophonicSubtracksPolicyEnum::Value::HighEv) ||
                                (policy == seqwires::MonophonicSubtracksPolicyEnum::Value::LowEv);
        const bool preferHigherPitches = (policy == seqwires::MonophonicSubtracksPolicyEnum::Value::High) ||
                                         (policy == seqwires::MonophonicSubtracksPolicyEnum::Value::HighEv);
        int trackToUse = -1;
        bool shouldEvict = false;
        const auto& groupInfo = event->getGroupingInfo();
        assert(groupInfo.m_category == c_noteCategory);
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
        if (isEvicting && (trackToUse == -1)) {
            seqwires::Pitch bestEvictablePitch = preferHigherPitches ? std::numeric_limits<seqwires::Pitch>::max()
                                                                     : std::numeric_limits<seqwires::Pitch>::min();
            for (int i = 0; i < trackInfos.size(); ++i) {
                auto& t = trackInfos[i];
                if (((groupInfo.m_groupValue > t.m_activeValue) == preferHigherPitches) &&
                    ((t.m_activeValue < bestEvictablePitch) == preferHigherPitches)) {
                    trackToUse = i;
                    bestEvictablePitch = t.m_activeValue;
                    shouldEvict = true;
                }
            }
        }
        return {trackToUse, shouldEvict};
    }

    void assignNoteEventsToTracks(std::vector<TrackInfo>& trackInfos, seqwires::ModelDuration& timeSinceLastEventOther,
                                  std::vector<NoteEventInfo>& noteEvents, PitchSet& evictedPitches,
                                  seqwires::MonophonicSubtracksPolicyEnum::Value policy,
                                  seqwires::MonophonicSubtracksResult& result) {
        const bool preferHigherPitches = (policy == seqwires::MonophonicSubtracksPolicyEnum::Value::High) ||
                                         (policy == seqwires::MonophonicSubtracksPolicyEnum::Value::HighEv);
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
            const seqwires::TrackEvent::GroupingInfo groupInfo = noteEvent.m_event->getGroupingInfo();
            const auto it = evictedPitches.find(groupInfo.m_groupValue);
            if (it != evictedPitches.end()) {
                if (groupInfo.m_grouping == seqwires::TrackEvent::GroupingInfo::Grouping::EndOfGroup) {
                    evictedPitches.erase(it);
                } // else ignore this event.
            } else {
                const auto [trackToUse, shouldEvict] = getTrackToUse(trackInfos, noteEvent.m_event, policy);
                if (trackToUse != -1) {
                    if (shouldEvict) {
                        evictEvent(trackInfos, trackToUse, evictedPitches, result);
                    }
                    moveNoteEventToTrack(trackInfos, noteEvent, trackToUse, result);
                } else {
                    moveEventToOtherTrack(timeSinceLastEventOther, noteEvent.m_event, result);
                }
            }
        }
    }
} // namespace

seqwires::MonophonicSubtracksResult seqwires::getMonophonicSubtracks(const Track& trackIn, int numTracks,
                                                                     MonophonicSubtracksPolicyEnum::Value policy) {
    assert(numTracks > 0);
    seqwires::MonophonicSubtracksResult result;

    std::vector<TrackInfo> trackInfos;
    trackInfos.resize(numTracks);
    result.m_noteTracks.resize(numTracks);
    ModelDuration timeSinceLastEventOther;

    using Group = std::tuple<TrackEvent::GroupingInfo::Category, TrackEvent::GroupingInfo::GroupValue>;

    std::vector<NoteEventInfo> noteEventsNow;
    PitchSet evictedPitches;

    for (auto& event : trackIn) {
        if (event.getTimeSinceLastEvent() != 0) {
            assignNoteEventsToTracks(trackInfos, timeSinceLastEventOther, noteEventsNow, evictedPitches, policy,
                                     result);
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
    assignNoteEventsToTracks(trackInfos, timeSinceLastEventOther, noteEventsNow, evictedPitches, policy, result);

    for (int i = 0; i < numTracks; ++i) {
        result.m_noteTracks[i].setDuration(trackIn.getDuration());
    }
    result.m_other.setDuration(trackIn.getDuration());

    return result;
}

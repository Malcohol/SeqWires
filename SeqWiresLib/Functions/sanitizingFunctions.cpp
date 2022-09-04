/**
 * Functions which sanitize a track.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <SeqWiresLib/Functions/sanitizingFunctions.hpp>

#include <SeqWiresLib/Tracks/trackEventHolder.hpp>

#include <set>

seqwires::Track seqwires::removeZeroDurationGroups(const Track& trackIn) {

    using Group = std::tuple<TrackEvent::GroupingInfo::Category, TrackEvent::GroupingInfo::GroupValue>;

    // This is used to identify groups which have collapsed to zero duration, and so should be removed.
    std::set<Group> groupsStartingAtCurrentTime;
    std::vector<TrackEventHolder> eventsAtCurrentTime;
    ModelDuration currentTimeSinceLastEvent;

    Track trackOut;

    auto processEventsAtCurrentTime = [&trackOut, &eventsAtCurrentTime, &currentTimeSinceLastEvent]() {
        for (auto& event : eventsAtCurrentTime) {
            event->setTimeSinceLastEvent(currentTimeSinceLastEvent);
            trackOut.addEvent(event.release());
            currentTimeSinceLastEvent = 0;
        }
    };

    for (auto it = trackIn.begin(); it != trackIn.end(); ++it) {
        const ModelDuration timeSinceLastEvent = it->getTimeSinceLastEvent();

        if (timeSinceLastEvent > 0) {
            processEventsAtCurrentTime();
            groupsStartingAtCurrentTime.clear();
            eventsAtCurrentTime.clear();
            // It's possible that eventsAtCurrentTime was empty, so carry forward the currentTimeSinceLastEvent.
            currentTimeSinceLastEvent += timeSinceLastEvent;
        }
        bool doAddEvent = true;
        const TrackEvent::GroupingInfo info = it->getGroupingInfo();
        const Group group = {info.m_category, info.m_groupValue};
        if (info.m_grouping == TrackEvent::GroupingInfo::Grouping::StartOfGroup) {
            groupsStartingAtCurrentTime.insert(group);
        } else if (info.m_grouping == TrackEvent::GroupingInfo::Grouping::EndOfGroup) {
            const auto git = groupsStartingAtCurrentTime.find(group);
            if (git != groupsStartingAtCurrentTime.end()) {
                doAddEvent = false;
                // Remove the collapsed group backwards from the end.
                for (int i = eventsAtCurrentTime.size() - 1; i >= 0; --i) {
                    TrackEventHolder& event = eventsAtCurrentTime[i];
                    const TrackEvent::GroupingInfo eventInfo = event->getGroupingInfo();
                    const Group eventGroup = {eventInfo.m_category, eventInfo.m_groupValue};
                    if (eventGroup == group) {
                        eventsAtCurrentTime.erase(eventsAtCurrentTime.begin() + i);
                        if (eventInfo.m_grouping == TrackEvent::GroupingInfo::Grouping::StartOfGroup) {
                            // Don't remove proceeding events which happen to have the same group.
                            break;
                        }
                    }
                }
            }
        }
        if (doAddEvent) {
            eventsAtCurrentTime.emplace_back(*it);
        }
    }
    processEventsAtCurrentTime();
    trackOut.setDuration(trackIn.getDuration());
    return trackOut;
}

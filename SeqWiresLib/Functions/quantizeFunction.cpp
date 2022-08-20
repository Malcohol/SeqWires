/**
 * Quantize track events.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <SeqWiresLib/Functions/quantizeFunction.hpp>

#include <SeqWiresLib/Tracks/trackEventHolder.hpp>

#include <set>

namespace {
    seqwires::ModelDuration getIdealTime(seqwires::ModelDuration time, seqwires::ModelDuration beat) {
        // TODO
        return 0;
    }
} // namespace

seqwires::Track seqwires::quantize(const Track& trackIn, ModelDuration beat) {

    using Group = std::tuple<TrackEvent::GroupingInfo::Category, TrackEvent::GroupingInfo::GroupValue>;

    // This is used to identify groups which have collapsed to zero duration, and so should be removed.
    std::set<Group> groupsStartingAtCurrentTime;
    std::set<Group> collapsedGroupsAtCurrentTime;
    std::vector<TrackEventHolder> eventsAtCurrentTime;
    ModelDuration currentTimeSinceLastEvent;

    Track trackOut;

    // The absolute time of the current event in trackIn.
    ModelDuration trackInAbsoluteTime;
    ModelDuration trackOutAbsoluteTime;

    auto processEventsAtCurrentTime = [&trackOut, &eventsAtCurrentTime, &collapsedGroupsAtCurrentTime,
                                       &currentTimeSinceLastEvent, &trackOutAbsoluteTime]() {
        for (auto& event : eventsAtCurrentTime) {
            const TrackEvent::GroupingInfo info = event->getGroupingInfo();
            const Group group = {info.m_category, info.m_groupValue};
            if (collapsedGroupsAtCurrentTime.find(group) != collapsedGroupsAtCurrentTime.end()) {
                event->setTimeSinceLastEvent(currentTimeSinceLastEvent);
                trackOut.addEvent(event.release());
                trackOutAbsoluteTime += currentTimeSinceLastEvent;
                currentTimeSinceLastEvent = 0;
            }
        }
    };

    for (auto it = trackIn.begin(); it != trackIn.end(); ++it) {
        const ModelDuration timeSinceLastEvent = it->getTimeSinceLastEvent();

        ModelDuration newTimeSinceLastEvent;
        if (timeSinceLastEvent > 0) {
            trackInAbsoluteTime += timeSinceLastEvent;
            const ModelDuration idealTime = getIdealTime(trackInAbsoluteTime, beat);
            newTimeSinceLastEvent = idealTime - trackOutAbsoluteTime;
        }

        if (newTimeSinceLastEvent > 0) {
            processEventsAtCurrentTime();
            groupsStartingAtCurrentTime.clear();
            collapsedGroupsAtCurrentTime.clear();
            eventsAtCurrentTime.clear();
            // It's possible that all events were removed, so carry forward the currentTimeSinceLastEvent.
            currentTimeSinceLastEvent += newTimeSinceLastEvent;
        }

        const TrackEvent::GroupingInfo info = it->getGroupingInfo();
        const Group group = {info.m_category, info.m_groupValue};
        if (info.m_grouping == TrackEvent::GroupingInfo::Grouping::StartOfGroup) {
            groupsStartingAtCurrentTime.insert(group);
        } else if (info.m_grouping == TrackEvent::GroupingInfo::Grouping::EndOfGroup) {
            const auto git = groupsStartingAtCurrentTime.find(group);
            if (git != groupsStartingAtCurrentTime.end()) {
                collapsedGroupsAtCurrentTime.insert(*git);
                groupsStartingAtCurrentTime.erase(git);
            }
        }
        eventsAtCurrentTime.emplace_back(*it);
    }
    processEventsAtCurrentTime();
    const ModelDuration idealDuration = getIdealTime(trackIn.getDuration(), beat);
    trackOut.setDuration(idealDuration);
    return trackOut;
}

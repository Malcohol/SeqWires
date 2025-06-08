/**
 * The TrackBuilder ensures tracks are conformant as they are built.
 *
 * (C) 2025 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <MusicLib/Utilities/trackBuilder.hpp>

bw_music::TrackBuilder::TrackBuilder()
    : m_track(std::make_unique<Track>()) {}

bool bw_music::TrackBuilder::onNewEvent(const TrackEvent& event) {
    const TrackEvent::GroupingInfo groupInfo = event.getGroupingInfo();
    if (groupInfo.m_grouping == TrackEvent::GroupingInfo::Grouping::NotInGroup) {
        return true;
    }
    if (!m_eventsAtCurrentTime.empty()) {
        m_eventsAtCurrentTime.emplace_back(event);
        return false;
    }
    const auto it = m_activeGroups.find(groupInfo);
    if (it == m_activeGroups.end()) {
        if (groupInfo.m_grouping == TrackEvent::GroupingInfo::Grouping::StartOfGroup) {
            m_eventsAtCurrentTime.emplace_back(event);
            return false;
        }
    } else {
        if (groupInfo.m_grouping == TrackEvent::GroupingInfo::Grouping::EndOfGroup) {
            m_activeGroups.erase(it);
            return true;
        } else if (groupInfo.m_grouping == TrackEvent::GroupingInfo::Grouping::EnclosedInGroup) {
            return true;
        } else if (groupInfo.m_grouping == TrackEvent::GroupingInfo::Grouping::StartOfGroup) {
            m_eventsAtCurrentTime.emplace_back(event);
            return false;
        }
    }
    return false;
}

void bw_music::TrackBuilder::addEvent(const TrackEvent& event) {
    assert(m_track && "The TrackBuilder is already finished");
    if (event.getTimeSinceLastEvent() > 0) {
        processEventsAtCurrentTime();
    }
    if (onNewEvent(event)) {
        issueEvent(event);
    } else {
        m_timeSinceLastEvent += event.getTimeSinceLastEvent();
    }
}

void bw_music::TrackBuilder::addEvent(TrackEvent&& event) {
    assert(m_track && "The TrackBuilder is already finished");
    if (event.getTimeSinceLastEvent() > 0) {
        processEventsAtCurrentTime();
    }
    if (onNewEvent(event)) {
        issueEvent(std::move(event));
    } else {
        m_timeSinceLastEvent += event.getTimeSinceLastEvent();
    }
}

void bw_music::TrackBuilder::issueEvent(const TrackEvent& event) {
    if (m_timeSinceLastEvent > 0) {
        TrackEventHolder tmp = event;
        tmp->setTimeSinceLastEvent(event.getTimeSinceLastEvent() + m_timeSinceLastEvent);
        m_track->addEvent(tmp.release());
        m_timeSinceLastEvent = 0;
    } else {
        m_track->addEvent(event);
    }
}

void bw_music::TrackBuilder::issueEvent(TrackEvent&& event) {
    if (m_timeSinceLastEvent > 0) {
        event.setTimeSinceLastEvent(event.getTimeSinceLastEvent() + m_timeSinceLastEvent);
        m_timeSinceLastEvent = 0;
    }
    m_track->addEvent(std::move(event));
}

void bw_music::TrackBuilder::processEventsAtCurrentTime() {
    unsigned int i = 0;
    while (i < m_eventsAtCurrentTime.size()) {
        if (auto& event = m_eventsAtCurrentTime[i]) {
            const TrackEvent::GroupingInfo groupInfo = event->getGroupingInfo();
            const auto activeGroupIt = m_activeGroups.find(groupInfo);
            if (groupInfo.m_grouping == TrackEvent::GroupingInfo::Grouping::StartOfGroup) {
                // Check whether the start is followed by a matching end at the same time.
                unsigned int j = i;
                while (j < m_eventsAtCurrentTime.size()) {
                    if (auto& otherEvent = m_eventsAtCurrentTime[j]) {
                        const TrackEvent::GroupingInfo otherGroupInfo = otherEvent->getGroupingInfo();
                        if (groupInfo == otherGroupInfo) {
                            if (otherGroupInfo.m_grouping == TrackEvent::GroupingInfo::Grouping::EndOfGroup) {
                                for (int k = i + 1; k <= j; ++j) {
                                    if (auto& eventInRange = m_eventsAtCurrentTime[j]) {
                                        const TrackEvent::GroupingInfo eventInRangeGroupInfo =
                                            eventInRange->getGroupingInfo();
                                        if (eventInRangeGroupInfo == groupInfo) {
                                            if (activeGroupIt == m_activeGroups.end()) {
                                                // Zero-length group: Remove the group
                                                eventInRange.reset();
                                            } else {
                                                // Assume End/Start out-of-order: Reorder those events
                                                issueEvent(eventInRange.release());
                                            }
                                        }
                                    }
                                    if (activeGroupIt == m_activeGroups.end()) {
                                        // Zero-length group: Nothing to do here, since i will be skipped.
                                    } else {
                                        // Assume End/Start out-of-order: Reorder those events
                                        issueEvent(event.release());
                                    }
                                }
                                break;
                            }
                        }
                    }
                    ++j;
                }
                if (j == m_eventsAtCurrentTime.size()) {
                    // Unmatched start: Normal case
                    m_activeGroups.emplace(groupInfo);
                    issueEvent(event.release());
                }
            } else if (groupInfo.m_grouping == TrackEvent::GroupingInfo::Grouping::EndOfGroup) {
                if (activeGroupIt != m_activeGroups.end()) {
                    m_activeGroups.erase(activeGroupIt);
                    issueEvent(event.release());
                }
            } else {
                assert(groupInfo.m_grouping == TrackEvent::GroupingInfo::Grouping::EnclosedInGroup);
                if (activeGroupIt != m_activeGroups.end()) {
                    issueEvent(event.release());
                }
            }
        }
        ++i;
    }
    m_eventsAtCurrentTime.clear();
}

std::unique_ptr<bw_music::Track> bw_music::TrackBuilder::finishAndGetTrack() {
    processEventsAtCurrentTime();
    return std::move(m_track);
}

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
    if (event.getTimeSinceLastEvent() > 0) {
        m_existingGroups.merge(m_newGroups);
        m_newGroups.clear();
        m_possiblePendingStartEvents.clear();
    }
    const TrackEvent::GroupingInfo groupInfo = event.getGroupingInfo();
    if (groupInfo.m_grouping == TrackEvent::GroupingInfo::Grouping::NotInGroup) {
        return true;
    }
    const auto it = m_existingGroups.find(groupInfo);
    if (it == m_existingGroups.end()) {
        if (groupInfo.m_grouping == TrackEvent::GroupingInfo::Grouping::StartOfGroup) {
            auto [_, wasInserted] = m_newGroups.insert(groupInfo);
            return wasInserted;
        }
    } else {
        if (groupInfo.m_grouping == TrackEvent::GroupingInfo::Grouping::EndOfGroup) {
            m_existingGroups.erase(it);
            for (auto pit = m_possiblePendingStartEvents.begin(); pit != m_possiblePendingStartEvents.end(); ++pit) {
                if (event == **pit) {
                    m_actuallyPendingStartEvent = *pit;
                    m_possiblePendingStartEvents.erase(pit);
                    break;
                }
            }
            return true;
        } else if (groupInfo.m_grouping == TrackEvent::GroupingInfo::Grouping::EnclosedInGroup) {
            return true;
        } else if (groupInfo.m_grouping == TrackEvent::GroupingInfo::Grouping::StartOfGroup) {
            m_possiblePendingStartEvents.emplace_back(event)->setTimeSinceLastEvent(0);
            return false;
        }
    }
    const auto cit = m_newGroups.find(groupInfo);
    if (cit != m_newGroups.end()) {
        if (groupInfo.m_grouping == TrackEvent::GroupingInfo::Grouping::EndOfGroup) {
            // TODO: Zero length group: Delete!
            return true;
        } else if (groupInfo.m_grouping == TrackEvent::GroupingInfo::Grouping::EnclosedInGroup) {
          return true;
        }
    }
    return false;
}

void bw_music::TrackBuilder::addEvent(const TrackEvent& event) {
    assert(m_track && "The TrackBuilder is already finished");
    if (onNewEvent(event)) {
        if (m_extraTimeSinceLastEvent > 0) {
            TrackEventHolder tmp = event;
            tmp->setTimeSinceLastEvent(event.getTimeSinceLastEvent() + m_extraTimeSinceLastEvent);
            m_track->addEvent(tmp.release());
            m_extraTimeSinceLastEvent = 0;
        } else {
            m_track->addEvent(event);
        }
        if (m_actuallyPendingStartEvent.hasEvent()) {
            m_track->addEvent(m_actuallyPendingStartEvent.release());
        }
    } else {
        m_extraTimeSinceLastEvent += event.getTimeSinceLastEvent();
    }
}

void bw_music::TrackBuilder::addEvent(TrackEvent&& event) {
    assert(m_track && "The TrackBuilder is already finished");
    if (onNewEvent(event)) {
        if (m_extraTimeSinceLastEvent > 0) {
            event.setTimeSinceLastEvent(event.getTimeSinceLastEvent() + m_extraTimeSinceLastEvent);
            m_extraTimeSinceLastEvent = 0;
        }
        m_track->addEvent(std::move(event));
        if (m_actuallyPendingStartEvent.hasEvent()) {
            m_track->addEvent(m_actuallyPendingStartEvent.release());
        }
    } else {
        m_extraTimeSinceLastEvent += event.getTimeSinceLastEvent();
    }
}

std::unique_ptr<bw_music::Track> bw_music::TrackBuilder::finishAndGetTrack() {
    return std::move(m_track);
}

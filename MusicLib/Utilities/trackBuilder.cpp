/**
 * The TrackBuilder ensures tracks are conformant as they are built.
 *
 * (C) 2025 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <MusicLib/Utilities/trackBuilder.hpp>

bw_music::TrackBuilder::TrackBuilder()
    : m_track(std::make_unique<Track>()) {
    }

bool bw_music::TrackBuilder::onNewEvent(const TrackEvent& event) {
    const TrackEvent::GroupingInfo groupInfo = event.getGroupingInfo();
    if (groupInfo.m_grouping == TrackEvent::GroupingInfo::Grouping::NotInGroup) {
        return true;
    }
    const auto it = m_groupState.find(groupInfo);
    if (it == m_groupState.end()) {
        if (groupInfo.m_grouping == TrackEvent::GroupingInfo::Grouping::StartOfGroup) {
            m_groupState.insert({groupInfo, GroupState::InGroup});
            return true;
        }
    } else if (it->second == GroupState::NotInGroup) {
        if (groupInfo.m_grouping == TrackEvent::GroupingInfo::Grouping::StartOfGroup) {
            it->second = GroupState::InGroup;
            return true;
        }
    } else {
        if (groupInfo.m_grouping == TrackEvent::GroupingInfo::Grouping::EndOfGroup) {
            it->second = GroupState::NotInGroup;
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
        m_track->addEvent(event);
    }
}

void bw_music::TrackBuilder::addEvent(TrackEvent&& event) {
    assert(m_track && "The TrackBuilder is already finished");
    if (onNewEvent(event)) {
        m_track->addEvent(std::move(event));
    }
}

std::unique_ptr<bw_music::Track> bw_music::TrackBuilder::finishAndGetTrack() {
    return std::move(m_track);
}

/**
 * Tracks describe a sequence of events, typically notes and chords.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <MusicLib/Types/Track/track.hpp>

#include <Common/Hash/hash.hpp>

#include <map>

bw_music::Track::Track() = default;

bw_music::Track::Track(ModelDuration duration) {
    setDuration(duration);
}

int bw_music::Track::getNumEvents() const {
    return m_blockStream.getNumEvents();
}

bw_music::ModelDuration bw_music::Track::getDuration() const {
    return m_duration;
}

bw_music::ModelDuration bw_music::Track::getTotalEventDuration() const {
    return m_totalEventDuration;
}

void bw_music::Track::setDuration(ModelDuration d) {
    if (d > getTotalEventDuration()) {
        m_duration = d;
    }
}

std::size_t bw_music::Track::getHash() const {
    // The duration can be changed without invalidating cached info. But hash will change.
    std::size_t hash = m_eventHash;
    babelwires::hash::mixInto(hash, m_duration);
    return hash;
}

bool bw_music::Track::operator==(const Value& other) const {
    const Track* const otherTrack = other.as<Track>();
    if (!otherTrack) {
        return false;
    }
    if (otherTrack->getNumEvents() != getNumEvents()) {
        return false;
    }
    if (otherTrack->m_duration != m_duration) {
        return false;
    }
    if (otherTrack->getHash() != getHash()) {
        return false;
    }
    auto thisIt = begin();
    auto otherIt = otherTrack->begin();
    while (thisIt != end()) {
        assert(otherIt != otherTrack->end());
        if (*thisIt != *otherIt) {
            return false;
        }
        ++thisIt;
        ++otherIt;
    }
    assert(otherIt == otherTrack->end());
    return true;
}

void bw_music::Track::addEvent(const TrackEvent& event) {
    onNewEvent(m_blockStream.addEvent(event));
}

void bw_music::Track::addEvent(TrackEvent&& event) {
    onNewEvent(m_blockStream.addEvent(std::move(event)));
};

void bw_music::Track::onNewEvent(const TrackEvent& event) {
    m_totalEventDuration += event.getTimeSinceLastEvent();
    babelwires::hash::mixInto(m_eventHash, event.getHash());
    TrackEvent::GroupingInfo groupingInfo = event.getGroupingInfo();
    if ((groupingInfo.m_grouping == TrackEvent::GroupingInfo::Grouping::NotInGroup) ||
        (groupingInfo.m_grouping == TrackEvent::GroupingInfo::Grouping::StartOfGroup)) {
        ++m_numEventGroupsByCategory[groupingInfo.m_category];
    }
    if (getTotalEventDuration() > m_duration) {
        m_duration = getTotalEventDuration();
    }
}

bw_music::Track::const_iterator bw_music::Track::end() const {
    return m_blockStream.end_impl<TrackEvent>();
}

bw_music::Track::const_iterator bw_music::Track::begin() const {
    return m_blockStream.begin_impl<TrackEvent>();
}

const std::unordered_map<const char*, int>& bw_music::Track::getNumEventGroupsByCategory() const {
    return m_numEventGroupsByCategory;
}

bool bw_music::Track::validate(
#ifndef NDEBUG
    bool assertIfInvalid
#endif
) const {
    std::map<TrackEvent::EventGroup, ModelDuration> activeGroups;
    for (const auto& e : *this) {
        if (e.getTimeSinceLastEvent() > 0) {
            for (auto& times : activeGroups) {
                times.second += e.getTimeSinceLastEvent();
            }
        }
        const auto groupInfo = e.getGroupingInfo();
        auto activeGroupIt = activeGroups.find(groupInfo);
        switch (groupInfo.m_grouping) {
            case TrackEvent::GroupingInfo::Grouping::StartOfGroup: {
                const bool noActiveGroup = (activeGroupIt == activeGroups.end());
                assert((!assertIfInvalid || noActiveGroup) &&
                       "Encountered a start event when there was already a matching group.");
                if (!noActiveGroup) {
                    return false;
                }
                activeGroups.emplace(groupInfo, 0);
                break;
            }
            case TrackEvent::GroupingInfo::Grouping::EnclosedInGroup: {
                const bool alreadyAGroup = (activeGroupIt != activeGroups.end());
                assert((!assertIfInvalid || alreadyAGroup) &&
                       "Encountered an enclosed event when there was no matching group.");
                if (!alreadyAGroup) {
                    return false;
                }
                break;
            }
            case TrackEvent::GroupingInfo::Grouping::EndOfGroup: {
                const bool alreadyAGroup = (activeGroupIt != activeGroups.end());
                const bool positiveDurationGroup = (activeGroupIt->second > 0);
                assert((!assertIfInvalid || alreadyAGroup) &&
                       "Encountered an end event when there was no matching group.");
                assert(positiveDurationGroup && "Encountered a zero-duration group");
                if (!alreadyAGroup || positiveDurationGroup) {
                    return false;
                }
                break;
            }
            case TrackEvent::GroupingInfo::Grouping::NotInGroup:
            default:
                break;
        }
    }
    return true;
}

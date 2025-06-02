/**
 * Tracks describe a sequence of events, typically notes and chords.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <MusicLib/Types/Track/track.hpp>

#include <Common/Hash/hash.hpp>

bw_music::Track::Track() = default;

bw_music::Track::Track(ModelDuration duration) {
    setDuration(duration);
}

int bw_music::Track::getNumEvents() const {
    return m_blockStream.getNumEvents();
}

bw_music::Track::CachedValues::CachedValues(ModelDuration trackDuration)
    : m_hash(trackDuration.getHash()) {}

void bw_music::Track::CachedValues::addEvent(const TrackEvent& event) {
    m_totalEventDuration += event.getTimeSinceLastEvent();
    babelwires::hash::mixInto(m_hash, event.getHash());
    TrackEvent::GroupingInfo groupingInfo = event.getGroupingInfo();
    if ((groupingInfo.m_grouping == TrackEvent::GroupingInfo::Grouping::NotInGroup) ||
        (groupingInfo.m_grouping == TrackEvent::GroupingInfo::Grouping::StartOfGroup)) {
        ++m_numEventGroupsByCategory[groupingInfo.m_category];
    }
}

void bw_music::Track::ensureCache() const {
    if (!m_cacheIsValid) {
        CachedValues newCache(getDuration());
        for (const TrackEvent& event : *this) {
            newCache.addEvent(event);
        }
        m_cachedValues = newCache;
        m_cacheIsValid = true;
    }
}

bw_music::ModelDuration bw_music::Track::getDuration() const {
    return m_duration;
}

bw_music::ModelDuration bw_music::Track::getTotalEventDuration() const {
    ensureCache();
    return m_cachedValues.m_totalEventDuration;
}

void bw_music::Track::setDuration(ModelDuration d) {
    if (d > getTotalEventDuration()) {
        m_duration = d;
    }
}

std::size_t bw_music::Track::getHash() const {
    ensureCache();
    return m_cachedValues.m_hash;
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

void bw_music::Track::onNewEvent(const TrackEvent& event) {
    ensureCache();
    m_cachedValues.addEvent(event);
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
    ensureCache();
    return m_cachedValues.m_numEventGroupsByCategory;
}

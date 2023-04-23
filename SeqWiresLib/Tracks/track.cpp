/**
 * Tracks describe a sequence of events, typically notes and chords.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <SeqWiresLib/Tracks/track.hpp>

#include <Common/Hash/hash.hpp>

int seqwires::Track::getNumEvents() const {
    return m_blockStream.getNumEvents();
}

seqwires::Track::CachedValues::CachedValues(ModelDuration trackDuration)
    : m_hash(trackDuration.getHash()) {}

void seqwires::Track::CachedValues::addEvent(const TrackEvent& event) {
    m_totalEventDuration += event.getTimeSinceLastEvent();
    babelwires::hash::mixInto(m_hash, event.getHash());
    TrackEvent::GroupingInfo groupingInfo = event.getGroupingInfo();
    if ((groupingInfo.m_grouping == TrackEvent::GroupingInfo::Grouping::NotInGroup) ||
        (groupingInfo.m_grouping == TrackEvent::GroupingInfo::Grouping::StartOfGroup)) {
        ++m_numEventGroupsByCategory[groupingInfo.m_category];
    }
}

void seqwires::Track::ensureCache() const {
    if (!m_cacheIsValid) {
        CachedValues newCache(getDuration());
        for (const TrackEvent& event : *this) {
            newCache.addEvent(event);
        }
        m_cachedValues = newCache;
        m_cacheIsValid = true;
    }
}

seqwires::ModelDuration seqwires::Track::getDuration() const {
    return m_duration;
}

seqwires::ModelDuration seqwires::Track::getTotalEventDuration() const {
    ensureCache();
    return m_cachedValues.m_totalEventDuration;
}

void seqwires::Track::setDuration(ModelDuration d) {
    if (d > getTotalEventDuration()) {
        m_duration = d;
    }
}

std::size_t seqwires::Track::getHash() const {
    ensureCache();
    return m_cachedValues.m_hash;
}

std::string seqwires::Track::toString() const {
    return m_duration.toString();
}

bool seqwires::Track::operator==(const Value& other) const {
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

void seqwires::Track::onNewEvent(const TrackEvent& event) {
    ensureCache();
    m_cachedValues.addEvent(event);
    if (getTotalEventDuration() > m_duration) {
        m_duration = getTotalEventDuration();
    }
}

seqwires::Track::const_iterator seqwires::Track::end() const {
    return m_blockStream.end_impl<TrackEvent>();
}

seqwires::Track::const_iterator seqwires::Track::begin() const {
    return m_blockStream.begin_impl<TrackEvent>();
}

const std::unordered_map<const char*, int>& seqwires::Track::getNumEventGroupsByCategory() const {
    ensureCache();
    return m_cachedValues.m_numEventGroupsByCategory;
}

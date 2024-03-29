/**
 * PercussionEvents describe musical percussions.
 * 
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <SeqWiresLib/Types/Track/TrackEvents/percussionEvents.hpp>

#include <Common/Hash/hash.hpp>

#include <sstream>
#include <typeinfo>
#include <algorithm>

seqwires::TrackEvent::GroupingInfo::Category seqwires::PercussionEvent::s_percussionEventCategory = "Percussion";

bool seqwires::PercussionOnEvent::operator==(const TrackEvent& other) const {
    if (typeid(other) != typeid(*this)) {
        return false;
    }
    auto otherOn = static_cast<const PercussionOnEvent&>(other);
    return (m_timeSinceLastEvent == otherOn.m_timeSinceLastEvent) && (m_instrument == otherOn.m_instrument) &&
           (m_velocity == otherOn.m_velocity);
}

std::size_t seqwires::PercussionOnEvent::getHash() const {
    return babelwires::hash::mixtureOf(static_cast<const char*>("PercOn"), m_timeSinceLastEvent, m_instrument, m_velocity);
}

seqwires::TrackEvent::GroupingInfo seqwires::PercussionOnEvent::getGroupingInfo() const {
    return {s_percussionEventCategory, m_instrument.toCode(), GroupingInfo::Grouping::StartOfGroup};
}

bool seqwires::PercussionOffEvent::operator==(const TrackEvent& other) const {
    if (typeid(other) != typeid(*this)) {
        return false;
    }
    auto otherOff = static_cast<const PercussionOffEvent&>(other);
    return (m_timeSinceLastEvent == otherOff.m_timeSinceLastEvent) && (m_instrument == otherOff.m_instrument) &&
           (m_velocity == otherOff.m_velocity);
}

std::size_t seqwires::PercussionOffEvent::getHash() const {
    return babelwires::hash::mixtureOf(static_cast<const char*>("PercOff"), m_timeSinceLastEvent, m_instrument, m_velocity);
}

seqwires::TrackEvent::GroupingInfo seqwires::PercussionOffEvent::getGroupingInfo() const {
    return {s_percussionEventCategory, m_instrument.toCode(), GroupingInfo::Grouping::EndOfGroup};
}

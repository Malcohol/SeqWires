/**
 * ChordEvents describe musical chords.
 * 
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include "SeqWiresLib/Tracks/chordEvents.hpp"

#include "Common/Utilities/hash.hpp"

#include <sstream>
#include <typeinfo>

seqwires::TrackEvent::GroupingInfo::Category seqwires::ChordEvent::s_chordEventCategory = "Chords";

bool seqwires::ChordOnEvent::operator==(const TrackEvent& other) const {
    if (typeid(other) != typeid(this)) {
        return false;
    }
    auto otherOn = static_cast<const ChordOnEvent&>(other);
    return (m_timeSinceLastEvent == otherOn.m_timeSinceLastEvent) && (m_root == otherOn.m_root) &&
           (m_chordType == otherOn.m_chordType);
}

std::size_t seqwires::ChordOnEvent::getHash() const {
    return babelwires::hash::mixtureOf(static_cast<const char*>("ChordOn"), m_timeSinceLastEvent, m_root, m_chordType);
}

seqwires::TrackEvent::GroupingInfo seqwires::ChordOnEvent::getGroupingInfo() const {
    return {s_chordEventCategory, 0, GroupingInfo::Grouping::StartOfGroup};
}

bool seqwires::ChordOffEvent::operator==(const TrackEvent& other) const {
    if (typeid(other) != typeid(this)) {
        return false;
    }
    auto otherOn = static_cast<const ChordOffEvent&>(other);
    return (m_timeSinceLastEvent == otherOn.m_timeSinceLastEvent);
}

std::size_t seqwires::ChordOffEvent::getHash() const {
    return babelwires::hash::mixtureOf(static_cast<const char*>("ChordOff"), m_timeSinceLastEvent);
}

seqwires::TrackEvent::GroupingInfo seqwires::ChordOffEvent::getGroupingInfo() const {
    return {s_chordEventCategory, 0, GroupingInfo::Grouping::StartOfGroup};
}
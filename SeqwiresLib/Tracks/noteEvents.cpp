/**
 * NoteEvents describe musical notes.
 * 
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include "SeqwiresLib/Tracks/noteEvents.hpp"

#include "Common/Utilities/hash.hpp"

#include <sstream>
#include <typeinfo>

seqwires::TrackEvent::GroupingInfo::Category seqwires::NoteEvent::s_noteEventCategory = "Notes";

bool seqwires::NoteOnEvent::operator==(const TrackEvent& other) const {
    if (typeid(other) != typeid(this)) {
        return false;
    }
    auto otherOn = static_cast<const NoteOnEvent&>(other);
    return (m_timeSinceLastEvent == otherOn.m_timeSinceLastEvent) && (m_pitch == otherOn.m_pitch) &&
           (m_velocity == otherOn.m_velocity);
}

std::size_t seqwires::NoteOnEvent::getHash() const {
    return babelwires::hash::mixtureOf(static_cast<const char*>("NoteOn"), m_timeSinceLastEvent, m_pitch, m_velocity);
}

seqwires::TrackEvent::GroupingInfo seqwires::NoteOnEvent::getGroupingInfo() const {
    return {s_noteEventCategory, m_pitch, GroupingInfo::Grouping::StartOfGroup};
}

bool seqwires::NoteOffEvent::operator==(const TrackEvent& other) const {
    if (typeid(other) != typeid(this)) {
        return false;
    }
    auto otherOff = static_cast<const NoteOffEvent&>(other);
    return (m_timeSinceLastEvent == otherOff.m_timeSinceLastEvent) && (m_pitch == otherOff.m_pitch) &&
           (m_velocity == otherOff.m_velocity);
}

std::size_t seqwires::NoteOffEvent::getHash() const {
    return babelwires::hash::mixtureOf(static_cast<const char*>("NoteOff"), m_timeSinceLastEvent, m_pitch, m_velocity);
}

seqwires::TrackEvent::GroupingInfo seqwires::NoteOffEvent::getGroupingInfo() const {
    return {s_noteEventCategory, m_pitch, GroupingInfo::Grouping::EndOfGroup};
}

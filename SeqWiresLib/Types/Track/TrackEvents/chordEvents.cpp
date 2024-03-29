/**
 * ChordEvents describe musical chords.
 * 
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <SeqWiresLib/Types/Track/TrackEvents/chordEvents.hpp>

#include <Common/Hash/hash.hpp>

#include <sstream>
#include <typeinfo>

seqwires::TrackEvent::GroupingInfo::Category seqwires::ChordEvent::s_chordEventCategory = "Chords";

bool seqwires::ChordOnEvent::operator==(const TrackEvent& other) const {
    if (typeid(other) != typeid(*this)) {
        return false;
    }
    auto otherOn = static_cast<const ChordOnEvent&>(other);
    return (m_timeSinceLastEvent == otherOn.m_timeSinceLastEvent) && (m_chord == otherOn.m_chord);
}

std::size_t seqwires::ChordOnEvent::getHash() const {
    return babelwires::hash::mixtureOf(static_cast<const char*>("ChordOn"), m_timeSinceLastEvent, m_chord.m_root, m_chord.m_chordType);
}

seqwires::TrackEvent::GroupingInfo seqwires::ChordOnEvent::getGroupingInfo() const {
    return {s_chordEventCategory, 0, GroupingInfo::Grouping::StartOfGroup};
}

void seqwires::ChordOnEvent::transpose(int pitchOffset) {
    assert(pitchOffset <= 127);
    assert(pitchOffset >= -127);
    static_assert(static_cast<unsigned int>(PitchClass::Value::NUM_VALUES) == 12);
    unsigned int rootPitch = (static_cast<unsigned int>(m_chord.m_root) + 132 + pitchOffset) % 12;
    m_chord.m_root = static_cast<PitchClass::Value>(rootPitch);
}

bool seqwires::ChordOffEvent::operator==(const TrackEvent& other) const {
    if (typeid(other) != typeid(*this)) {
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
/**
 * An abstraction for Enums of percussion instruments which can map to pitch values.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <BabelWiresPlugins/Smf/Plugin/Percussion/percussionSet.hpp>

#include <unordered_set>

namespace {
    /// Removes duplicates.
    babelwires::Enum::EnumValues getUniqueEnumValues(const std::vector<babelwires::Identifier>& values) {
        babelwires::Enum::EnumValues enumValues;
        std::unordered_set<babelwires::Identifier> alreadySeen;
        for (auto id : values) {
            if (alreadySeen.find(id) == alreadySeen.end()) {
                enumValues.emplace_back(id);
                alreadySeen.insert(id);
            }
        }
        return enumValues;
    }
} // namespace

smf::PercussionSet::PercussionSet(babelwires::LongIdentifier identifier, babelwires::VersionNumber version,
                                  InstrumentBlock instruments,
                                  seqwires::Pitch pitchOfDefaultInstrument)
    : Enum(identifier, version, getUniqueEnumValues(instruments.m_instruments), pitchOfDefaultInstrument - instruments.m_pitchOfLowestInstrument) {
    seqwires::Pitch pitch = instruments.m_pitchOfLowestInstrument;
    for (auto id : instruments.m_instruments) {
        if (m_instrumentToPitch.find(id) == m_instrumentToPitch.end()) {
            // In the case of duplicates, use the first pitch to match the sequence of the EnumValues.
            m_instrumentToPitch[id] = pitch;
        } else {
            assert((pitch != pitchOfDefaultInstrument) && "You can only use the lowest pitch of a duplicate instrument as the default value");
        }
        m_pitchToInstrument[pitch] = id;
        ++pitch;
    }
}

std::optional<seqwires::Pitch> smf::PercussionSet::tryGetPitchFromInstrument(babelwires::Identifier identifier) const {
    const auto it = m_instrumentToPitch.find(identifier);
    if(it != m_instrumentToPitch.end()) {
        return it->second;
    }
    return {};
}

std::optional<babelwires::Identifier> smf::PercussionSet::tryGetInstrumentFromPitch(seqwires::Pitch pitch) const {
    const auto it = m_pitchToInstrument.find(pitch);
    if (it != m_pitchToInstrument.end()) {
        return it->second;
    }
    return {};
}

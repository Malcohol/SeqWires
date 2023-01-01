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
                                  std::vector<babelwires::Identifier> values, unsigned int indexOfDefaultValue,
                                  seqwires::Pitch pitchOfLowestInstrument)
    : Enum(identifier, version, getUniqueEnumValues(values), indexOfDefaultValue) {
    seqwires::Pitch pitch = pitchOfLowestInstrument;
    for (auto id : values) {
        // In the case of duplicates, allow later pitches to override earlier pitches.
        m_instrumentToPitch[id] = pitch;
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

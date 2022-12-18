/**
 * An abstraction for Enums of percussion instruments which can map to pitch values.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <BabelWiresPlugins/Smf/Plugin/Percussion/percussionSet.hpp>

smf::PercussionSet::PercussionSet(babelwires::LongIdentifier identifier,
                                                      babelwires::VersionNumber version, EnumValues values,
                                                      unsigned int indexOfDefaultValue,
                                                      seqwires::Pitch pitchOfLowestInstrument)
    : Enum(identifier, version, values, indexOfDefaultValue)
    , m_pitchOfLowestInstrument(pitchOfLowestInstrument) {}

std::optional<seqwires::Pitch>
smf::PercussionSet::tryGetPitchFromInstrument(babelwires::Identifier identifier) const {
    const int index = tryGetIndexFromIdentifier(identifier);
    if (index != -1) {
        return index + m_pitchOfLowestInstrument;
    }
    return {};
}

std::optional<babelwires::Identifier>
smf::PercussionSet::tryGetInstrumentFromPitch(seqwires::Pitch pitch) const {
    const EnumValues& enumValues = getEnumValues();
    if ((pitch >= m_pitchOfLowestInstrument) && (pitch <= enumValues.size() + m_pitchOfLowestInstrument)) {
        return enumValues[pitch - m_pitchOfLowestInstrument];
    }
    return {};
}

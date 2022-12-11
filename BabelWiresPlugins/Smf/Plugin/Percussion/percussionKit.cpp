/**
 * An abstraction for Enums of percussion instruments which can map to pitch values.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <BabelWiresPlugins/Smf/Plugin/Percussion/percussionKit.hpp>

smf::PercussionKit::PercussionKit(babelwires::LongIdentifier identifier, babelwires::VersionNumber version,
                                  EnumValues values, unsigned int indexOfDefaultValue)
    : Enum(identifier, version, values, indexOfDefaultValue) {}

smf::ContiguousPercussionKit::ContiguousPercussionKit(babelwires::LongIdentifier identifier,
                                                      babelwires::VersionNumber version, EnumValues values,
                                                      unsigned int indexOfDefaultValue,
                                                      seqwires::Pitch pitchOfLowestInstrument)
    : PercussionKit(identifier, version, values, indexOfDefaultValue)
    , m_pitchOfLowestInstrument(pitchOfLowestInstrument) {}

std::optional<seqwires::Pitch>
smf::ContiguousPercussionKit::tryGetPitchFromInstrument(babelwires::Identifier identifier) const {
    const int index = tryGetIndexFromIdentifier(identifier);
    if (index != -1) {
        return index + m_pitchOfLowestInstrument;
    }
    return {};
}

std::optional<babelwires::Identifier>
smf::ContiguousPercussionKit::tryGetInstrumentFromPitch(seqwires::Pitch pitch) const {
    const EnumValues& enumValues = getEnumValues();
    if ((pitch >= m_pitchOfLowestInstrument) && (pitch <= enumValues.size() + m_pitchOfLowestInstrument)) {
        return enumValues[pitch - m_pitchOfLowestInstrument];
    }
    return {};
}

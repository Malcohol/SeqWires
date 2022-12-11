/**
 * Defines enums for standard percussion sets.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <SeqWiresLib/percussion.hpp>

#include <Common/Identifiers/identifierRegistry.hpp>

ENUM_DEFINE_ENUM_VALUE_SOURCE(GM_PERCUSSION_VALUES);
ENUM_DEFINE_ENUM_VALUE_SOURCE(GM2_STANDARD_PERCUSSION_VALUES_LO);
ENUM_DEFINE_ENUM_VALUE_SOURCE(GM2_STANDARD_PERCUSSION_VALUES_HI);

namespace {
    babelwires::Enum::EnumValues buildGM2EnumValues(const seqwires::GMPercussionKit& gmPercussionKit) {
        babelwires::Enum::EnumValues values;
        const std::size_t numValues = static_cast<std::size_t>(seqwires::GM2StandardPercussionKit::Value::NUM_VALUES);
        values.reserve(numValues);
        babelwires::Enum::EnumValues lo = ENUM_IDENTIFIER_VECTOR(GM2_STANDARD_PERCUSSION_VALUES_LO);
        const babelwires::Enum::EnumValues& mid = gmPercussionKit.getEnumValues();
        babelwires::Enum::EnumValues hi = ENUM_IDENTIFIER_VECTOR(GM2_STANDARD_PERCUSSION_VALUES_HI);
        values.insert(values.end(), lo.begin(), lo.end());
        values.insert(values.end(), mid.begin(), mid.end());
        values.insert(values.end(), hi.begin(), hi.end());
        return values;
    }
}

seqwires::PercussionKit::PercussionKit(babelwires::LongIdentifier identifier, babelwires::VersionNumber version,
                                       EnumValues values, unsigned int indexOfDefaultValue)
    : Enum(identifier, version, values, indexOfDefaultValue) {}


seqwires::GMPercussionKit::GMPercussionKit()
    : PercussionKit(getThisIdentifier(), 1, ENUM_IDENTIFIER_VECTOR(GM_PERCUSSION_VALUES), 0) {}

babelwires::LongIdentifier seqwires::GMPercussionKit::getThisIdentifier() {
    return REGISTERED_LONGID("GMPercussion", "General MIDI Percussion", "9f6dec59-4d75-4a4b-9cba-e7ff76390c5f");
}

std::optional<seqwires::Pitch> seqwires::GMPercussionKit::tryGetPitchFromInstrument(babelwires::Identifier identifier) const {
    const int index = tryGetIndexFromIdentifier(identifier);
    if (index != -1) {
        return index + 35;
    }
    return {};
}

std::optional<babelwires::Identifier> seqwires::GMPercussionKit::tryGetInstrumentFromPitch(Pitch pitch) const {
    if ((pitch >= 35) && (pitch <= 81)) {
        return getEnumValues()[pitch - 35];
    }
    return {};
}

seqwires::GM2StandardPercussionKit::GM2StandardPercussionKit(const GMPercussionKit& gmPercussionKit)
    : PercussionKit(getThisIdentifier(), 1, buildGM2EnumValues(gmPercussionKit), 0) {}

babelwires::LongIdentifier seqwires::GM2StandardPercussionKit::getThisIdentifier() {
    return REGISTERED_LONGID("GM2Percussion", "General MIDI 2 Percussion", "9fc0c107-f76c-432a-af58-c794f01df455");
}

std::optional<seqwires::Pitch> seqwires::GM2StandardPercussionKit::tryGetPitchFromInstrument(babelwires::Identifier identifier) const {
    const int index = tryGetIndexFromIdentifier(identifier);
    if (index != -1) {
        return index + 27;
    }
    return {};
}

std::optional<babelwires::Identifier> seqwires::GM2StandardPercussionKit::tryGetInstrumentFromPitch(Pitch pitch) const {
    if ((pitch >= 27) && (pitch <= 87)) {
        return getEnumValues()[pitch - 35];
    }
    return {};
}
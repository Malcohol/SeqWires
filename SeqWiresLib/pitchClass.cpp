/**
 * Defines a data type for pitch class.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <SeqWiresLib/pitchClass.hpp>

#include <Common/Identifiers/identifierRegistry.hpp>

ENUM_DEFINE_ENUM_VALUE_SOURCE(PITCH_CLASS_VALUES);

seqwires::PitchClass::PitchClass()
    : Enum(ENUM_IDENTIFIER_VECTOR(PITCH_CLASS_VALUES), 0) {}

#define JUST_NAMES(A, B, C) B, 

namespace {
    const char* s_pitchClassNames[static_cast<unsigned int>(seqwires::PitchClass::Value::NUM_VALUES)] = {
        PITCH_CLASS_VALUES(JUST_NAMES)
    };
} // namespace

std::string seqwires::PitchClass::valueToString(PitchClass::Value p) {
    return s_pitchClassNames[static_cast<unsigned int>(p)];
}

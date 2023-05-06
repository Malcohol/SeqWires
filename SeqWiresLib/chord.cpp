/**
 * Defines a data type for chords.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <SeqWiresLib/chord.hpp>

#include <Common/Identifiers/identifierRegistry.hpp>

ENUM_DEFINE_ENUM_VALUE_SOURCE(CHORD_TYPE_VALUES);

seqwires::ChordType::ChordType()
    : EnumType(ENUM_IDENTIFIER_VECTOR(CHORD_TYPE_VALUES), 0) {}

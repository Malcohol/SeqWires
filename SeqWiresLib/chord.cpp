/**
 * Defines a data type for chords.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include "SeqWiresLib/chord.hpp"

ENUM_DEFINE_ENUM_VALUE_SOURCE(CHORD_TYPE_VALUES);

seqwires::ChordType::ChordType()
: RegisteredEnum<ChordType>(REGISTERED_LONGID("ChordType", "Chord Type", "c63ea174-1562-4cb5-a456-d6c0bd89e335"), 1, ENUM_IDENTIFIER_VECTOR(CHORD_TYPE_VALUES), 0) {}

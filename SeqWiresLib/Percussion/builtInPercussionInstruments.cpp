/**
 * Defines enums for standard percussion sets.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <SeqWiresLib/Percussion/builtInPercussionInstruments.hpp>

ENUM_DEFINE_ENUM_VALUE_SOURCE(seqwires::BuiltInPercussionInstruments, BUILT_IN_PERCUSSION_INSTRUMENTS);

seqwires::BuiltInPercussionInstruments::BuiltInPercussionInstruments()
    : EnumType(getStaticValueSet(), 0) {}


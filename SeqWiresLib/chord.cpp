/**
 * Defines a data type for chords.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include "SeqWiresLib/chord.hpp"

namespace {

#define CHORD_TYPE_SELECT_SECOND_ARGUMENT(A, B, C) B,
    const char* s_chordTypeNames[static_cast<std::uint8_t>(seqwires::ChordType::Value::NUM_CHORD_TYPES)] = {
        CHORD_TYPE_VALUES(CHORD_TYPE_SELECT_SECOND_ARGUMENT)
    };
} // namespace


std::string seqwires::chordTypeToString(ChordType::Value t) {
    return s_chordTypeNames[static_cast<std::uint8_t>(t)];
}

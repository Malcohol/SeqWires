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
    const char* s_chordTypeNames[seqwires::ChordType::NUM_CHORD_TYPES] = {
        CHORD_TYPE_VALUES(CHORD_TYPE_SELECT_SECOND_ARGUMENT)
    };
} // namespace


std::string seqwires::chordTypeToString(ChordType t) {
    return s_chordTypeNames[t];
}

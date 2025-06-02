/**
 * Defines a data type for chords.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <MusicLib/chord.hpp>

#include <Common/Identifiers/identifierRegistry.hpp>

ENUM_DEFINE_ENUM_VALUE_SOURCE(seqwires::ChordType, CHORD_TYPE_VALUES);

seqwires::ChordType::ChordType()
    : EnumType(getStaticValueSet(), 0) {}

seqwires::NoChord::NoChord()
    : EnumType({getNoChordValue()}, 0) {}

babelwires::ShortId seqwires::NoChord::getNoChordValue() {
    return BW_SHORT_ID("NoChrd","No Chord", "026e8e78-bb05-4386-8a67-2034890acd6e");
}

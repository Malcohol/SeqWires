/**
 * Defines enums for standard percussion sets.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <SeqWiresLib/builtInPercussionInstruments.hpp>

#include <Common/Identifiers/identifierRegistry.hpp>

ENUM_DEFINE_ENUM_VALUE_SOURCE(BUILT_IN_PERCUSSION_INSTRUMENTS);

seqwires::BuiltInPercussionInstruments::BuiltInPercussionInstruments()
    : Enum(getThisIdentifier(), 1, ENUM_IDENTIFIER_VECTOR(BUILT_IN_PERCUSSION_INSTRUMENTS), 0) {}

babelwires::LongIdentifier seqwires::BuiltInPercussionInstruments::getThisIdentifier() {
    return REGISTERED_LONGID("BuiltInPercussion", "Built In Percussion", "c67f1a9e-653d-42b5-bf73-1fdc2f8a9b1a");
}

babelwires::Enum::EnumValues
seqwires::BuiltInPercussionInstruments::convertToIdentifiers(const std::vector<Value>& values) const {
    babelwires::Enum::EnumValues identifiers;
    std::transform(
        values.cbegin(), values.cend(), std::back_inserter(identifiers),
        [this](seqwires::BuiltInPercussionInstruments::Value value) { return getIdentifierFromValue(value); });
    return identifiers;
}

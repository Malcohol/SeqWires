/**
 * AbstractPercussionSet is an empty Enum which is acts as the common type of Enums of percussion instruments.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <SeqWiresLib/Percussion/abstractPercussionSet.hpp>

#include <Common/Identifiers/registeredIdentifier.hpp>

seqwires::AbstractPercussionSet::AbstractPercussionSet()
    : babelwires::Enum({}, 0) {}

bool seqwires::AbstractPercussionSet::isAbstract() const {
    return true;
}

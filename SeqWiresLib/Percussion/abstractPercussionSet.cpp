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
    : babelwires::Enum(getThisIdentifier(), 1, {}, 0) {}

babelwires::LongIdentifier seqwires::AbstractPercussionSet::getThisIdentifier() {
    return REGISTERED_LONGID("AbstractPercussionSet", "AbstractPercussionSet",
                             "f81e2125-28e5-485a-88c0-500b5c1dc31e");
}

bool seqwires::AbstractPercussionSet::isAbstract() const {
    return true;
}

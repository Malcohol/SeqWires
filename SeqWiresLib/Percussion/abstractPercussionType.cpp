/**
 * AbstractPercussionType is an empty Enum which is acts as the common type of Enums of percussion instruments.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <SeqWiresLib/Percussion/abstractPercussionType.hpp>

#include <Common/Identifiers/registeredIdentifier.hpp>

seqwires::AbstractPercussionType::AbstractPercussionType()
: babelwires::Enum(getThisIdentifier(), 1, {}, 0) {
}

babelwires::LongIdentifier seqwires::AbstractPercussionType::getThisIdentifier() {
    return REGISTERED_LONGID("AbstractPercussionType", "AbstractPercussionType", "f81e2125-28e5-485a-88c0-500b5c1dc31e");
}

bool seqwires::AbstractPercussionType::isAbstract() const { 
    return true;
}

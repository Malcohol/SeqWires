/**
 * AbstractPercussionSet is an empty Enum which is acts as the common type of Enums of percussion instruments.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <SeqWiresLib/Percussion/abstractPercussionSet.hpp>

#include <Common/Identifiers/registeredIdentifier.hpp>

babelwires::Type::Tag seqwires::percussionTypeTag() {
    return BW_MEDIUM_ID("PercussionTag", "Percussion Type", "116e6017-9710-42e0-9464-daae91c3fc12");
}

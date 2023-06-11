/**
 * AbstractPercussionSet is an empty Enum which is acts as the common type of Enums of percussion instruments.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <BabelWiresLib/TypeSystem/primitiveType.hpp>
#include <BabelWiresLib/Types/Enum/enumType.hpp>

namespace seqwires {

    /// Percussion types should use this tag.
    babelwires::Type::Tag percussionTypeTag();

} // namespace seqwires

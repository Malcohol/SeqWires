/**
 * AbstractPercussionType is an empty Enum which is acts as the common type of Enums of percussion instruments.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <BabelWiresLib/Enums/enum.hpp>

namespace seqwires {
    /// Any supertype of this enum is assumed to carry values suitable for use as percussion instruments.
    /// Note: The supertype relationship is not automatic. A type has to declare itself a supertype of this
    /// type (or one of its existing supertypes).
    class AbstractPercussionType : public babelwires::Enum {
      public:
        AbstractPercussionType();

        virtual bool isAbstract() const override;

        static babelwires::LongIdentifier getThisIdentifier();
    };
} // namespace seqwires

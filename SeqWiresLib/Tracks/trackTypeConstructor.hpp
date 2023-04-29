/**
 * A TypeConstructor which constructs a track type with an initial duration.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <SeqWiresLib/Tracks/trackType.hpp>
#include <SeqWiresLib/musicTypes.hpp>

#include <BabelWiresLib/TypeSystem/typeConstructor.hpp>

namespace seqwires {
    /// Construct a track type with an initial duration.
    class TrackTypeConstructor : public babelwires::TypeConstructor {
      public:
        /// Note that the we don't represent the default in the name.
        TYPE_CONSTRUCTOR("Track", "Track", "bba2f18c-9ff1-406c-b39c-47a2e4047caa", 1);

        std::unique_ptr<babelwires::Type> constructType(babelwires::TypeRef newTypeRef, const std::vector<const babelwires::Type*>& typeArguments,
                                            const std::vector<babelwires::EditableValueHolder>& valueArguments) const override;

        babelwires::SubtypeOrder compareSubtypeHelper(const babelwires::TypeSystem& typeSystem, const babelwires::TypeConstructorArguments& argumentsA,
                                          const babelwires::TypeConstructorArguments& argumentsB) const override;

        babelwires::SubtypeOrder compareSubtypeHelper(const babelwires::TypeSystem& typeSystem, const babelwires::TypeConstructorArguments& arguments,
                                          const babelwires::TypeRef& other) const override;

      private:
        /// Throws a TypeSystem exception if the arguments are not of the expect type.
        static ModelDuration
        extractValueArguments(const std::vector<babelwires::EditableValueHolder>& valueArguments);
    };
} // namespace babelwires
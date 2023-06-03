/**
 * A TypeConstructor which constructs a track type with an initial duration.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <SeqWiresLib/Types/Track/trackTypeConstructor.hpp>

#include <BabelWiresLib/TypeSystem/typeSystemException.hpp>
#include <BabelWiresLib/Types/Rational/rationalValue.hpp>

seqwires::ModelDuration seqwires::TrackTypeConstructor::extractValueArguments(
    const std::vector<babelwires::EditableValueHolder>& valueArguments) {
    if (valueArguments.size() != 1) {
        throw babelwires::TypeSystemException()
            << "TrackTypeConstructor expects 1 value arguments but got " << valueArguments.size();
    }

    if (const babelwires::RationalValue* rationalValue = valueArguments[0]->as<babelwires::RationalValue>()) {
        return rationalValue->get();
    } else {
        throw babelwires::TypeSystemException() << "Argument 0 given to TrackTypeConstructor was not a RationalValue";
    }
}

std::unique_ptr<babelwires::Type> seqwires::TrackTypeConstructor::constructType(const babelwires::TypeSystem& typeSystem, 
    babelwires::TypeRef newTypeRef, const std::vector<const babelwires::Type*>& typeArguments,
    const std::vector<babelwires::EditableValueHolder>& valueArguments) const {
    if (typeArguments.size() != 0) {
        throw babelwires::TypeSystemException()
            << "TrackTypeConstructor does not expect type arguments but got " << typeArguments.size();
    }
    ModelDuration initialDuration = extractValueArguments(valueArguments);
    return std::make_unique<babelwires::ConstructedType<TrackType>>(std::move(newTypeRef), initialDuration);
}

babelwires::SubtypeOrder
seqwires::TrackTypeConstructor::compareSubtypeHelper(const babelwires::TypeSystem& typeSystem,
                                                     const babelwires::TypeConstructorArguments& argumentsA,
                                                     const babelwires::TypeConstructorArguments& argumentsB) const {
    return babelwires::SubtypeOrder::IsEquivalent;
}

babelwires::SubtypeOrder
seqwires::TrackTypeConstructor::compareSubtypeHelper(const babelwires::TypeSystem& typeSystem,
                                                     const babelwires::TypeConstructorArguments& arguments,
                                                     const babelwires::TypeRef& other) const {
    const babelwires::Type* const type = other.tryResolve(typeSystem);
    if (!type) {
        return babelwires::SubtypeOrder::IsUnrelated;
    }
    return type->as<TrackType>() ? babelwires::SubtypeOrder::IsEquivalent : babelwires::SubtypeOrder::IsUnrelated;
}

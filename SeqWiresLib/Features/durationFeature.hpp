/**
 * The DurationFeature is an RationalFeature describing a duration in whole note units.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <BabelWiresLib/Types/Rational/rationalFeature.hpp>

namespace seqwires {

    /// The DurationFeature is an RationalFeature describing a duration in whole note units.
    class DurationFeature : public babelwires::RationalFeature {
      public:
        DurationFeature()
            : babelwires::RationalFeature(0, std::numeric_limits<babelwires::Rational::ComponentType>::max()) {}
    };

} // namespace seqwires

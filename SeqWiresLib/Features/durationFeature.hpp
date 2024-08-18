/**
 * The DurationFeature is an RationalFeature describing a duration in whole note units.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <SeqWiresLib/Types/duration.hpp>

#include <BabelWiresLib/Features/simpleValueFeature.hpp>

namespace seqwires {

    /// The DurationFeature is an RationalFeature describing a duration in whole note units.
    class DurationFeature : public babelwires::SimpleValueFeature {
      public:
        DurationFeature()
            : babelwires::SimpleValueFeature(Duration::getThisIdentifier()) {}
    };

} // namespace seqwires

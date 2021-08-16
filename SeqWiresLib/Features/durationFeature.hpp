/**
 * The DurationFeature is an RationalFeature describing a duration in whole note units.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include "BabelWires/Features/featureMixins.hpp"
#include "BabelWires/Features/numericFeature.hpp"

namespace seqwires {

    /// The DurationFeature is an RationalFeature describing a duration in whole note units.
    class DurationFeature
        : public babelwires::HasStaticRange<babelwires::RationalFeature, 0, std::numeric_limits<int>::max()> {};

} // namespace seqwires

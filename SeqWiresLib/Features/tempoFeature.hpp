/**
 * The TempoFeature is an IntFeature for holding a tempo value in bpm.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include "BabelWiresLib/Features/featureMixins.hpp"
#include "BabelWiresLib/Features/numericFeature.hpp"

namespace seqwires {

    /// The TempoFeature is an IntFeature for holding a tempo value in bpm.
    /// Tempos are limited to integral bpm.
    /// The default tempo is 120.
    class TempoFeature
        : public babelwires::HasStaticDefault<babelwires::HasStaticRange<babelwires::IntFeature, 0, 255>, 120> {};

} // namespace seqwires

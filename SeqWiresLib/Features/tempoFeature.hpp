/**
 * The TempoFeature is an IntFeature for holding a tempo value in bpm.
 * 
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include "BabelWires/Features/numericFeature.hpp"

namespace seqwires {

    /// The TempoFeature is an IntFeature for holding a tempo value in bpm.
    /// Tempos are limited to integral bpm.
    class TempoFeature : public babelwires::IntFeature {
      protected:
        virtual void doSetToDefault() override;
    };

} // namespace seqwires

#pragma once

#include "BabelWires/Features/numericFeature.hpp"

namespace seqwires {

    /// The model limits tempos to integral bpm.
    class TempoFeature : public babelwires::IntFeature {
      protected:
        virtual void doSetToDefault() override;
    };

} // namespace seqwires

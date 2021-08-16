/**
 * The PitchFeature is an IntFeature holding a single MIDI note value.
 * 
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include "BabelWires/Features/numericFeature.hpp"
#include "BabelWires/Features/featureMixins.hpp"

namespace seqwires {

    /// The PitchFeature is an IntFeature holding a single MIDI note value.
    /// The default value is middle C (60).
    class PitchFeature : public babelwires::HasStaticDefault<babelwires::HasStaticRange<babelwires::IntFeature, 0, 127>, 60> {
      protected:
        virtual const babelwires::ValueNames* getValueNames() const override;
    };

} // namespace seqwires

/**
 * The TempoFeature is an IntFeature for holding a tempo value in bpm.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <BabelWiresLib/Features/simpleValueFeature.hpp>

#include <SeqWiresLib/Types/tempo.hpp>

namespace seqwires {

    // TODO Remove: No longer necessary.
    /// The TempoFeature is an IntFeature for holding a tempo value in bpm.
    /// Tempos are limited to integral bpm.
    /// The default tempo is 120.
    class TempoFeature : public babelwires::SimpleValueFeature {
      public:
        TempoFeature()
            : babelwires::SimpleValueFeature(Tempo::getThisIdentifier()) {}
    };

} // namespace seqwires

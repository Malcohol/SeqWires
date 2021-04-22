#pragma once

#include "BabelWires/Features/heavyValueFeature.hpp"
#include "SeqwiresLib/Tracks/track.hpp"

namespace seqwires {

    /// A features which carries a track.
    class TrackFeature : public babelwires::HeavyValueFeature<Track> {
      protected:
        std::string doGetValueType() const override;
    };

} // namespace seqwires

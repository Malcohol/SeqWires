/**
 * The TrackFeature is a ValueFeature which carries a track of sequence data.
 * 
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <BabelWiresLib/Features/heavyValueFeature.hpp>
#include <SeqWiresLib/Tracks/track.hpp>

namespace seqwires {

    /// A features which carries a track.
    class TrackFeature : public babelwires::HeavyValueFeature<Track> {
      protected:
        std::string doGetValueType() const override;
    };

} // namespace seqwires

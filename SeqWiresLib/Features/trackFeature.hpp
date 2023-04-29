/**
 * The TrackFeature is a ValueFeature which carries a track of sequence data.
 * 
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <BabelWiresLib/Features/simpleValueFeature.hpp>

#include <SeqWiresLib/Tracks/track.hpp>
#include <SeqWiresLib/Tracks/trackType.hpp>

namespace seqwires {

    /// A features which carries a track.
    class TrackFeature : public babelwires::SimpleValueFeature {
      public:
        TrackFeature();

        const Track& get() const;
        void set(Track track);
    };

} // namespace seqwires

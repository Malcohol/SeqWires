#pragma once

#include "SeqwiresLib/Tracks/track.hpp"

namespace seqwires {
    enum class MonophonicSubtrackPolicy {
        // TODO Add more policies
        EarliestNote
    };

    // UNTESTED
    std::vector<Track> getMonophonicSubtracks(const Track& trackIn, int maxNumTracks = -1,
                                              MonophonicSubtrackPolicy policy = MonophonicSubtrackPolicy::EarliestNote);
} // namespace seqwires

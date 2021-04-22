/**
 * A function which extracts a list of monophonic tracks from a track.
 * 
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include "SeqWiresLib/Tracks/track.hpp"

namespace seqwires {
    enum class MonophonicSubtrackPolicy {
        // TODO Add more policies
        EarliestNote
    };

    // UNTESTED
    std::vector<Track> getMonophonicSubtracks(const Track& trackIn, int maxNumTracks = -1,
                                              MonophonicSubtrackPolicy policy = MonophonicSubtrackPolicy::EarliestNote);
} // namespace seqwires

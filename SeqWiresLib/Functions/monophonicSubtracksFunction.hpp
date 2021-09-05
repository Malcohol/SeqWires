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
    enum class MonophonicSubtracksPolicy {
        PreferHigherPitches,
        PreferLowerPitches
    };

    struct MonophonicSubtracksResult {
        std::vector<Track> m_noteTracks;
        Track m_other;
    };

    MonophonicSubtracksResult getMonophonicSubtracks(const Track& trackIn, int numTracks, MonophonicSubtracksPolicy policy);
} // namespace seqwires

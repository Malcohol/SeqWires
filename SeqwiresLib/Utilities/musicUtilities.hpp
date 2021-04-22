/**
 * Some utility functions useful for manipulating music sequence data.
 * 
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

namespace seqwires {
    class Track;

    /// Get the minimum denominator sufficient to represent the durations of all events in the track.
    int getMinimumDenominator(const Track& track);
} // namespace seqwires

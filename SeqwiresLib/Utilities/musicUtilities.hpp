#pragma once

namespace seqwires {
    class Track;

    /// Get the minimum denominator sufficient to represent the durations of all events in the track.
    int getMinimumDenominator(const Track& track);
} // namespace seqwires

#pragma once

#include "SeqwiresLib/Tracks/track.hpp"

#include <memory>

namespace seqwires {
    /// Groups which start before the excerpt are dropped.
    /// Groups which finish after the excerpt are truncated.
    std::unique_ptr<Track> getTrackExcerpt(const seqwires::Track& trackIn, ModelDuration start, ModelDuration duration);
} // namespace seqwires

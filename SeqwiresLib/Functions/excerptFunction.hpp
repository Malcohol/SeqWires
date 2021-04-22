/**
 * A function which extracts a section of sequence data from a track.
 * 
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include "SeqwiresLib/Tracks/track.hpp"

#include <memory>

namespace seqwires {
    /// A function which extracts a section of sequence data from a track.
    /// Groups which start before the excerpt are dropped.
    /// Groups which finish after the excerpt are truncated.
    std::unique_ptr<Track> getTrackExcerpt(const seqwires::Track& trackIn, ModelDuration start, ModelDuration duration);
} // namespace seqwires

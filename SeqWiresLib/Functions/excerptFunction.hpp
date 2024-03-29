/**
 * A function which extracts a section of sequence data from a track.
 * 
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <SeqWiresLib/Types/Track/track.hpp>

#include <memory>

namespace seqwires {
    /// A function which extracts a section of sequence data from a track.
    /// Groups which start before the excerpt are dropped.
    /// Groups which finish after the excerpt are truncated.
    Track getTrackExcerpt(const Track& trackIn, ModelDuration start, ModelDuration duration);
} // namespace seqwires

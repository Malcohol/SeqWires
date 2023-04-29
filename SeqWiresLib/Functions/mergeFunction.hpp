/**
 * Function which merges tracks together.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <SeqWiresLib/Types/Track/track.hpp>

namespace seqwires {
    /// Merge the events of the sourceTracks into targetTrack.
    Track mergeTracks(const std::vector<const Track*>& sourceTracks);
} // namespace seqwires

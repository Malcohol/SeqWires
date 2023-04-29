/**
 * Functions which sanitize a track.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <SeqWiresLib/Types/Track/track.hpp>

#include <memory>

namespace seqwires {
    /// Remove any groups which have zero duration.
    Track removeZeroDurationGroups(const Track& trackIn);
} // namespace seqwires

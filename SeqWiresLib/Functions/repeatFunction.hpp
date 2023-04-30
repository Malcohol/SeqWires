/**
 * A function which repeats sequence data a number of times.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <SeqWiresLib/Types/Track/track.hpp>

#include <memory>

namespace seqwires {
    /// A function which repeats a section of sequence data a number of times.
    Track repeatTrack(const Track& trackIn, int count);
} // namespace seqwires

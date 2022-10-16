/**
 * Quantize track events.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <SeqWiresLib/Tracks/track.hpp>

namespace seqwires {
    /// Move the time at which events occur to the nearest beat.
    Track quantize(const Track& trackIn, ModelDuration beat);
}

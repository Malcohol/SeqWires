#pragma once

#include "Common/BlockStream/streamEventHolder.hpp"
#include "SeqwiresLib/Tracks/trackEvent.hpp"

namespace seqwires {
    // This is very common, so give it a name.
    using TrackEventHolder = babelwires::StreamEventHolder<TrackEvent>;
} // namespace seqwires

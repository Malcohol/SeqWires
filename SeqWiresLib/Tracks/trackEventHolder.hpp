/**
 * A TrackEventHolder is a container for a single mutable TrackEvent.
 * 
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include "Common/BlockStream/streamEventHolder.hpp"
#include "SeqWiresLib/Tracks/trackEvent.hpp"

namespace seqwires {
    /// A TrackEventHolder is a container for a single mutable TrackEvent.
    using TrackEventHolder = babelwires::StreamEventHolder<TrackEvent>;
} // namespace seqwires

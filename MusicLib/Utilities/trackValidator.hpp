/**
 * The TrackValidator checks that the events and groups in a track are correct.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <MusicLib/Types/Track/track.hpp>

namespace bw_music {
    class Track;

    /// Check that the following requirements are met:
    /// * There are no group events outside a group.
    /// * All groups must have strictly positive duration
    bool isTrackValid(const Track& track);

#ifndef NDEBUG
    void assertTrackIsValid(const Track& track);
#endif

} // namespace bw_music

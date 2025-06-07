/**
 * The TrackBuilder constructs conformant tracks.
 *
 * (C) 2025 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <MusicLib/Types/Track/track.hpp>

namespace bw_music {
    class TrackBuilder {
      public:
        TrackBuilder();

        /// Add a TrackEvent by moving or copying it into the track.
        template <typename EVENT, typename = std::enable_if_t<std::is_convertible_v<EVENT&, const TrackEvent&>>>
        void addEvent(EVENT&& srcEvent) {
            if (validateEvent(srcEvent)) {
                onNewEvent(m_track.addEvent(std::forward<EVENT>(srcEvent)));
            }
        };

        void validateEvent(const TrackEvent& event);
        std::unique_ptr<Track> finish();

      private:
        std::unique_ptr<Track> m_track;
    };
} // namespace bw_music

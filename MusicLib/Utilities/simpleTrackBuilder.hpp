/**
 * The SimpleTrackBuilder ensures tracks are valid by asserting.
 *
 * (C) 2025 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <MusicLib/Types/Track/track.hpp>
#include <MusicLib/Utilities/trackValidator.hpp>

#include <set>

namespace bw_music {
    /// The SimpleTrackBuilder ensures tracks are valid by asserting in finish.
    /// This is intended for cases where you know the events being added are valid.
    class SimpleTrackBuilder {
      public:
        /// Add a TrackEvent by moving or copying it into the track.
        template <typename EVENT>
        void addEvent(EVENT&& event) {
            m_track.addEvent(std::forward<EVENT>(event));
        };

        void setDuration(ModelDuration d) {
            m_track.setDuration(d);
        }

        Track finishAndGetTrack() {
            assertTrackIsValid(m_track);
            return std::move(m_track);
        }

      private:
        Track m_track;
    };
} // namespace bw_music

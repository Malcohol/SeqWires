/**
 * The TrackBuilder ensures tracks are conformant as they are built.
 *
 * (C) 2025 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <MusicLib/Types/Track/track.hpp>

/// Ensures the following:
/// * All groups must have strictly positive duration
/// * No overlapping groups of the same category and value.
namespace bw_music {
    class TrackBuilder {
      public:
        TrackBuilder();

        /// Add a TrackEvent by moving or copying it into the track.
        void addEvent(const TrackEvent& event);
        void addEvent(TrackEvent&& event);

        std::unique_ptr<Track> finishAndGetTrack();

      private:
        bool onNewEvent(const TrackEvent& event);

      private:
        std::unique_ptr<Track> m_track;

        enum class GroupState {
            InGroup,
            NotInGroup,
            NotPresent
        };

        std::unordered_map<TrackEvent::EventGroup, GroupState> m_groupState;
    };
} // namespace bw_music

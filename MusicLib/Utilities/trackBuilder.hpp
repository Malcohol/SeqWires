/**
 * The TrackBuilder ensures tracks are conformant as they are built.
 *
 * (C) 2025 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <MusicLib/Types/Track/track.hpp>
#include <MusicLib/Types/Track/TrackEvents/trackEventHolder.hpp>

#include <set>

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

        std::set<TrackEvent::EventGroup> m_existingGroups;
        std::set<TrackEvent::EventGroup> m_newGroups;

        /// When events are dropped, their time gets added to the next actual event.
        ModelDuration m_extraTimeSinceLastEvent;

        /// Start events which occurred when there was an already a matching group.
        /// This is sometimes because events at the same time get added out of sequence.
        /// Temporarily keep them until we've either seen corresponding end events or time has
        /// passed.
        std::vector<TrackEventHolder> m_possiblePendingStartEvents;

        /// A start event that happened at the same time as an end event. Add it directly after the
        /// end event.
        TrackEventHolder m_actuallyPendingStartEvent;
    };
} // namespace bw_music


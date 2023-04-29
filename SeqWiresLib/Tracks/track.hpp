/**
 * Tracks describe a sequence of events, typically notes and chords.
 * 
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <SeqWiresLib/Tracks/trackEvent.hpp>
#include <SeqWiresLib/musicTypes.hpp>

#include <BabelWiresLib/TypeSystem/value.hpp>

#include <Common/types.hpp>

#include <cassert>
#include <unordered_map>
#include <vector>

namespace seqwires {
    /// A track carries a stream of TrackEvents.
    class Track : public babelwires::Value {
      public:
        CLONEABLE(Track);

        Track();
        /// Create an empty track with a given duration.
        Track(ModelDuration duration);

        /// Add a TrackEvent by moving or copying it into the track.
        template <typename EVENT, typename = std::enable_if_t<std::is_convertible_v<EVENT&, const TrackEvent&>>>
        void addEvent(EVENT&& srcEvent) {
            onNewEvent(m_blockStream.addEvent(std::forward<EVENT>(srcEvent)));
        };

        /// Get the total number of events in the track.
        int getNumEvents() const;

        /// The duration of the track. Note: this can exceed the total duration of the events.
        ModelDuration getDuration() const;

        /// Sets the track to have the given duration, unless that is shorter than the total duration of the events,
        /// in which case, the operation is ignored.
        void setDuration(ModelDuration d);

        /// Return the total duration of the events in the track (which may be smaller than m_duration).
        ModelDuration getTotalEventDuration() const;

        /// Get a hash corresponding to the state of the track's contents
        std::size_t getHash() const override;

        std::string toString() const override;

        /// Determine whether the data in this track and the other are exactly the same.
        /// Note: This is unconcerned with how events are laid out in their streams.
        bool operator==(const Value& other) const override;

        /// Get a summary of the track contents, by category.
        const std::unordered_map<const char*, int>& getNumEventGroupsByCategory() const;

      public:
        using const_iterator = babelwires::BlockStream::Iterator<const babelwires::BlockStream, const TrackEvent>;
        const_iterator begin() const;
        const_iterator end() const;

        // IMPORTANT: Don't offer non-const begin and end. They would have to invalidate the cache, and might
        // be chosen instead of the const versions when iterating. Instead, offer an accessor that returns
        // a span.
        // using iterator = seqwires::BlockStream::Iterator<seqwires::BlockStream, TrackEvent>;
      protected:
        /// Update the cached values.
        void onNewEvent(const TrackEvent& event);

        /// Ensure the cached values are up-to-date.
        void ensureCache() const;

      protected:
        /// The track's events are stored in a BlockStream.
        babelwires::BlockStream m_blockStream;

        /// The length of the track.
        /// May be longer than the events duration, but may not be shorter.
        ModelDuration m_duration;

        mutable struct CachedValues {
            CachedValues(ModelDuration trackDuration);
            void addEvent(const TrackEvent& event);
            /// The total duration of the events in the track.
            ModelDuration m_totalEventDuration = 0;
            std::size_t m_hash = 0;

            /// A summary of information about the track.
            std::unordered_map<const char*, int> m_numEventGroupsByCategory;
        } m_cachedValues = CachedValues(0);

        /// Are the values in the cache up-to-date, or do they need to be recalculated.
        mutable bool m_cacheIsValid = true;
    };
} // namespace seqwires

#pragma once

#include "SeqwiresLib/Tracks/noteEvents.hpp"
#include "SeqwiresLib/Utilities/filteredTrackIterator.hpp"

namespace seqwires {
    /// This returns note events which never overlap.
    /// Any note started when the active note is playing is ignored.
    struct MonophonicNoteIterator : FilteredTrackIterator<NoteEvent> {
        enum InteriorEventFilter { OnOffOnly, AllEvents };

        /// If onOffOnly is true, then only NoteOnEvent and NoteOffEvent events will be returned.
        MonophonicNoteIterator(const Track& track, InteriorEventFilter interiorEventFilter);

        bool isEventOfInterest(const TrackEvent& event) override;

        bool m_noteIsActive = false;
        Pitch m_activePitch = 0;
        InteriorEventFilter m_interiorEventFilter = OnOffOnly;
    };

    /// Return a span of iterators which only return non-overlapping notes.
    babelwires::Span<MonophonicNoteIterator>
    iterateOverMonotonicNotes(const Track& track, MonophonicNoteIterator::InteriorEventFilter interiorEventFilter);
} // namespace seqwires

/**
 * A MonophonicNoteIterator iterates over a track, returning note events which never overlap.
 * 
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <SeqWiresLib/Utilities/monophonicNoteIterator.hpp>

bool seqwires::MonophonicNoteIterator::isEventOfInterest(const TrackEvent& event) {
    if (const NoteOnEvent* noteOn = event.as<NoteOnEvent>()) {
        if (!m_noteIsActive) {
            m_activePitch = noteOn->m_pitch;
            m_noteIsActive = true;
            return true;
        }
    } else if (const NoteOffEvent* noteOff = event.as<NoteOffEvent>()) {
        if (m_noteIsActive && (noteOff->m_pitch == m_activePitch)) {
            m_noteIsActive = false;
            return true;
        }
    } else if (const NoteEvent* note = event.as<NoteEvent>()) {
        if ((m_interiorEventFilter == AllEvents) && m_noteIsActive && (note->m_pitch == m_activePitch)) {
            return true;
        }
    }
    return false;
}

seqwires::MonophonicNoteIterator::MonophonicNoteIterator(const Track& track, InteriorEventFilter interiorEventFilter)
    : FilteredTrackIterator<NoteEvent>(track)
    , m_interiorEventFilter(interiorEventFilter) {}

babelwires::Span<seqwires::MonophonicNoteIterator>
seqwires::iterateOverMonotonicNotes(const Track& track,
                                    MonophonicNoteIterator::InteriorEventFilter interiorEventFilter) {
    MonophonicNoteIterator begin(track, interiorEventFilter);
    begin.initBegin();
    MonophonicNoteIterator end(track, interiorEventFilter);
    end.initEnd();
    return {begin, end};
}

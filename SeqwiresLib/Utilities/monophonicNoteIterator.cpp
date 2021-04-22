#include "SeqwiresLib/Utilities/monophonicNoteIterator.hpp"

bool seqwires::MonophonicNoteIterator::isEventOfInterest(const TrackEvent& event) {
    if (const NoteOnEvent* noteOn = event.asA<NoteOnEvent>()) {
        if (!m_noteIsActive) {
            m_activePitch = noteOn->m_pitch;
            m_noteIsActive = true;
            return true;
        }
    } else if (const NoteOffEvent* noteOff = event.asA<NoteOffEvent>()) {
        if (m_noteIsActive && (noteOff->m_pitch == m_activePitch)) {
            m_noteIsActive = false;
            return true;
        }
    } else if (const NoteEvent* note = event.asA<NoteEvent>()) {
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

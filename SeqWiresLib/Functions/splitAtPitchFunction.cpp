#include <SeqWiresLib/Functions/splitAtPitchFunction.hpp>

#include <SeqWiresLib/Types/Track/TrackEvents/noteEvents.hpp>
#include <SeqWiresLib/Utilities/filteredTrackIterator.hpp>

namespace {
    struct NotesAbove : seqwires::FilteredTrackIterator<> {
        NotesAbove(const seqwires::Track& track, seqwires::Pitch pitch) : seqwires::FilteredTrackIterator<>(track), m_pitch(pitch) {}

        virtual bool isEventOfInterest(const seqwires::TrackEvent& event) override {
            if (const seqwires::NoteEvent* noteEvent = event.as<seqwires::NoteEvent>()) {
                return noteEvent->m_pitch >= m_pitch;
            }
            return false;
        }

        seqwires::Pitch m_pitch;
    };

    struct NotesBelow : seqwires::FilteredTrackIterator<> {
        NotesBelow(const seqwires::Track& track, seqwires::Pitch pitch) : seqwires::FilteredTrackIterator<>(track), m_pitch(pitch) {}

        virtual bool isEventOfInterest(const seqwires::TrackEvent& event) override {
            if (const seqwires::NoteEvent* noteEvent = event.as<seqwires::NoteEvent>()) {
                return noteEvent->m_pitch < m_pitch;
            }
            return false;
        }

        seqwires::Pitch m_pitch;
    };

    struct NotNotes : seqwires::FilteredTrackIterator<> {
        NotNotes(const seqwires::Track& track) : seqwires::FilteredTrackIterator<>(track) {}

        virtual bool isEventOfInterest(const seqwires::TrackEvent& event) override {
            return event.as<seqwires::NoteEvent>() == nullptr;
        }

        seqwires::Pitch m_pitch;
    };
}

seqwires::SplitAtPitchResult seqwires::splitAtPitch(Pitch pitch, const Track& sourceTrack) {
    SplitAtPitchResult result;
    // TODO Do this in one traversal.
    {
        NotesAbove aboveIt(sourceTrack, pitch);
        NotesAbove aboveItEnd(sourceTrack, pitch);
        aboveIt.initBegin();
        aboveItEnd.initEnd();
        for (; aboveIt != aboveItEnd; ++aboveIt) {
            result.m_equalOrAbove.addEvent(*aboveIt);
        }
    }
    {
        NotesBelow belowIt(sourceTrack, pitch);
        NotesBelow belowItEnd(sourceTrack, pitch);
        belowIt.initBegin();
        belowItEnd.initEnd();
        for (; belowIt != belowItEnd; ++belowIt) {
            result.m_below.addEvent(*belowIt);
        }
    }
    {
        NotNotes otherIt(sourceTrack);
        NotNotes otherItEnd(sourceTrack);
        otherIt.initBegin();
        otherItEnd.initEnd();
        for (; otherIt != otherItEnd; ++otherIt) {
            result.m_other.addEvent(*otherIt);
        }
    }

    result.m_equalOrAbove.setDuration(sourceTrack.getDuration());
    result.m_below.setDuration(sourceTrack.getDuration());
    result.m_other.setDuration(sourceTrack.getDuration());

    return result;
}

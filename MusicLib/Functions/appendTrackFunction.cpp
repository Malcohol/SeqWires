/**
 * Function which join tracks together sequencially.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <MusicLib/Functions/appendTrackFunction.hpp>

#include <BabelWiresLib/ValueTree/modelExceptions.hpp>

void bw_music::appendTrack(Track& targetTrack, const Track& sourceTrack) {
    assert((&targetTrack != &sourceTrack) && "You cannot have source and target track being the same");

    const ModelDuration initialDuration = targetTrack.getDuration();
    const ModelDuration gapAtEnd = targetTrack.getDuration() - targetTrack.getTotalEventDuration();

    auto it = sourceTrack.begin();
    if (it != sourceTrack.end()) {
        TrackEventHolder firstEventInSequence = *it;
        firstEventInSequence->setTimeSinceLastEvent(firstEventInSequence->getTimeSinceLastEvent() + gapAtEnd);
        targetTrack.addEvent(firstEventInSequence.release());

        for (++it; it != sourceTrack.end(); ++it) {
            targetTrack.addEvent(*it);
        }
    }
    
    targetTrack.setDuration(initialDuration + sourceTrack.getDuration());
}

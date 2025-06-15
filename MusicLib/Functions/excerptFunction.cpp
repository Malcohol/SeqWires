/**
 * A function which extracts a section of sequence data from a track.
 * 
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <MusicLib/Functions/excerptFunction.hpp>

#include <MusicLib/Utilities/validTrackBuilder.hpp>

#include <set>

bw_music::Track bw_music::getTrackExcerpt(const Track& trackIn, ModelDuration start,
                                                           ModelDuration duration) {
    ValidTrackBuilder trackOut;

    ModelDuration end = start + duration;
    ModelDuration timeProcessed;
    ModelDuration timeOfCurrentEvent;

    auto it = trackIn.begin();

    while ((it != trackIn.end()) && ((timeProcessed + it->getTimeSinceLastEvent()) < start)) {
        timeProcessed += it->getTimeSinceLastEvent();
        ++it;
    }

    bool isFirstEvent = true;
    while ((it != trackIn.end()) && ((timeProcessed + it->getTimeSinceLastEvent()) <= end)) {
        TrackEventHolder newEvent = *it;
        if (isFirstEvent) {
            newEvent->setTimeSinceLastEvent(newEvent->getTimeSinceLastEvent() + timeProcessed - start);
            isFirstEvent = false;
        }
        trackOut.addEvent(newEvent.release());
        timeProcessed += it->getTimeSinceLastEvent();
        ++it;
    }

    trackOut.setDuration(duration);
    return trackOut.finishAndGetTrack();
}

/**
 * Function which merges tracks together.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <SeqWiresLib/Functions/mergeFunction.hpp>

#include <SeqWiresLib/Types/Track/TrackEvents/trackEventHolder.hpp>
#include <SeqWiresLib/Utilities/trackTraverser.hpp>

#include <BabelWiresLib/ValueTree/modelExceptions.hpp>

seqwires::Track seqwires::mergeTracks(const std::vector<const Track*>& sourceTracks) {
    Track trackOut;

    seqwires::ModelDuration trackDuration = 0;
    std::vector<TrackTraverser<Track::const_iterator>> traversers;

    const int numTracks = sourceTracks.size();
    traversers.reserve(numTracks);

    for (int i = 0; i < numTracks; ++i) {
        const seqwires::Track& track = *sourceTracks[i];
        traversers.emplace_back(track, track);
        traversers.back().leastUpperBoundDuration(trackDuration);
    }

    seqwires::ModelDuration timeSinceStart = 0;
    while (timeSinceStart < trackDuration) {
        seqwires::ModelDuration timeToNextEvent = trackDuration - timeSinceStart;
        for (int i = 0; i < numTracks; ++i) {
            traversers[i].greatestLowerBoundNextEvent(timeToNextEvent);
        }

        bool isFirstEvent = true;
        for (int i = 0; i < numTracks; ++i) {
            const seqwires::Track& track = *sourceTracks[i];
            traversers[i].advance(timeToNextEvent,
                                  [&isFirstEvent, &timeToNextEvent, &trackOut](const TrackEvent& event) {
                                      TrackEventHolder newEvent = event;
                                      newEvent->setTimeSinceLastEvent(isFirstEvent ? timeToNextEvent : 0);
                                      trackOut.addEvent(newEvent.release());
                                      isFirstEvent = false;
                                  });
        }

        timeSinceStart += timeToNextEvent;
    }

    trackOut.setDuration(trackDuration);

    return trackOut;
}

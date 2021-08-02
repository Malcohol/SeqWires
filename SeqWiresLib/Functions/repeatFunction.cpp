/**
 * A function which repeats sequence data a number of times.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include "SeqWiresLib/Functions/repeatFunction.hpp"

#include "SeqWiresLib/Tracks/trackEventHolder.hpp"

#include "BabelWires/Features/modelExceptions.hpp"

#include <set>

std::unique_ptr<seqwires::Track> seqwires::repeatTrack(const Track& trackIn, int count) {
    if (count < 0) {
        throw babelwires::ModelException() << "You cannot have repeat a negative number of times";
    }

    auto trackOut = std::make_unique<Track>();

    ModelDuration gapAtEnd = 0;

    for (int i = 0; i < count; ++i) {
        auto it = trackIn.begin();
        if (it != trackIn.end()) {
            TrackEventHolder firstEventInSequence = *it;
            firstEventInSequence->setTimeSinceLastEvent(firstEventInSequence->getTimeSinceLastEvent() + gapAtEnd);
            trackOut->addEvent(firstEventInSequence.release());

            for (; it != trackIn.end(); ++it) {
                // TODO Shouldn't need this.
                TrackEventHolder event = *it;
                trackOut->addEvent(event.release());
            }
            gapAtEnd = trackIn.getDuration() - trackIn.getTotalEventDuration();
        }
    }

    trackOut->setDuration(trackIn.getDuration() * count);

    return trackOut;
}
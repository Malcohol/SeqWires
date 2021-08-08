/**
 * A function which creates a track with modified pitches.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include "SeqWiresLib/Functions/transposeFunction.hpp"

#include "SeqWiresLib/Tracks/trackEventHolder.hpp"

#include "BabelWires/Features/modelExceptions.hpp"

std::unique_ptr<seqwires::Track> seqwires::transposeTrack(const Track& trackIn, int pitchOffset) {
    auto trackOut = std::make_unique<seqwires::Track>();

    
    for (auto it = trackIn.begin(); it != trackIn.end(); ++it) {
        TrackEventHolder holder(*it);
        holder->transpose(pitchOffset);
        trackOut->addEvent(holder.release());
    }
    trackOut->setDuration(trackIn.getDuration());

    return trackOut;
}


/**
 * A function which repeats sequence data a number of times.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <SeqWiresLib/Functions/repeatFunction.hpp>

#include <SeqWiresLib/Functions/appendTrackFunction.hpp>
#include <SeqWiresLib/Types/Track/TrackEvents/trackEventHolder.hpp>

#include <BabelWiresLib/Features/modelExceptions.hpp>

seqwires::Track seqwires::repeatTrack(const Track& trackIn, int count) {
    if (count < 0) {
        throw babelwires::ModelException() << "You cannot have repeat a negative number of times";
    }

    Track trackOut;

    for (int i = 0; i < count; ++i) {
        appendTrack(trackOut, trackIn);
    }

    return trackOut;
}

/**
 * Quantize track events.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <MusicLib/Functions/quantizeFunction.hpp>

#include <MusicLib/Types/Track/TrackEvents/trackEventHolder.hpp>
#include <MusicLib/Functions/sanitizingFunctions.hpp>

namespace {
    seqwires::ModelDuration getIdealTime(seqwires::ModelDuration time, seqwires::ModelDuration beat) {
        auto [div, mod] = time.divmod(beat);
        if (mod >= beat / 2) {
            ++div;
        }
        return beat * div;
    }
} // namespace

seqwires::Track seqwires::quantize(const Track& trackIn, ModelDuration beat) {

    using Group = std::tuple<TrackEvent::GroupingInfo::Category, TrackEvent::GroupingInfo::GroupValue>;

    Track trackOut;

    ModelDuration currentTimeSinceLastEvent;

    // The absolute time of the current event in trackIn.
    ModelDuration trackInAbsoluteTime;
    ModelDuration trackOutAbsoluteTime;

    for (auto it = trackIn.begin(); it != trackIn.end(); ++it) {
        const ModelDuration timeSinceLastEvent = it->getTimeSinceLastEvent();

        ModelDuration newTimeSinceLastEvent;
        if (timeSinceLastEvent > 0) {
            trackInAbsoluteTime += timeSinceLastEvent;
            const ModelDuration idealTime = getIdealTime(trackInAbsoluteTime, beat);
            const ModelDuration newTimeSinceLastEvent = idealTime - trackOutAbsoluteTime;
            TrackEventHolder event = *it;
            event->setTimeSinceLastEvent(newTimeSinceLastEvent);
            trackOut.addEvent(event.release());
            trackOutAbsoluteTime += newTimeSinceLastEvent;
        } else {
            trackOut.addEvent(*it);
        }
    }
    const ModelDuration idealDuration = getIdealTime(trackIn.getDuration(), beat);
    trackOut.setDuration(idealDuration);
    return removeZeroDurationGroups(trackOut);
}

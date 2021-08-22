/**
 * A processor which repeats sequence data a number of times.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include "BabelWiresLib/Processors/commonProcessor.hpp"

namespace babelwires {
    class IntFeature;
} // namespace babelwires

namespace seqwires {
    class TrackFeature;

    /// A processor which limits a track to events between certain points.
    // TODO UNTESTED
    class SplitAtPitchProcessor : public babelwires::CommonProcessor {
      public:
        SplitAtPitchProcessor();

        virtual void process(babelwires::UserLogger& userLogger) override;

        struct Factory : public babelwires::CommonProcessorFactory<SplitAtPitchProcessor> {
            Factory();
        };
      private:
        babelwires::IntFeature* m_pitch;
        TrackFeature* m_trackIn;
        TrackFeature* m_equalOrAboveTrackOut;
        TrackFeature* m_belowTrackOut;
        TrackFeature* m_otherTrackOut;
    };

} // namespace seqwires

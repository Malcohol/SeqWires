/**
 * A processor which repeats sequence data a number of times.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <BabelWiresLib/Processors/commonProcessor.hpp>

namespace babelwires {
    class IntFeature;
    class ArrayFeature;
    class EnumFeature;
} // namespace babelwires


namespace seqwires {
    class TrackFeature;

    /// A processor which limits a track to events between certain points.
    class MonophonicSubtracksProcessor : public babelwires::CommonProcessor {
      public:
        MonophonicSubtracksProcessor(const babelwires::ProjectContext& projectContext);

        virtual void process(babelwires::UserLogger& userLogger) override;

        struct Factory : public babelwires::CommonProcessorFactory<MonophonicSubtracksProcessor> {
            Factory();
        };
      private:
        babelwires::IntFeature* m_numSubtracks;
        babelwires::EnumFeature* m_policy;
        TrackFeature* m_trackIn;
        babelwires::ArrayFeature* m_tracksOut;
        TrackFeature* m_otherTrackOut;
    };

} // namespace seqwires

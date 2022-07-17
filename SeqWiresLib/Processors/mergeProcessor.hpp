/**
 * A processor which extracts a section of sequence data from a track.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <BabelWiresLib/Processors/commonProcessor.hpp>

namespace babelwires {
    class ArrayFeature;
} // namespace babelwires

namespace seqwires {
    class TrackFeature;

    /// A processor which limits a track to events between certain points.
    class MergeProcessor : public babelwires::CommonProcessor {
      public:
        MergeProcessor(const babelwires::ProjectContext& projectContext);

        virtual void process(babelwires::UserLogger& userLogger) override;

        struct Factory : public babelwires::CommonProcessorFactory<MergeProcessor> {
            Factory();
        };

      private:
        babelwires::ArrayFeature* m_tracksIn;
        seqwires::TrackFeature* m_trackOut;
    };

} // namespace seqwires

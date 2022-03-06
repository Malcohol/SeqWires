/**
 * A processor which repeats sequence data a number of times.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include "SeqWiresLib/Features/trackFeature.hpp"

#include "BabelWiresLib/Processors/parallelProcessor.hpp"

namespace babelwires {
    class IntFeature;
    class ArrayFeature;
} // namespace babelwires

namespace seqwires {

    /// A processor which limits a track to events between certain points.
    class TransposeProcessor : public babelwires::ParallelProcessor<seqwires::TrackFeature, seqwires::TrackFeature> {
      public:
        TransposeProcessor(const babelwires::ProjectContext& projectContext);

        void processEntry(babelwires::UserLogger& userLogger, const seqwires::TrackFeature& input, seqwires::TrackFeature& output) const override;

        struct Factory : public babelwires::CommonProcessorFactory<TransposeProcessor> {
            Factory();
        };
      private:
        babelwires::IntFeature* m_pitchOffset;
    };

} // namespace seqwires

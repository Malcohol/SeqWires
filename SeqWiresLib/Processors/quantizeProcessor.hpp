/**
 * A processor which repeats quantizes a track to a given beat.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <SeqWiresLib/Types/Track/trackFeature.hpp>

#include <BabelWiresLib/Processors/parallelProcessor.hpp>

namespace babelwires {
    class RationalFeature;
    class ArrayFeature;
} // namespace babelwires

namespace seqwires {

    /// A processor which limits a track to events between certain points.
    class QuantizeProcessor : public babelwires::ParallelProcessor<seqwires::TrackFeature, seqwires::TrackFeature> {
      public:
        QuantizeProcessor(const babelwires::ProjectContext& projectContext);

        void processEntry(babelwires::UserLogger& userLogger, const seqwires::TrackFeature& input, seqwires::TrackFeature& output) const override;

        struct Factory : public babelwires::CommonProcessorFactory<QuantizeProcessor> {
            Factory();
        };
      private:
        babelwires::RationalFeature* m_beat;
    };

} // namespace seqwires

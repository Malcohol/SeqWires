/**
 * A processor which extracts a section of sequence data from a track. 
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
    class ExcerptProcessor : public babelwires::ParallelProcessor<seqwires::TrackFeature, seqwires::TrackFeature> {
      public:
        ExcerptProcessor(const babelwires::ProjectContext& projectContext);

        void processEntry(babelwires::UserLogger& userLogger, const seqwires::TrackFeature& input, seqwires::TrackFeature& output) const override;

        struct Factory : public babelwires::CommonProcessorFactory<ExcerptProcessor> {
            Factory();
        };

      private:
        babelwires::RationalFeature* m_start;
        babelwires::RationalFeature* m_duration;
    };

} // namespace seqwires

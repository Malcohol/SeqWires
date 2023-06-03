/**
 * Processor which maps percussion events.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <SeqWiresLib/Types/Track/trackFeature.hpp>

#include <BabelWiresLib/Processors/parallelProcessor.hpp>

namespace babelwires {
    class MapFeature;
}

namespace seqwires {

    class PercussionMapProcessor
        : public babelwires::ParallelProcessor<seqwires::TrackFeature, seqwires::TrackFeature> {
      public:
        PercussionMapProcessor(const babelwires::ProjectContext& context);

        void processEntry(babelwires::UserLogger& userLogger, const seqwires::TrackFeature& input,
                          seqwires::TrackFeature& output) const override;

        struct Factory : public babelwires::CommonProcessorFactory<PercussionMapProcessor> {
            Factory();
        };

      private:
        babelwires::SimpleValueFeature* m_percussionMapFeature;
    };
} // namespace seqwires

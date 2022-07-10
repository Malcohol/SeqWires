/**
 * Processor which applies a map to chord events.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include "SeqWiresLib/Features/trackFeature.hpp"

#include "BabelWiresLib/Processors/parallelProcessor.hpp"

namespace babelwires { class MapFeature; }

namespace seqwires {

    class ChordMapProcessor : public babelwires::ParallelProcessor<seqwires::TrackFeature, seqwires::TrackFeature> {
      public:
        ChordMapProcessor(const babelwires::ProjectContext& context);

        void processEntry(babelwires::UserLogger& userLogger, const seqwires::TrackFeature& input, seqwires::TrackFeature& output) const override;

        struct Factory : public babelwires::CommonProcessorFactory<ChordMapProcessor> {
            Factory();
        };

      private:
        babelwires::MapFeature* m_chordTypeMapFeature;
        babelwires::MapFeature* m_pitchClassMapFeature;
    };
} // namespace seqwires

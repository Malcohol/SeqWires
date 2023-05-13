/**
 * Processor which applies a map to chord events.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <SeqWiresLib/Types/Track/trackFeature.hpp>

#include <BabelWiresLib/Processors/parallelProcessor.hpp>

namespace babelwires {
    class MapFeature2;
} // namespace babelwires

namespace seqwires {

    class ChordMapProcessor : public babelwires::ParallelProcessor<seqwires::TrackFeature, seqwires::TrackFeature> {
      public:
        ChordMapProcessor(const babelwires::ProjectContext& context);

        void processEntry(babelwires::UserLogger& userLogger, const seqwires::TrackFeature& input,
                          seqwires::TrackFeature& output) const override;

        struct Factory : public babelwires::CommonProcessorFactory<ChordMapProcessor> {
            Factory();
        };

      private:
        babelwires::MapFeature2* m_chordTypeMapFeature;
        babelwires::MapFeature2* m_pitchClassMapFeature;
    };
} // namespace seqwires

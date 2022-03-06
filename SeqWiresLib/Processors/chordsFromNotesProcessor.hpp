/**
 * Processor which interprets note events as defining chords.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include "BabelWiresLib/Processors/commonProcessor.hpp"

namespace seqwires {
    class TrackFeature;

    class ChordsFromNotesProcessor : public babelwires::CommonProcessor {
      public:
        ChordsFromNotesProcessor(const babelwires::ProjectContext& projectContext);

        virtual void process(babelwires::UserLogger& userLogger) override;

        struct Factory : public babelwires::CommonProcessorFactory<ChordsFromNotesProcessor> {
            Factory();
        };

      private:
        TrackFeature* m_trackIn;
        TrackFeature* m_trackOut;
    };
}

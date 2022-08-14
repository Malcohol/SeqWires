/**
 * Processor which interprets note events as defining chords.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <BabelWiresLib/Processors/commonProcessor.hpp>

namespace babelwires {
    class EnumFeature;
} // namespace babelwires

namespace seqwires {
    class TrackFeature;

    class FingeredChordsProcessor : public babelwires::CommonProcessor {
      public:
        FingeredChordsProcessor(const babelwires::ProjectContext& projectContext);

        virtual void process(babelwires::UserLogger& userLogger) override;

        struct Factory : public babelwires::CommonProcessorFactory<FingeredChordsProcessor> {
            Factory();
        };

      private:
        babelwires::EnumFeature* m_sustainPolicy;
        TrackFeature* m_trackIn;
        TrackFeature* m_trackOut;
    };
} // namespace seqwires

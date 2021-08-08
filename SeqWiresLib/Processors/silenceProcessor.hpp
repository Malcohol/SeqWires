/**
 * A processor which creates a silent track of a certain duration.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include "BabelWires/Processors/processor.hpp"
#include "BabelWires/Processors/processorFactory.hpp"

namespace babelwires {
    class RationalFeature;
    class ArrayFeature;
} // namespace babelwires

namespace seqwires {
    class TrackFeature;

    /// A processor which creates a silent track of a certain duration.
    // Note: The excerpt processor can do this too, but someone might not think of that.
    class SilenceProcessor : public babelwires::CommonProcessor {
      public:
        SilenceProcessor();

        virtual void process(babelwires::UserLogger& userLogger) override;

      private:
        babelwires::RationalFeature* m_duration;
        seqwires::TrackFeature* m_trackOut;
    };

    /// Create an excerpt processor.
    class SilenceProcessorFactory : public babelwires::ProcessorFactory {
      public:
        SilenceProcessorFactory();
        virtual std::unique_ptr<babelwires::Processor> createNewProcessor() const override;
    };

} // namespace seqwires

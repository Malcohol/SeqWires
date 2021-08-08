/**
 * A processor which repeats sequence data a number of times.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include "BabelWires/Processors/processor.hpp"
#include "BabelWires/Processors/processorFactory.hpp"

namespace babelwires {
    class IntFeature;
    class ArrayFeature;
} // namespace babelwires

namespace seqwires {

    /// A processor which limits a track to events between certain points.
    class RepeatProcessor : public babelwires::CommonProcessor {
      public:
        RepeatProcessor();

        virtual void process(babelwires::UserLogger& userLogger) override;

      private:
        babelwires::IntFeature* m_count;
        babelwires::ArrayFeature* m_tracksIn;
        babelwires::ArrayFeature* m_tracksOut;
    };

    /// Create an excerpt processor.
    class RepeatProcessorFactory : public babelwires::ProcessorFactory {
      public:
        RepeatProcessorFactory();
        virtual std::unique_ptr<babelwires::Processor> createNewProcessor() const override;
    };

} // namespace seqwires

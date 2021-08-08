/**
 * A processor which extracts a section of sequence data from a track. 
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

    /// A processor which limits a track to events between certain points.
    class ExcerptProcessor : public babelwires::CommonProcessor {
      public:
        ExcerptProcessor();

        virtual void process(babelwires::UserLogger& userLogger) override;

      private:
        babelwires::RationalFeature* m_start;
        babelwires::RationalFeature* m_duration;

        babelwires::ArrayFeature* m_tracksIn;
        babelwires::ArrayFeature* m_tracksOut;
    };

    /// Create an excerpt processor.
    class ExcerptProcessorFactory : public babelwires::ProcessorFactory {
      public:
        ExcerptProcessorFactory();
        virtual std::unique_ptr<babelwires::Processor> createNewProcessor() const override;
    };

} // namespace seqwires

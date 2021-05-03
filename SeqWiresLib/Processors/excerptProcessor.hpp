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
    class ExcerptProcessor : public babelwires::Processor {
      public:
        ExcerptProcessor();

        virtual void process(babelwires::UserLogger& userLogger) override;
        virtual babelwires::RecordFeature* getInputFeature() override;
        virtual babelwires::RecordFeature* getOutputFeature() override;

      private:
        std::unique_ptr<babelwires::RecordFeature> m_inputFeature;
        std::unique_ptr<babelwires::RecordFeature> m_outputFeature;

        babelwires::RationalFeature* m_start;
        babelwires::RationalFeature* m_duration;

        babelwires::ArrayFeature* m_tracksIn;
        babelwires::ArrayFeature* m_tracksOut;
        std::unordered_map<babelwires::ArrayIndex, babelwires::ArrayIndex> m_inToOutMap;
    };

    /// Create an excerpt processor.
    class ExcerptProcessorFactory : public babelwires::ProcessorFactory {
      public:
        ExcerptProcessorFactory();
        virtual std::unique_ptr<babelwires::Processor> createNewProcessor() const override;
    };

} // namespace seqwires

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
    class RepeatProcessor : public babelwires::Processor {
      public:
        RepeatProcessor();

        virtual void process(babelwires::UserLogger& userLogger) override;
        virtual babelwires::RecordFeature* getInputFeature() override;
        virtual babelwires::RecordFeature* getOutputFeature() override;

      private:
        std::unique_ptr<babelwires::RecordFeature> m_inputFeature;
        std::unique_ptr<babelwires::RecordFeature> m_outputFeature;

        babelwires::IntFeature* m_count;
        babelwires::ArrayFeature* m_tracksIn;
        babelwires::ArrayFeature* m_tracksOut;
        std::unordered_map<babelwires::ArrayIndex, babelwires::ArrayIndex> m_inToOutMap;
    };

    /// Create an excerpt processor.
    class RepeatProcessorFactory : public babelwires::ProcessorFactory {
      public:
        RepeatProcessorFactory();
        virtual std::unique_ptr<babelwires::Processor> createNewProcessor() const override;
    };

} // namespace seqwires

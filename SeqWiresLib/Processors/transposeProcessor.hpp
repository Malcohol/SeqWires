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
    class TransposeProcessor : public babelwires::Processor {
      public:
        TransposeProcessor();

        virtual void process(babelwires::UserLogger& userLogger) override;
        virtual babelwires::RecordFeature* getInputFeature() override;
        virtual babelwires::RecordFeature* getOutputFeature() override;

      private:
        std::unique_ptr<babelwires::RecordFeature> m_inputFeature;
        std::unique_ptr<babelwires::RecordFeature> m_outputFeature;

        babelwires::IntFeature* m_pitchOffset;
        babelwires::ArrayFeature* m_tracksIn;
        babelwires::ArrayFeature* m_tracksOut;
    };

    /// Create an excerpt processor.
    class TransposeProcessorFactory : public babelwires::ProcessorFactory {
      public:
        TransposeProcessorFactory();
        virtual std::unique_ptr<babelwires::Processor> createNewProcessor() const override;
    };

} // namespace seqwires

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
    class ArrayFeature;
} // namespace babelwires

namespace seqwires {
    class TrackFeature;

    /// A processor which limits a track to events between certain points.
    class ConcatenateProcessor : public babelwires::Processor {
      public:
        ConcatenateProcessor();

        virtual void process(babelwires::UserLogger& userLogger) override;
        virtual babelwires::RecordFeature* getInputFeature() override;
        virtual babelwires::RecordFeature* getOutputFeature() override;

      private:
        std::unique_ptr<babelwires::RecordFeature> m_inputFeature;
        std::unique_ptr<babelwires::RecordFeature> m_outputFeature;

        babelwires::ArrayFeature* m_tracksIn;
        seqwires::TrackFeature* m_trackOut;
        std::unordered_map<babelwires::ArrayIndex, babelwires::ArrayIndex> m_inToOutMap;
    };

    /// Create an excerpt processor.
    class ConcatenateProcessorFactory : public babelwires::ProcessorFactory {
      public:
        ConcatenateProcessorFactory();
        virtual std::unique_ptr<babelwires::Processor> createNewProcessor() const override;
    };

} // namespace seqwires

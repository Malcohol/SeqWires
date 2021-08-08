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
    class ConcatenateProcessor : public babelwires::CommonProcessor {
      public:
        ConcatenateProcessor();

        virtual void process(babelwires::UserLogger& userLogger) override;

      private:
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

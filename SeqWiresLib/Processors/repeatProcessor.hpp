/**
 * A processor which repeats sequence data a number of times.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include "BabelWires/Processors/commonProcessor.hpp"

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

        struct Factory : public babelwires::CommonProcessorFactory<RepeatProcessor> {
            Factory();
        };
      private:
        babelwires::IntFeature* m_count;
        babelwires::ArrayFeature* m_tracksIn;
        babelwires::ArrayFeature* m_tracksOut;
    };

} // namespace seqwires

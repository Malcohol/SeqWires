/**
 * A processor which quantizes quantizes a track to a given beat.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <SeqWiresLib/instance.hpp>

#include <BabelWiresLib/Processors/parallelProcessor.hpp>
#include <BabelWiresLib/Types/Rational/rationalType.hpp>


namespace seqwires {

    class QuantizeProcessorInput : public babelwires::ParallelValueProcessorInputBase {
      public:
        PRIMITIVE_TYPE("QuantTrcksIn", "Quantize In", "86a46b16-69a3-41bb-bbb3-19f8cb0a4e4d", 1);

        QuantizeProcessorInput();

        DECLARE_INSTANCE_BEGIN(QuantizeProcessorInput)
        DECLARE_INSTANCE_FIELD(Beat, babelwires::RationalType)
        // No need to mention the array here.
        DECLARE_INSTANCE_END()
    };

    class QuantizeProcessorOutput : public babelwires::ParallelValueProcessorOutputBase {
      public:
        PRIMITIVE_TYPE("QuantTrcksOut", "Quantize Out", "89feb462-63b1-473a-bc77-29540bda43f7", 1);

        QuantizeProcessorOutput();
    };

    /// A processor which quantizes the events in a track a specified number of times.
    class QuantizeProcessor : public babelwires::ParallelValueProcessor {
      public:
        BW_PROCESSOR_WITH_DEFAULT_FACTORY("QuantizeTracks", "Quantize", "1ae89077-2cfb-4071-910c-2f5dcfc85b17");

        QuantizeProcessor(const babelwires::ProjectContext& projectContext);

        static babelwires::ShortId getCommonArrayId();

        void processEntry(babelwires::UserLogger& userLogger, const babelwires::ValueFeature& inputFeature,
                          const babelwires::ValueFeature& input, babelwires::ValueFeature& output) const override;
    };

} // namespace seqwires

/**
 * A processor which repeats sequence data a number of times.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <SeqWiresLib/Functions/monophonicSubtracksFunction.hpp>
#include <SeqWiresLib/Types/Track/trackInstance.hpp>
#include <SeqWiresLib/Types/Track/trackType.hpp>

#include <BabelWiresLib/Instance/instance.hpp>
#include <BabelWiresLib/Processors/processorFactory.hpp>
#include <BabelWiresLib/Processors/processor.hpp>
#include <BabelWiresLib/TypeSystem/primitiveType.hpp>
#include <BabelWiresLib/Types/Record/recordType.hpp>

namespace seqwires {
    class MonophonicSubtracksProcessorInput : public babelwires::RecordType {
      public:
        PRIMITIVE_TYPE("MonoTracksIn", "Monophonic Tracks Input", "e8041fe6-29de-470e-9235-e1f05c5f791e", 1);

        MonophonicSubtracksProcessorInput();

        DECLARE_INSTANCE_BEGIN(MonophonicSubtracksProcessorInput)
        DECLARE_INSTANCE_FIELD(NumTrk, babelwires::IntType)
        DECLARE_INSTANCE_FIELD(Policy, MonophonicSubtracksPolicyEnum)
        DECLARE_INSTANCE_FIELD(Input, seqwires::TrackType)
        DECLARE_INSTANCE_END()
    };

    class MonophonicSubtracksProcessorOutput : public babelwires::RecordType {
      public:
        PRIMITIVE_TYPE("MonoTracksOut", "Monophonic Tracks Output", "c2228921-da8a-45c0-bebf-24951d651090", 1);

        MonophonicSubtracksProcessorOutput();

        DECLARE_INSTANCE_BEGIN(MonophonicSubtracksProcessorOutput)
        DECLARE_INSTANCE_ARRAY_FIELD(Sbtrks, seqwires::TrackType)
        DECLARE_INSTANCE_FIELD(Other, seqwires::TrackType)
        DECLARE_INSTANCE_END()
    };

    class MonophonicSubtracksProcessor : public babelwires::Processor {
      public:
        BW_PROCESSOR_WITH_DEFAULT_FACTORY("MonoSubtracksProcessor", "Monophonic subtracks", "7b6bbc49-24a5-4657-86fd-c457d77feaf9");

        MonophonicSubtracksProcessor(const babelwires::ProjectContext& projectContext);

      protected:
        void processValue(babelwires::UserLogger& userLogger, const babelwires::ValueTreeNode& inputFeature,
                          babelwires::ValueTreeNode& outputFeature) const override;
    };

} // namespace seqwires

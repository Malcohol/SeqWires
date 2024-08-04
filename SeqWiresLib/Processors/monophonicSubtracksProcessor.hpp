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
#include <SeqWiresLib/pitch.hpp>

#include <BabelWiresLib/Instance/instance.hpp>
#include <BabelWiresLib/Processors/processorFactory.hpp>
#include <BabelWiresLib/Processors/valueProcessor.hpp>
#include <BabelWiresLib/TypeSystem/primitiveType.hpp>
#include <BabelWiresLib/Types/Record/recordType.hpp>

#include <BabelWiresLib/Processors/commonProcessor.hpp>

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

    class MonophonicSubtracksProcessor : public babelwires::ValueProcessor {
      public:
        MonophonicSubtracksProcessor(const babelwires::ProjectContext& projectContext);

        struct Factory : public babelwires::CommonProcessorFactory<MonophonicSubtracksProcessor> {
            Factory();
        };

      protected:
        void processValue(babelwires::UserLogger& userLogger, const babelwires::ValueFeature& inputFeature,
                          babelwires::ValueFeature& outputFeature) const override;
    };

} // namespace seqwires

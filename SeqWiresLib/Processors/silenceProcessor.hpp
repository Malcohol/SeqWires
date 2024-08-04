/**
 * A processor which creates a silent track of a certain duration.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <BabelWiresLib/Processors/commonProcessor.hpp>

#include <SeqWiresLib/Types/Track/trackInstance.hpp>
#include <SeqWiresLib/Types/Track/trackType.hpp>

#include <BabelWiresLib/Instance/instance.hpp>
#include <BabelWiresLib/Processors/processorFactory.hpp>
#include <BabelWiresLib/Processors/valueProcessor.hpp>
#include <BabelWiresLib/TypeSystem/primitiveType.hpp>
#include <BabelWiresLib/Types/Record/recordType.hpp>


namespace babelwires {
    class RationalFeature;
    class ArrayFeature;
    class ValueFeature;
} // namespace babelwires

namespace seqwires {
    class TrackFeature;

    class SilenceProcessorInput : public babelwires::RecordType {
      public:
        PRIMITIVE_TYPE("SilentTrackIn", "Silence In", "b3d5c37c-aa07-47ac-9ebb-3cf81731b97b", 1);

        SilenceProcessorInput();

        DECLARE_INSTANCE_BEGIN(SilenceProcessorInput)
        DECLARE_INSTANCE_FIELD(Durn, babelwires::RationalType)
        DECLARE_INSTANCE_END()
    };

    class SilenceProcessorOutput : public babelwires::RecordType {
      public:
        PRIMITIVE_TYPE("SilentTrackOut", "Silence Out", "fd8b4645-1435-448c-bf9d-0cebf8999a81", 1);

        SilenceProcessorOutput();

        DECLARE_INSTANCE_BEGIN(SilenceProcessorOutput)
        DECLARE_INSTANCE_FIELD(Track, TrackType)
        DECLARE_INSTANCE_END()
    };

    class SilenceProcessor : public babelwires::ValueProcessor {
      public:
        BW_PROCESSOR_WITH_DEFAULT_FACTORY("SilentTrack", "Silence", "c59e4643-b7d8-430b-980b-bd81b5aa007b");

        SilenceProcessor(const babelwires::ProjectContext& projectContext);

      protected:
        void processValue(babelwires::UserLogger& userLogger, const babelwires::ValueFeature& inputFeature,
                          babelwires::ValueFeature& outputFeature) const override;
    };

} // namespace seqwires

/**
 * A processor which extracts a section of sequence data from a track.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <SeqWiresLib/Types/Track/trackFeature.hpp>

#include <BabelWiresLib/Instance/instance.hpp>
#include <BabelWiresLib/Processors/parallelProcessor.hpp>
#include <BabelWiresLib/Processors/valueProcessor.hpp>
#include <BabelWiresLib/Types/Record/recordType.hpp>

namespace babelwires {
    class RationalFeature;
    class ArrayFeature;

    class ParallelValueProcessorInputBase : public RecordType {
      public:
        ParallelValueProcessorInputBase(std::vector<RecordType::Field> commonInput, ShortId arrayId, TypeRef entryType);
    };

    class ParallelValueProcessorOutputBase : public RecordType {
      public:
        ParallelValueProcessorOutputBase(ShortId arrayId, TypeRef entryType);
    };

    class ParallelValueProcessor : public ValueProcessor {
      public:
        ParallelValueProcessor(const ProjectContext& projectContext, const TypeRef& parallelInput,
                               const TypeRef& parallelOutput);

      protected:
        void processValue(UserLogger& userLogger, const ValueFeature& inputFeature,
                          ValueFeature& outputFeature) const override final;

        virtual void processEntry(UserLogger& userLogger, const ValueFeature& inputFeature,
                                  const ValueFeature& inputEntry, ValueFeature& outputEntry) const = 0;
    };

} // namespace babelwires

namespace seqwires {

    class ExcerptProcessorInput : public babelwires::ParallelValueProcessorInputBase {
      public:
        PRIMITIVE_TYPE("ExcerptTrckIn", "Excerpt In", "68577705-b5c2-499c-a2db-8ab5f63e5728", 1);

        ExcerptProcessorInput();

        DECLARE_INSTANCE_BEGIN(ExcerptProcessorInput)
        DECLARE_INSTANCE_FIELD(Start, babelwires::RationalType)
        DECLARE_INSTANCE_FIELD(Duratn, babelwires::RationalType)
        // No need to mention the array here.
        DECLARE_INSTANCE_END()
    };

    class ExcerptProcessorOutput : public babelwires::ParallelValueProcessorOutputBase {
      public:
        PRIMITIVE_TYPE("ExcerptTrckOut", "Excerpt Out", "73469491-111c-441a-b89c-2f8aceaa640c", 1);

        ExcerptProcessorOutput();
    };

    /// A processor which limits a track to events between certain points.
    class ExcerptProcessor : public babelwires::ParallelValueProcessor {
      public:
        BW_PROCESSOR_WITH_DEFAULT_FACTORY("TrackExcerpt", "Excerpt", "83c74dba-7861-447c-9abb-0b4439061baf");

        ExcerptProcessor(const babelwires::ProjectContext& projectContext);

        static babelwires::ShortId getCommonArrayId();

        void processEntry(babelwires::UserLogger& userLogger, const babelwires::ValueFeature& inputFeature,
                          const babelwires::ValueFeature& input, babelwires::ValueFeature& output) const override;
    };

} // namespace seqwires

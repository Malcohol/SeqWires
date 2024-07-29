/**
 * A processor which repeats sequence data a number of times.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <SeqWiresLib/pitch.hpp>
#include <SeqWiresLib/Types/Track/trackType.hpp>
#include <SeqWiresLib/Types/Track/trackInstance.hpp>

#include <BabelWiresLib/Processors/commonProcessor.hpp>
#include <BabelWiresLib/TypeSystem/primitiveType.hpp>
#include <BabelWiresLib/Types/Record/recordType.hpp>
#include <BabelWiresLib/Instance/instance.hpp>
#include <BabelWiresLib/Processors/valueProcessor.hpp>

namespace babelwires {
    class EnumFeature;
} // namespace babelwires

namespace seqwires {
    class TrackFeature;

    /// A processor which limits a track to events between certain pitches.
    class SplitAtPitchProcessor : public babelwires::CommonProcessor {
      public:
        SplitAtPitchProcessor(const babelwires::ProjectContext& projectContext);

        virtual void process(babelwires::UserLogger& userLogger) override;

        struct Factory : public babelwires::CommonProcessorFactory<SplitAtPitchProcessor> {
            Factory();
        };
      private:
        babelwires::EnumFeature* m_pitch;
        TrackFeature* m_trackIn;
        TrackFeature* m_equalOrAboveTrackOut;
        TrackFeature* m_belowTrackOut;
        TrackFeature* m_otherTrackOut;
    };

    class SplitAtPitchProcessorInput : babelwires::RecordType {
      public:
        PRIMITIVE_TYPE("PitchSplitIn", "Split At Pitch Input", "f901af3a-c27b-449c-961a-8f43dee7d9a6", 1);

        SplitAtPitchProcessorInput();

        DECLARE_INSTANCE_BEGIN(SplitAtPitchProcessorInput)
        DECLARE_INSTANCE_FIELD(Pitch, seqwires::PitchEnum)
        DECLARE_INSTANCE_FIELD(Input, seqwires::TrackType)
        DECLARE_INSTANCE_END()        
    };

    class SplitAtPitchProcessorOutput : babelwires::RecordType {
      public:
        PRIMITIVE_TYPE("PitchSplitOut", "Split At Pitch Output", "50f790e1-0ef3-4f34-ad14-5a6762772e19", 1);

        SplitAtPitchProcessorOutput();

        DECLARE_INSTANCE_BEGIN(SplitAtPitchProcessorInput)
        DECLARE_INSTANCE_FIELD(Above, seqwires::TrackType)
        DECLARE_INSTANCE_FIELD(Below, seqwires::TrackType)
        DECLARE_INSTANCE_FIELD(Other, seqwires::TrackType)
        DECLARE_INSTANCE_END()
    };

    class SplitAtPitchProcessor2 : public babelwires::ValueProcessor<SplitAtPitchProcessorInput, SplitAtPitchProcessorOutput> {
      public:
        SplitAtPitchProcessor2(const babelwires::ProjectContext& projectContext);

      protected:
        void processValue(babelwires::UserLogger& userLogger, const babelwires::Instance<const babelwires::ValueFeature, SplitAtPitchProcessorInput>& input,
                                  babelwires::Instance<babelwires::ValueFeature, SplitAtPitchProcessorOutput>& output) const override;
    };

} // namespace seqwires

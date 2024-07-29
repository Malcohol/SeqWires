/**
 * A processor which repeats sequence data a number of times.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <SeqWiresLib/Processors/splitAtPitchProcessor.hpp>

#include <SeqWiresLib/Functions/splitAtPitchFunction.hpp>
#include <SeqWiresLib/Types/Track/trackFeature.hpp>

#include <BabelWiresLib/Features/arrayFeature.hpp>
#include <BabelWiresLib/Features/modelExceptions.hpp>
#include <BabelWiresLib/Features/rootFeature.hpp>
#include <BabelWiresLib/Types/Enum/enumFeature.hpp>

#include <Common/Identifiers/registeredIdentifier.hpp>

#include <set>

seqwires::SplitAtPitchProcessor::SplitAtPitchProcessor(const babelwires::ProjectContext& projectContext)
    : CommonProcessor(projectContext) {
    m_pitch =
        m_inputFeature->addField(std::make_unique<babelwires::EnumFeature>(seqwires::PitchEnum::getThisIdentifier()),
                                 BW_SHORT_ID("Pitch", "Pitch", "6b721baa-084f-450b-bf35-2e08a9592958"));
    m_trackIn = m_inputFeature->addField(std::make_unique<TrackFeature>(),
                                         BW_SHORT_ID("Input", "Input Track", "9314a43f-256a-4915-b218-f2ba37133863"));
    m_equalOrAboveTrackOut =
        m_outputFeature->addField(std::make_unique<TrackFeature>(),
                                  BW_SHORT_ID("Above", "Notes at/above", "4c65b7f1-e546-4df2-9891-23623b74bc23"));
    m_belowTrackOut = m_outputFeature->addField(
        std::make_unique<TrackFeature>(), BW_SHORT_ID("Below", "Notes below", "ab6a1d57-8c77-4df9-baf7-4b24136d9279"));
    m_otherTrackOut = m_outputFeature->addField(std::make_unique<TrackFeature>(),
                                                BW_SHORT_ID("Other", "Other", "83bf663c-2931-467f-8403-b12e18138c68"));
}

seqwires::SplitAtPitchProcessor::Factory::Factory()
    : CommonProcessorFactory(
          BW_LONG_ID("SplitAtPitchProcessor", "Split At Pitch", "9f5076f5-42a1-4655-a71e-b6b06a4e92cc"), 1) {}

void seqwires::SplitAtPitchProcessor::process(babelwires::UserLogger& userLogger) {
    if (m_pitch->isChanged(babelwires::Feature::Changes::SomethingChanged) ||
        m_trackIn->isChanged(babelwires::Feature::Changes::SomethingChanged)) {
        const int pitch = m_pitch->tryGetEnumIndex();
        if (pitch >= 0) {
            auto newTracksOut = splitAtPitch(Pitch(pitch), m_trackIn->get());
            m_equalOrAboveTrackOut->set(std::move(newTracksOut.m_equalOrAbove));
            m_belowTrackOut->set(std::move(newTracksOut.m_below));
            m_otherTrackOut->set(std::move(newTracksOut.m_other));
        } else {
            throw babelwires::ModelException() << "A valid pitch value was not specified in the SplitAtPitchProcessor";
        }
    }
}

seqwires::SplitAtPitchProcessorInput::SplitAtPitchProcessorInput()
    : babelwires::RecordType({{BW_SHORT_ID("Pitch", "Pitch", "6b721baa-084f-450b-bf35-2e08a9592958"),
                               seqwires::PitchEnum::getThisIdentifier()},
                              {BW_SHORT_ID("Input", "Input Track", "9314a43f-256a-4915-b218-f2ba37133863"),
                               DefaultTrackType::getThisIdentifier()}}) {}

seqwires::SplitAtPitchProcessorOutput::SplitAtPitchProcessorOutput()
    : babelwires::RecordType({
          {BW_SHORT_ID("Above", "Notes at/above", "4c65b7f1-e546-4df2-9891-23623b74bc23"),
           DefaultTrackType::getThisIdentifier()},
          {BW_SHORT_ID("Below", "Notes below", "ab6a1d57-8c77-4df9-baf7-4b24136d9279"),
           DefaultTrackType::getThisIdentifier()},
          {BW_SHORT_ID("Other", "Other", "83bf663c-2931-467f-8403-b12e18138c68"),
           DefaultTrackType::getThisIdentifier()},
      }) {}

seqwires::SplitAtPitchProcessor2::SplitAtPitchProcessor2(const babelwires::ProjectContext& projectContext)
    : ValueProcessor(projectContext, SplitAtPitchProcessorInput::getThisIdentifier(),
                     SplitAtPitchProcessorOutput::getThisIdentifier()) {}

void seqwires::SplitAtPitchProcessor2::processValue(babelwires::UserLogger& userLogger, const babelwires::ValueFeature& inputFeature, babelwires::ValueFeature& outputFeature) const {
    SplitAtPitchProcessorInput::Instance<const babelwires::ValueFeature> input{inputFeature};
    auto pitch = input.getPitch();
    auto trackIn = input.getInput();
    if (pitch->isChanged(babelwires::Feature::Changes::SomethingChanged) ||
        trackIn->isChanged(babelwires::Feature::Changes::SomethingChanged)) {
        const int pitchIndex = pitch.getInstanceType().tryGetIndexFromIdentifier(pitch.get().get());
        if (pitchIndex >= 0) {
            auto newTracksOut = splitAtPitch(Pitch(pitchIndex), trackIn.get());
            auto outputType = outputFeature.getTypeRef().toString();
            SplitAtPitchProcessorOutput::Instance<babelwires::ValueFeature> output{outputFeature};
            output.getAbove().set(std::move(newTracksOut.m_equalOrAbove));
            output.getBelow().set(std::move(newTracksOut.m_below));
            output.getOther().set(std::move(newTracksOut.m_other));
        } else {
            throw babelwires::ModelException() << "A valid pitch value was not specified in the SplitAtPitchProcessor";
        }
    }
}

seqwires::SplitAtPitchProcessor2::Factory::Factory()
    : CommonProcessorFactory(
          BW_LONG_ID("SplitAtPitchProcessor2", "Split At Pitch 2", "c7b79e31-98f4-4d20-b946-f55113eb3b75"), 1) {}

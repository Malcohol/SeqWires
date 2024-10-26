/**
 * A processor which repeats sequence data a number of times.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <SeqWiresLib/Processors/splitAtPitchProcessor.hpp>

#include <SeqWiresLib/Functions/splitAtPitchFunction.hpp>

#include <BabelWiresLib/Features/modelExceptions.hpp>

#include <Common/Identifiers/registeredIdentifier.hpp>

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

seqwires::SplitAtPitchProcessor::SplitAtPitchProcessor(const babelwires::ProjectContext& projectContext)
    : Processor(projectContext, SplitAtPitchProcessorInput::getThisIdentifier(),
                     SplitAtPitchProcessorOutput::getThisIdentifier()) {}

void seqwires::SplitAtPitchProcessor::processValue(babelwires::UserLogger& userLogger, const babelwires::ValueFeature& inputFeature, babelwires::ValueFeature& outputFeature) const {
    SplitAtPitchProcessorInput::ConstInstance input{inputFeature};
    auto pitch = input.getPitch();
    auto trackIn = input.getInput();
    if (pitch->isChanged(babelwires::Feature::Changes::SomethingChanged) ||
        trackIn->isChanged(babelwires::Feature::Changes::SomethingChanged)) {
        const int pitchIndex = pitch.getInstanceType().tryGetIndexFromIdentifier(pitch.get().get());
        if (pitchIndex >= 0) {
            auto newTracksOut = splitAtPitch(Pitch(pitchIndex), trackIn.get());
            auto outputType = outputFeature.getTypeRef().toString();
            SplitAtPitchProcessorOutput::Instance output{outputFeature};
            output.getAbove().set(std::move(newTracksOut.m_equalOrAbove));
            output.getBelow().set(std::move(newTracksOut.m_below));
            output.getOther().set(std::move(newTracksOut.m_other));
        } else {
            throw babelwires::ModelException() << "A valid pitch value was not specified in the SplitAtPitchProcessor";
        }
    }
}

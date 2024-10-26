/**
 * A processor which creates a silent track of a certain duration.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <SeqWiresLib/Processors/silenceProcessor.hpp>

#include <SeqWiresLib/Types/duration.hpp>

#include <BabelWiresLib/Types/Rational/rationalValue.hpp>

seqwires::SilenceProcessorInput::SilenceProcessorInput()
    : babelwires::RecordType(
          {{BW_SHORT_ID("Durn", "Duration", "05d3ea91-cb90-42f5-9988-2fb2e02e231c"), Duration::getThisIdentifier()}}) {}

seqwires::SilenceProcessorOutput::SilenceProcessorOutput()
    : babelwires::RecordType({{BW_SHORT_ID("Track", "Track", "86f3d028-a616-4a95-a566-a010ffcabb19"),
                               DefaultTrackType::getThisIdentifier()}}) {}

seqwires::SilenceProcessor::SilenceProcessor(const babelwires::ProjectContext& projectContext)
    : Processor(projectContext, SilenceProcessorInput::getThisIdentifier(),
                     SilenceProcessorOutput::getThisIdentifier()) {}

void seqwires::SilenceProcessor::processValue(babelwires::UserLogger& userLogger,
                                              const babelwires::ValueTreeNode& inputFeature,
                                              babelwires::ValueTreeNode& outputFeature) const {
    SilenceProcessorInput::ConstInstance input{inputFeature};
    if (input->isChanged(babelwires::ValueTreeNode::Changes::SomethingChanged)) {
        SilenceProcessorOutput::Instance output{outputFeature};
        output.getTrack().set(Track(input.getDurn().get()));
    }
}
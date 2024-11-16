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
          {{BW_SHORT_ID("Durn", "Duration", "05d3ea91-cb90-42f5-9988-2fb2e02e231c"), Duration::getThisType()}}) {}

seqwires::SilenceProcessorOutput::SilenceProcessorOutput()
    : babelwires::RecordType({{BW_SHORT_ID("Track", "Track", "86f3d028-a616-4a95-a566-a010ffcabb19"),
                               DefaultTrackType::getThisType()}}) {}

seqwires::SilenceProcessor::SilenceProcessor(const babelwires::ProjectContext& projectContext)
    : Processor(projectContext, SilenceProcessorInput::getThisType(),
                     SilenceProcessorOutput::getThisType()) {}

void seqwires::SilenceProcessor::processValue(babelwires::UserLogger& userLogger,
                                              const babelwires::ValueTreeNode& input,
                                              babelwires::ValueTreeNode& output) const {
    SilenceProcessorInput::ConstInstance in{input};
    if (in->isChanged(babelwires::ValueTreeNode::Changes::SomethingChanged)) {
        SilenceProcessorOutput::Instance out{output};
        out.getTrack().set(Track(in.getDurn().get()));
    }
}
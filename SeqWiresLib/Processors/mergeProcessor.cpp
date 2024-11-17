/**
 * A processor which extracts a section of sequence data from a track.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <SeqWiresLib/Processors/mergeProcessor.hpp>

#include <SeqWiresLib/Functions/mergeFunction.hpp>

#include <BabelWiresLib/Types/Array/arrayTypeConstructor.hpp>

#include <Common/Identifiers/registeredIdentifier.hpp>

#include <set>

seqwires::MergeProcessorInput::MergeProcessorInput()
    : babelwires::RecordType(
          {{BW_SHORT_ID("Input", "Input tracks", "80b175ae-c954-4943-96d8-eaffcd7ed6e1"),
            babelwires::ArrayTypeConstructor::makeTypeRef(DefaultTrackType::getThisType(), 2, 16)}}) {}

seqwires::MergeProcessorOutput::MergeProcessorOutput()
    : babelwires::RecordType({{BW_SHORT_ID("Output", "Output track", "ab56e996-d361-42ed-a0df-44a90a73dc20"),
                               DefaultTrackType::getThisType()}}) {}

seqwires::MergeProcessor::MergeProcessor(const babelwires::ProjectContext& projectContext)
    : Processor(projectContext, MergeProcessorInput::getThisType(), MergeProcessorOutput::getThisType()) {}

void seqwires::MergeProcessor::processValue(babelwires::UserLogger& userLogger, const babelwires::ValueTreeNode& input,
                                            babelwires::ValueTreeNode& output) const {
    MergeProcessorInput::ConstInstance in{input};
    if (in->isChanged(babelwires::ValueTreeNode::Changes::SomethingChanged)) {
        std::vector<const Track*> tracksIn;
        for (int i = 0; i < in.getInput().getSize(); ++i) {
            tracksIn.emplace_back(&in.getInput().getEntry(i).get());
        }
        MergeProcessorOutput::Instance out{output};
        out.getOutput().set(mergeTracks(tracksIn));
    }
}

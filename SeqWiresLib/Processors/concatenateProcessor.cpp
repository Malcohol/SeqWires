/**
 * A processor which extracts a section of sequence data from a track.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <SeqWiresLib/Processors/concatenateProcessor.hpp>

#include <SeqWiresLib/Functions/appendTrackFunction.hpp>

#include <BabelWiresLib/Types/Array/arrayTypeConstructor.hpp>
#include <BabelWiresLib/Types/Int/intTypeConstructor.hpp>

seqwires::ConcatenateProcessorInput::ConcatenateProcessorInput()
    : babelwires::RecordType(
          {{BW_SHORT_ID("Input", "Input tracks", "3b8d8cd7-21d9-44a1-877e-134915fe5aca"),
            babelwires::TypeRef{babelwires::ArrayTypeConstructor::getThisIdentifier(),
                                {{DefaultTrackType::getThisIdentifier()},
                                 {babelwires::IntValue(2), babelwires::IntValue(16), babelwires::IntValue(2)}}}}}) {}

seqwires::ConcatenateProcessorOutput::ConcatenateProcessorOutput()
    : babelwires::RecordType({{BW_SHORT_ID("Output", "Output track", "873d5d66-c5ec-46a4-9aba-f5f4223bdfd4"),
                               DefaultTrackType::getThisIdentifier()}}) {}

seqwires::ConcatenateProcessor::ConcatenateProcessor(const babelwires::ProjectContext& projectContext)
    : babelwires::Processor(projectContext, ConcatenateProcessorInput::getThisIdentifier(),
                                 ConcatenateProcessorOutput::getThisIdentifier()) {}

void seqwires::ConcatenateProcessor::processValue(babelwires::UserLogger& userLogger,
                                                           const babelwires::ValueTreeNode& input,
                                                           babelwires::ValueTreeNode& output) const {
    ConcatenateProcessorInput::ConstInstance in{input};
    if (in->isChanged(babelwires::ValueTreeNode::Changes::SomethingChanged)) {
        Track trackOut;

        for (int i = 0; i < in.getInput().getSize(); ++i) {
            appendTrack(trackOut, in.getInput().getEntry(i).get());
        }

        ConcatenateProcessorOutput::Instance out{output};

        out.getOutput().set(std::move(trackOut));
    }
}

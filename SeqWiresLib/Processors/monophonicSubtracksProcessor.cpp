/**
 * A processor which repeats sequence data a number of times.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <SeqWiresLib/Processors/monophonicSubtracksProcessor.hpp>

#include <BabelWiresLib/Types/Array/arrayTypeConstructor.hpp>
#include <BabelWiresLib/Types/Int/intTypeConstructor.hpp>

seqwires::MonophonicSubtracksProcessorInput::MonophonicSubtracksProcessorInput()
    : babelwires::RecordType(
          {{BW_SHORT_ID("NumTrk", "Num subtracks", "30bc74d2-b678-4986-8296-929db40fc8c2"),
            babelwires::TypeRef{babelwires::IntTypeConstructor::getThisIdentifier(),
                                {{}, {babelwires::IntValue(1), babelwires::IntValue(16), babelwires::IntValue(1)}}}},
           {BW_SHORT_ID("Policy", "Policy", "c3192ee7-adec-4239-83a1-ef2d130ce421"),
            MonophonicSubtracksPolicyEnum::getThisType()},
           {BW_SHORT_ID("Input", "Input Track", "16e6745d-2456-489f-b73b-8704a442591b"),
            DefaultTrackType::getThisType()}}) {}

seqwires::MonophonicSubtracksProcessorOutput::MonophonicSubtracksProcessorOutput()
    : babelwires::RecordType(
          {{BW_SHORT_ID("Sbtrks", "Mono tracks", "27c5fbe2-1060-4dc4-b46a-735b48128e17"),
            babelwires::TypeRef{babelwires::ArrayTypeConstructor::getThisIdentifier(),
                                {{DefaultTrackType::getThisType()},
                                 {babelwires::IntValue(0), babelwires::IntValue(16), babelwires::IntValue(1)}}}},
           {BW_SHORT_ID("Other", "Other", "bc3a5261-630c-43d7-bda5-f85dd6a1fe2b"),
            DefaultTrackType::getThisType()}}) {}

seqwires::MonophonicSubtracksProcessor::MonophonicSubtracksProcessor(const babelwires::ProjectContext& projectContext)
    : Processor(projectContext, MonophonicSubtracksProcessorInput::getThisType(),
                     MonophonicSubtracksProcessorOutput::getThisType()) {}

void seqwires::MonophonicSubtracksProcessor::processValue(babelwires::UserLogger& userLogger,
                                                           const babelwires::ValueTreeNode& input,
                                                           babelwires::ValueTreeNode& output) const {
    MonophonicSubtracksProcessorInput::ConstInstance in{input};
    if (in->isChanged(babelwires::ValueTreeNode::Changes::SomethingChanged)) {
        const unsigned int numTracks = in.getNumTrk().get();
        const MonophonicSubtracksPolicyEnum::Value policy = in.getPolicy().get();
        const seqwires::Track& trackIn = in.getInput().get();
        auto result = getMonophonicSubtracks(trackIn, numTracks, policy);
        
        MonophonicSubtracksProcessorOutput::Instance out{output};
        auto tracksOut = out.getSbtrks();
        tracksOut.setSize(numTracks);
        for (int i = 0; i < result.m_noteTracks.size(); ++i) {
            tracksOut.getEntry(i).set(std::move(result.m_noteTracks[i]));
        }
        out.getOther().set(std::move(result.m_other));
    }
}

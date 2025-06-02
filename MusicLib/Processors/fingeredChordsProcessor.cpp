/**
 * Processor which interprets note events as defining chords.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <MusicLib/Processors/fingeredChordsProcessor.hpp>

seqwires::FingeredChordsProcessorInput::FingeredChordsProcessorInput()
    : babelwires::RecordType({{BW_SHORT_ID("Policy", "Policy", "a1dd2ae0-e91e-40fe-af4a-c74f2c7d978a"),
                               seqwires::FingeredChordsSustainPolicyEnum::getThisType()},
                              {BW_SHORT_ID("Notes", "Notes", "f0ef98dd-6b1a-4a11-ac21-5492ec7ce038"),
                               DefaultTrackType::getThisType()}}) {}

seqwires::FingeredChordsProcessorOutput::FingeredChordsProcessorOutput()
    : babelwires::RecordType({{BW_SHORT_ID("Chords", "Chords", "25154c6f-fe60-4d0a-b991-7cb7aa43409a"),
                               DefaultTrackType::getThisType()}}) {}


seqwires::FingeredChordsProcessor::FingeredChordsProcessor(const babelwires::ProjectContext& projectContext)
    : Processor(projectContext, FingeredChordsProcessorInput::getThisType(),
                     FingeredChordsProcessorOutput::getThisType()) {}

void seqwires::FingeredChordsProcessor::processValue(babelwires::UserLogger& userLogger,
                                                      const babelwires::ValueTreeNode& input,
                                                      babelwires::ValueTreeNode& output) const {
    FingeredChordsProcessorInput::ConstInstance in{input};
    if (in->isChanged(babelwires::ValueTreeNode::Changes::SomethingChanged)) {
        FingeredChordsProcessorOutput::Instance out{output};
        out.getChords().set(fingeredChordsFunction(in.getNotes().get(), in.getPolicy().get()));
    }
}

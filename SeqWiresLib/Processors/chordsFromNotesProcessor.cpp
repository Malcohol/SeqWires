#include "SeqWiresLib/Processors/chordsFromNotesProcessor.hpp"
#include "SeqWiresLib/Features/trackFeature.hpp"
#include "SeqWiresLib/Functions/chordsFromNotesFunction.hpp"

seqwires::ChordsFromNotesProcessor::ChordsFromNotesProcessor() {
    babelwires::FieldIdentifier field = FIELD_NAME("Track", "Track", "f0ef98dd-6b1a-4a11-ac21-5492ec7ce038");
    m_trackIn = m_inputFeature->addField(std::make_unique<TrackFeature>(), field);
    m_trackOut = m_outputFeature->addField(std::make_unique<TrackFeature>(),field);
}

seqwires::ChordsFromNotesProcessor::Factory::Factory()
    : CommonProcessorFactory("ChordsFromNotes", "Chords from Notes", 1) {}

void seqwires::ChordsFromNotesProcessor::process(babelwires::UserLogger& userLogger) {
    if (m_trackIn->isChanged(babelwires::Feature::Changes::SomethingChanged)) {
        m_trackOut->set(std::make_unique<Track>(chordsFromNotesFunction(m_trackIn->get())));
    }
}


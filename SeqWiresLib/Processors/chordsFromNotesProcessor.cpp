#include <SeqWiresLib/Processors/chordsFromNotesProcessor.hpp>
#include <SeqWiresLib/Features/trackFeature.hpp>
#include <SeqWiresLib/Functions/chordsFromNotesFunction.hpp>

#include <BabelWiresLib/Features/rootFeature.hpp>

seqwires::ChordsFromNotesProcessor::ChordsFromNotesProcessor(const babelwires::ProjectContext& projectContext)
: babelwires::CommonProcessor(projectContext) {
    m_trackIn = m_inputFeature->addField(std::make_unique<TrackFeature>(),
                                              REGISTERED_ID("Notes", "Notes", "f0ef98dd-6b1a-4a11-ac21-5492ec7ce038"));
    m_trackOut = m_outputFeature->addField(std::make_unique<TrackFeature>(),
                                              REGISTERED_ID("Chords", "Chords", "e9edb860-33fb-4c99-bcdc-d746b6bf244e"));
}

seqwires::ChordsFromNotesProcessor::Factory::Factory()
    : CommonProcessorFactory(REGISTERED_LONGID("ChordsFromNotes", "Chords from Notes", "9980627f-22b5-48d1-b26f-911038750568"), 1) {}

void seqwires::ChordsFromNotesProcessor::process(babelwires::UserLogger& userLogger) {
    if (m_trackIn->isChanged(babelwires::Feature::Changes::SomethingChanged)) {
        m_trackOut->set(std::make_unique<Track>(chordsFromNotesFunction(m_trackIn->get())));
    }
}


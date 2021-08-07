/**
 * A processor which extracts a section of sequence data from a track. 
 * 
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include "SeqWiresLib/Processors/concatenateProcessor.hpp"
#include "BabelWires/Features/arrayFeature.hpp"
#include "BabelWires/Features/featureMixins.hpp"
#include "SeqWiresLib/Features/trackFeature.hpp"
#include "SeqWiresLib/Functions/appendTrackFunction.hpp"

#include "BabelWires/Features/Path/fieldName.hpp"

#include <set>

seqwires::ConcatenateProcessor::ConcatenateProcessor()
    : m_inputFeature(std::make_unique<babelwires::RecordFeature>())
    , m_outputFeature(std::make_unique<babelwires::RecordFeature>()) {
    m_tracksIn = m_inputFeature->addField(std::make_unique<babelwires::HasStaticSizeRange<babelwires::StandardArrayFeature<seqwires::TrackFeature>, 2, 16>>(),
                                              FIELD_NAME("Srcs", "Source tracks", "3b8d8cd7-21d9-44a1-877e-134915fe5aca"));
    m_trackOut = m_outputFeature->addField(std::make_unique<TrackFeature>(),
                                              FIELD_NAME("Result", "Result Track", "873d5d66-c5ec-46a4-9aba-f5f4223bdfd4"));
}

seqwires::ConcatenateProcessorFactory::ConcatenateProcessorFactory()
    : ProcessorFactory("ConcatenateProcessor", "Concatenate Processor", 1) {}

std::unique_ptr<babelwires::Processor> seqwires::ConcatenateProcessorFactory::createNewProcessor() const {
    return std::make_unique<ConcatenateProcessor>();
}

babelwires::RecordFeature* seqwires::ConcatenateProcessor::getInputFeature() {
    return m_inputFeature.get();
}

babelwires::RecordFeature* seqwires::ConcatenateProcessor::getOutputFeature() {
    return m_outputFeature.get();
}

void seqwires::ConcatenateProcessor::process(babelwires::UserLogger& userLogger) {
    auto trackOut = std::make_unique<Track>();

    for (int i = 0; i < m_tracksIn->getNumFeatures(); ++i) {
        auto trackFeatureIn = static_cast<const TrackFeature*>(m_tracksIn->getFeature(i));
        if (!trackFeatureIn->isChanged(babelwires::Feature::Changes::SomethingChanged)) {
            return;
        }
    }

    for (int i = 0; i < m_tracksIn->getNumFeatures(); ++i) {
        auto trackFeatureIn = static_cast<const TrackFeature*>(m_tracksIn->getFeature(i));
        appendTrack(*trackOut, trackFeatureIn->get());   
    }
    
    m_trackOut->set(std::move(trackOut));
}

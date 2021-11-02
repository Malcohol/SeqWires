/**
 * A processor which extracts a section of sequence data from a track. 
 * 
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include "SeqWiresLib/Processors/concatenateProcessor.hpp"
#include "BabelWiresLib/Features/arrayFeature.hpp"
#include "BabelWiresLib/Features/featureMixins.hpp"
#include "SeqWiresLib/Features/trackFeature.hpp"
#include "SeqWiresLib/Functions/appendTrackFunction.hpp"

#include "Common/Identifiers/registeredIdentifier.hpp"

#include <set>

seqwires::ConcatenateProcessor::ConcatenateProcessor() {
    m_tracksIn = m_inputFeature->addField(std::make_unique<babelwires::HasStaticSizeRange<babelwires::StandardArrayFeature<seqwires::TrackFeature>, 2, 16>>(),
                                              REGISTERED_ID("Input", "Input tracks", "3b8d8cd7-21d9-44a1-877e-134915fe5aca"));
    m_trackOut = m_outputFeature->addField(std::make_unique<TrackFeature>(),
                                              REGISTERED_ID("Output", "Output Track", "873d5d66-c5ec-46a4-9aba-f5f4223bdfd4"));
}

seqwires::ConcatenateProcessor::Factory::Factory()
    : CommonProcessorFactory("ConcatenateTracks", "Concatenate", 1) {}

void seqwires::ConcatenateProcessor::process(babelwires::UserLogger& userLogger) {
    bool hasChanges = false;

    for (int i = 0; i < m_tracksIn->getNumFeatures(); ++i) {
        auto trackFeatureIn = static_cast<const TrackFeature*>(m_tracksIn->getFeature(i));
        if (trackFeatureIn->isChanged(babelwires::Feature::Changes::SomethingChanged)) {
            hasChanges = true;
        }
    }

    if (!hasChanges) {
        return;
    }

    auto trackOut = std::make_unique<Track>();

    for (int i = 0; i < m_tracksIn->getNumFeatures(); ++i) {
        auto trackFeatureIn = static_cast<const TrackFeature*>(m_tracksIn->getFeature(i));
        appendTrack(*trackOut, trackFeatureIn->get());   
    }
    
    m_trackOut->set(std::move(trackOut));
}

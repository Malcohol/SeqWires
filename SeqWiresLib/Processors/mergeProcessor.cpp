/**
 * A processor which extracts a section of sequence data from a track. 
 * 
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include "SeqWiresLib/Processors/mergeProcessor.hpp"
#include "BabelWiresLib/Features/arrayFeature.hpp"
#include "BabelWiresLib/Features/featureMixins.hpp"
#include "SeqWiresLib/Features/trackFeature.hpp"
#include "SeqWiresLib/Functions/mergeFunction.hpp"

#include "BabelWiresLib/Identifiers/declareIdentifier.hpp"

#include <set>

seqwires::MergeProcessor::MergeProcessor() {
    m_tracksIn = m_inputFeature->addField(std::make_unique<babelwires::HasStaticSizeRange<babelwires::StandardArrayFeature<seqwires::TrackFeature>, 2, 16>>(),
                                              FIELD_NAME("Input", "Input tracks", "80b175ae-c954-4943-96d8-eaffcd7ed6e1"));
    m_trackOut = m_outputFeature->addField(std::make_unique<TrackFeature>(),
                                              FIELD_NAME("Output", "Output Track", "ab56e996-d361-42ed-a0df-44a90a73dc20"));
}

seqwires::MergeProcessor::Factory::Factory()
    : CommonProcessorFactory("MergeTracks", "Merge", 1) {}

void seqwires::MergeProcessor::process(babelwires::UserLogger& userLogger) {
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

    std::vector<const Track*> tracksIn;

    for (int i = 0; i < m_tracksIn->getNumFeatures(); ++i) {
        tracksIn.emplace_back(&static_cast<const TrackFeature*>(m_tracksIn->getFeature(i))->get());
    }

    m_trackOut->set(std::make_unique<Track>(mergeTracks(tracksIn)));
}

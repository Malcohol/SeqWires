/**
 * A processor which extracts a section of sequence data from a track.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <SeqWiresLib/Processors/mergeProcessor.hpp>

#include <SeqWiresLib/Features/trackFeature.hpp>
#include <SeqWiresLib/Functions/mergeFunction.hpp>

#include <BabelWiresLib/Features/arrayFeature.hpp>
#include <BabelWiresLib/Features/featureMixins.hpp>
#include <BabelWiresLib/Features/rootFeature.hpp>

#include <Common/Identifiers/registeredIdentifier.hpp>

#include <set>

seqwires::MergeProcessor::MergeProcessor(const babelwires::ProjectContext& projectContext)
    : babelwires::CommonProcessor(projectContext) {
    m_tracksIn = m_inputFeature->addField(
        std::make_unique<
            babelwires::HasStaticSizeRange<babelwires::StandardArrayFeature<seqwires::TrackFeature>, 2, 16>>(),
        REGISTERED_ID("Input", "Input tracks", "80b175ae-c954-4943-96d8-eaffcd7ed6e1"));
    m_trackOut =
        m_outputFeature->addField(std::make_unique<TrackFeature>(),
                                  REGISTERED_ID("Output", "Output Track", "ab56e996-d361-42ed-a0df-44a90a73dc20"));
}

seqwires::MergeProcessor::Factory::Factory()
    : CommonProcessorFactory(REGISTERED_LONGID("MergeTracks", "Merge", "ed004257-0ae3-44aa-abb9-d752c2eba0c1"), 1) {}

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

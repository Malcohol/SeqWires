/**
 * A processor which repeats sequence data a number of times. 
 * 
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include "SeqWiresLib/Processors/transposeProcessor.hpp"
#include "BabelWires/Features/arrayFeature.hpp"
#include "BabelWires/Features/featureMixins.hpp"
#include "BabelWires/Features/numericFeature.hpp"
#include "SeqWiresLib/Features/trackFeature.hpp"
#include "SeqWiresLib/Functions/transposeFunction.hpp"

#include "BabelWires/Features/Path/fieldName.hpp"

#include <set>

namespace {
    using TransposeArrayFeature =
        babelwires::HasStaticSizeRange<babelwires::StandardArrayFeature<seqwires::TrackFeature>, 1, 16>;
} // namespace

seqwires::TransposeProcessor::TransposeProcessor() {
    m_pitchOffset = m_inputFeature->addField(std::make_unique<babelwires::HasStaticRange<babelwires::IntFeature, -127, 127>>(),
                                       FIELD_NAME("Offset", "Pitch Offset", "5cfa1541-f25e-4671-ac11-2ff71c883418"));
    m_tracksIn = m_inputFeature->addField(std::make_unique<TransposeArrayFeature>(),
                                              FIELD_NAME("Tracks", "Tracks", "83f05b66-7890-4542-8344-1409e50539b5"));
    m_tracksOut = m_outputFeature->addField(std::make_unique<TransposeArrayFeature>(),
                                                FIELD_NAME("Tracks", "Tracks", "5b86d6db-97f5-409c-96b9-b56b70902799"));
}

seqwires::TransposeProcessor::Factory::Factory()
    : CommonProcessorFactory("TransposeTracks", "Transpose", 1) {}

void seqwires::TransposeProcessor::process(babelwires::UserLogger& userLogger) {
    if (m_tracksIn->isChanged(babelwires::Feature::Changes::StructureChanged)) {
        // Keep the out structure in sync with the in structure, and
        // try to ensure that tracks which corresponding before still
        // correspond afterwards.
        m_tracksOut->copyStructureFrom(*m_tracksIn);
    }

    const bool offsetChanged = m_pitchOffset->isChanged(babelwires::Feature::Changes::SomethingChanged);

    for (int i = 0; i < m_tracksIn->getNumFeatures(); ++i) {
        auto trackFeatureIn = static_cast<const TrackFeature*>(m_tracksIn->getFeature(i));
        if (offsetChanged || trackFeatureIn->isChanged(babelwires::Feature::Changes::SomethingChanged)) {
            auto trackOut = transposeTrack(trackFeatureIn->get(), m_pitchOffset->get());
            static_cast<TrackFeature*>(m_tracksOut->getFeature(i))->set(std::move(trackOut));
        }
    }
}

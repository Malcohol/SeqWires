/**
 * A processor which extracts a section of sequence data from a track. 
 * 
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include "SeqWiresLib/Processors/excerptProcessor.hpp"
#include "BabelWires/Features/arrayFeature.hpp"
#include "BabelWires/Features/featureMixins.hpp"
#include "BabelWires/Features/numericFeature.hpp"
#include "SeqWiresLib/Features/trackFeature.hpp"
#include "SeqWiresLib/Functions/excerptFunction.hpp"

#include "BabelWires/Features/Path/fieldName.hpp"

#include <set>

namespace {
    using NonNegativeRationalFeature =
        babelwires::HasStaticRange<babelwires::RationalFeature, 0, std::numeric_limits<int>::max()>;
    using ExcerptArrayFeature =
        babelwires::HasStaticSizeRange<babelwires::StandardArrayFeature<seqwires::TrackFeature>, 1, 16>;
} // namespace

seqwires::ExcerptProcessor::ExcerptProcessor() {
    m_start = m_inputFeature->addField(std::make_unique<NonNegativeRationalFeature>(),
                                       FIELD_NAME("Start", "Start", "4b95f5db-a542-4660-a8db-97d3a5f831ca"));
    m_duration = m_inputFeature->addField(std::make_unique<NonNegativeRationalFeature>(),
                                          FIELD_NAME("Duratn", "Duration", "d83ebbc2-1492-4578-a3b8-4969eb6a2042"));
    m_tracksIn = m_inputFeature->addField(std::make_unique<ExcerptArrayFeature>(),
                                              FIELD_NAME("Notes", "Notes", "983b3bcb-7086-4791-8e18-d8c7550d45d3"));
    m_tracksOut = m_outputFeature->addField(std::make_unique<ExcerptArrayFeature>(),
                                                FIELD_NAME("Notes", "Notes", "9feb0f11-fafb-4744-92f1-87eb34b30747"));
}

seqwires::ExcerptProcessor::Factory::Factory()
    : CommonProcessorFactory("ExcerptProcessor", "Excerpt Processor", 1) {}

void seqwires::ExcerptProcessor::process(babelwires::UserLogger& userLogger) {
    if (m_tracksIn->isChanged(babelwires::Feature::Changes::StructureChanged)) {
        // Keep the out structure in sync with the in structure, and
        // try to ensure that tracks which corresponding before still
        // correspond afterwards.
        m_tracksOut->copyStructureFrom(*m_tracksIn);
    }

    const bool durationChanged = m_start->isChanged(babelwires::Feature::Changes::SomethingChanged) ||
                                 m_duration->isChanged(babelwires::Feature::Changes::SomethingChanged);

    const ModelDuration start = m_start->get();
    const ModelDuration duration = m_duration->get();
    for (int i = 0; i < m_tracksIn->getNumFeatures(); ++i) {
        auto trackFeatureIn = static_cast<const TrackFeature*>(m_tracksIn->getFeature(i));
        if (durationChanged || trackFeatureIn->isChanged(babelwires::Feature::Changes::SomethingChanged)) {
            auto trackOut = getTrackExcerpt(trackFeatureIn->get(), start, duration);
            static_cast<TrackFeature*>(m_tracksOut->getFeature(i))->set(std::move(trackOut));
        }
    }
}

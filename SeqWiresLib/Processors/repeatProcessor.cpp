/**
 * A processor which repeats sequence data a number of times. 
 * 
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include "SeqWiresLib/Processors/repeatProcessor.hpp"
#include "BabelWires/Features/arrayFeature.hpp"
#include "BabelWires/Features/featureMixins.hpp"
#include "BabelWires/Features/numericFeature.hpp"
#include "SeqWiresLib/Features/trackFeature.hpp"
#include "SeqWiresLib/Functions/appendTrackFunction.hpp"

#include "BabelWires/Features/Path/fieldName.hpp"

#include <set>

namespace {
    using NonNegativeIntFeature =
        babelwires::HasStaticRange<babelwires::IntFeature, 0, std::numeric_limits<int>::max()>;
    using RepeatArrayFeature =
        babelwires::HasStaticSizeRange<babelwires::StandardArrayFeature<seqwires::TrackFeature>, 1, 16>;
} // namespace

seqwires::RepeatProcessor::RepeatProcessor()
    : m_inputFeature(std::make_unique<babelwires::RecordFeature>())
    , m_outputFeature(std::make_unique<babelwires::RecordFeature>()) {
    m_count = m_inputFeature->addField(std::make_unique<NonNegativeIntFeature>(),
                                       FIELD_NAME("Count", "Count", "f5d2ab08-4430-47fa-b26c-0ff2154826e3"));
    m_tracksIn = m_inputFeature->addField(std::make_unique<RepeatArrayFeature>(),
                                              FIELD_NAME("Tracks", "Tracks", "f727937f-0215-4527-bab4-0eca269d6c5c"));
    m_tracksOut = m_outputFeature->addField(std::make_unique<RepeatArrayFeature>(),
                                                FIELD_NAME("Tracks", "Tracks", "f7e8dc03-4e1b-4dce-ba9a-6b54e9cba6f4"));
}

seqwires::RepeatProcessorFactory::RepeatProcessorFactory()
    : ProcessorFactory("RepeatProcessor", "Repeat Processor", 1) {}

std::unique_ptr<babelwires::Processor> seqwires::RepeatProcessorFactory::createNewProcessor() const {
    return std::make_unique<RepeatProcessor>();
}

babelwires::RecordFeature* seqwires::RepeatProcessor::getInputFeature() {
    return m_inputFeature.get();
}

babelwires::RecordFeature* seqwires::RepeatProcessor::getOutputFeature() {
    return m_outputFeature.get();
}

void seqwires::RepeatProcessor::process(babelwires::UserLogger& userLogger) {
    if (m_tracksIn->isChanged(babelwires::Feature::Changes::StructureChanged)) {
        // Keep the out structure in sync with the in structure, and
        // try to ensure that tracks which corresponding before still
        // correspond afterwards.
        m_tracksOut->copyStructureFrom(*m_tracksIn);
    }

    const bool countChanged = m_count->isChanged(babelwires::Feature::Changes::SomethingChanged);

    const int count = m_count->get();
    for (int i = 0; i < m_tracksIn->getNumFeatures(); ++i) {
        auto trackFeatureIn = static_cast<const TrackFeature*>(m_tracksIn->getFeature(i));
        if (countChanged || trackFeatureIn->isChanged(babelwires::Feature::Changes::SomethingChanged)) {
            auto trackOut = std::make_unique<Track>();

            const Track& trackIn = trackFeatureIn->get();
            for (int i = 0; i < count; ++i) {
                appendTrack(*trackOut, trackIn);
            }
            static_cast<TrackFeature*>(m_tracksOut->getFeature(i))->set(std::move(trackOut));
        }
    }
}

#include "SeqwiresLib/Processors/excerptProcessor.hpp"
#include "BabelWires/Features/arrayFeature.hpp"
#include "BabelWires/Features/featureMixins.hpp"
#include "BabelWires/Features/numericFeature.hpp"
#include "SeqwiresLib/Features/trackFeature.hpp"
#include "SeqwiresLib/Functions/excerptFunction.hpp"

#include "BabelWires/Features/Path/fieldName.hpp"

#include <set>

namespace {
    using NonNegativeRationalFeature =
        babelwires::HasStaticRange<babelwires::RationalFeature, 0, std::numeric_limits<int>::max()>;
    using ExcerptArrayFeature =
        babelwires::HasStaticSizeRange<babelwires::StandardArrayFeature<seqwires::TrackFeature>, 1, 16>;
} // namespace

seqwires::ExcerptProcessor::ExcerptProcessor()
    : m_inputFeature(std::make_unique<babelwires::RecordFeature>())
    , m_outputFeature(std::make_unique<babelwires::RecordFeature>()) {
    m_start = m_inputFeature->addField(std::make_unique<NonNegativeRationalFeature>(),
                                       FIELD_NAME("Start", "Start", "4b95f5db-a542-4660-a8db-97d3a5f831ca"));
    m_duration = m_inputFeature->addField(std::make_unique<NonNegativeRationalFeature>(),
                                          FIELD_NAME("Duratn", "Duration", "d83ebbc2-1492-4578-a3b8-4969eb6a2042"));
    m_noteTracksIn = m_inputFeature->addField(std::make_unique<ExcerptArrayFeature>(),
                                              FIELD_NAME("Notes", "Notes", "983b3bcb-7086-4791-8e18-d8c7550d45d3"));
    m_noteTracksOut = m_outputFeature->addField(std::make_unique<ExcerptArrayFeature>(),
                                                FIELD_NAME("Notes", "Notes", "9feb0f11-fafb-4744-92f1-87eb34b30747"));
}

seqwires::ExcerptProcessorFactory::ExcerptProcessorFactory()
    : ProcessorFactory("ExcerptProcessor", "Excerpt Processor", 1) {}

std::unique_ptr<babelwires::Processor> seqwires::ExcerptProcessorFactory::createNewProcessor() const {
    return std::make_unique<ExcerptProcessor>();
}

babelwires::RecordFeature* seqwires::ExcerptProcessor::getInputFeature() {
    return m_inputFeature.get();
}

babelwires::RecordFeature* seqwires::ExcerptProcessor::getOutputFeature() {
    return m_outputFeature.get();
}

void seqwires::ExcerptProcessor::process(babelwires::UserLogger& userLogger) {
    if (m_noteTracksIn->isChanged(babelwires::Feature::Changes::StructureChanged)) {
        // Keep the out structure in sync with the in structure, and
        // try to ensure that tracks which corresponding before still
        // correspond afterwards.
        m_noteTracksOut->copyStructureFrom(*m_noteTracksIn);
    }

    const bool durationChanged = m_start->isChanged(babelwires::Feature::Changes::SomethingChanged) ||
                                 m_duration->isChanged(babelwires::Feature::Changes::SomethingChanged);

    const ModelDuration start = m_start->get();
    const ModelDuration duration = m_duration->get();
    for (int i = 0; i < m_noteTracksIn->getNumFeatures(); ++i) {
        auto trackFeatureIn = static_cast<const TrackFeature*>(m_noteTracksIn->getFeature(i));
        if (durationChanged || trackFeatureIn->isChanged(babelwires::Feature::Changes::SomethingChanged)) {
            auto trackOut = getTrackExcerpt(trackFeatureIn->get(), start, duration);
            static_cast<TrackFeature*>(m_noteTracksOut->getFeature(i))->set(std::move(trackOut));
        }
    }
}

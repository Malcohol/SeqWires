/**
 * A processor which creates a silent track of a certain duration.
 * 
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include "SeqWiresLib/Processors/silenceProcessor.hpp"
#include "BabelWires/Features/arrayFeature.hpp"
#include "BabelWires/Features/featureMixins.hpp"
#include "BabelWires/Features/numericFeature.hpp"
#include "SeqWiresLib/Features/trackFeature.hpp"
#include "SeqWiresLib/Functions/appendTrackFunction.hpp"

#include "BabelWires/Features/Path/fieldName.hpp"

#include <set>

namespace {
    using NonNegativeRationalFeature =
        babelwires::HasStaticRange<babelwires::RationalFeature, 0, std::numeric_limits<int>::max()>;
} // namespace

seqwires::SilenceProcessor::SilenceProcessor()
    : m_inputFeature(std::make_unique<babelwires::RecordFeature>())
    , m_outputFeature(std::make_unique<babelwires::RecordFeature>()) {
    m_duration = m_inputFeature->addField(std::make_unique<NonNegativeRationalFeature>(),
                                       FIELD_NAME("Durn", "Duration", "05d3ea91-cb90-42f5-9988-2fb2e02e231c"));
    m_trackOut = m_outputFeature->addField(std::make_unique<TrackFeature>(),
                                                FIELD_NAME("Track", "Track", "86f3d028-a616-4a95-a566-a010ffcabb19"));
}

seqwires::SilenceProcessorFactory::SilenceProcessorFactory()
    : ProcessorFactory("SilenceProcessor", "Silence Processor", 1) {}

std::unique_ptr<babelwires::Processor> seqwires::SilenceProcessorFactory::createNewProcessor() const {
    return std::make_unique<SilenceProcessor>();
}

babelwires::RecordFeature* seqwires::SilenceProcessor::getInputFeature() {
    return m_inputFeature.get();
}

babelwires::RecordFeature* seqwires::SilenceProcessor::getOutputFeature() {
    return m_outputFeature.get();
}

void seqwires::SilenceProcessor::process(babelwires::UserLogger& userLogger) {
    if (m_duration->isChanged(babelwires::Feature::Changes::SomethingChanged)) {
        auto trackOut = std::make_unique<Track>();
        trackOut->setDuration(m_duration->get());
        m_trackOut->set(std::move(trackOut));
    }
}

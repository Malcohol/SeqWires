/**
 * A processor which creates a silent track of a certain duration.
 * 
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include "SeqWiresLib/Processors/silenceProcessor.hpp"
#include "BabelWiresLib/Features/arrayFeature.hpp"
#include "BabelWiresLib/Features/featureMixins.hpp"
#include "BabelWiresLib/Features/numericFeature.hpp"
#include "SeqWiresLib/Features/trackFeature.hpp"
#include "SeqWiresLib/Features/durationFeature.hpp"
#include "SeqWiresLib/Functions/appendTrackFunction.hpp"

#include "BabelWiresLib/Identifiers/declareIdentifier.hpp"

seqwires::SilenceProcessor::SilenceProcessor() {
    m_duration = m_inputFeature->addField(std::make_unique<DurationFeature>(),
                                       FIELD_NAME("Durn", "Duration", "05d3ea91-cb90-42f5-9988-2fb2e02e231c"));
    m_trackOut = m_outputFeature->addField(std::make_unique<TrackFeature>(),
                                                FIELD_NAME("Track", "Track", "86f3d028-a616-4a95-a566-a010ffcabb19"));
}

seqwires::SilenceProcessor::Factory::Factory()
    : CommonProcessorFactory("SilentTrack", "Silence", 1) {}

void seqwires::SilenceProcessor::process(babelwires::UserLogger& userLogger) {
    if (m_duration->isChanged(babelwires::Feature::Changes::SomethingChanged)) {
        auto trackOut = std::make_unique<Track>();
        trackOut->setDuration(m_duration->get());
        m_trackOut->set(std::move(trackOut));
    }
}

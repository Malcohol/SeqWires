/**
 * A processor which repeats sequence data a number of times. 
 * 
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include "SeqWiresLib/Processors/splitAtPitchProcessor.hpp"

#include "SeqWiresLib/Features/trackFeature.hpp"
#include "SeqWiresLib/Functions/splitAtPitchFunction.hpp"
#include "SeqWiresLib/Features/pitchFeature.hpp"

#include "BabelWiresLib/Features/arrayFeature.hpp"
#include "BabelWiresLib/Features/featureMixins.hpp"
#include "BabelWiresLib/Features/numericFeature.hpp"
#include "BabelWiresLib/Features/rootFeature.hpp"

#include "Common/Identifiers/registeredIdentifier.hpp"

#include <set>

seqwires::SplitAtPitchProcessor::SplitAtPitchProcessor(const babelwires::ProjectContext& projectContext)
: CommonProcessor(projectContext) {
    m_pitch = m_inputFeature->addField(std::make_unique<PitchFeature>(),
                                       REGISTERED_ID("Pitch", "Pitch", "6b721baa-084f-450b-bf35-2e08a9592958"));
    m_trackIn = m_inputFeature->addField(std::make_unique<TrackFeature>(),
                                              REGISTERED_ID("Input", "Input Track", "9314a43f-256a-4915-b218-f2ba37133863"));
    m_equalOrAboveTrackOut = m_outputFeature->addField(std::make_unique<TrackFeature>(),
                                                REGISTERED_ID("Above", "Notes at/above", "4c65b7f1-e546-4df2-9891-23623b74bc23"));
    m_belowTrackOut = m_outputFeature->addField(std::make_unique<TrackFeature>(),
                                                REGISTERED_ID("Below", "Notes below", "ab6a1d57-8c77-4df9-baf7-4b24136d9279"));
    m_otherTrackOut = m_outputFeature->addField(std::make_unique<TrackFeature>(),
                                                REGISTERED_ID("Other", "Other", "83bf663c-2931-467f-8403-b12e18138c68"));
}

seqwires::SplitAtPitchProcessor::Factory::Factory()
    : CommonProcessorFactory(REGISTERED_LONGID("SplitAtPitchProcessor", "Split At Pitch", "9f5076f5-42a1-4655-a71e-b6b06a4e92cc"), 1) {}

void seqwires::SplitAtPitchProcessor::process(babelwires::UserLogger& userLogger) {
    if (m_pitch->isChanged(babelwires::Feature::Changes::SomethingChanged) || m_trackIn->isChanged(babelwires::Feature::Changes::SomethingChanged)) {
        auto newTracksOut = splitAtPitch(m_pitch->get(), m_trackIn->get());
        m_equalOrAboveTrackOut->set(std::move(newTracksOut.m_equalOrAbove));
        m_belowTrackOut->set(std::move(newTracksOut.m_below));
        m_otherTrackOut->set(std::move(newTracksOut.m_other));
    }
}

/**
 * A processor which repeats sequence data a number of times.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include "SeqWiresLib/Processors/monophonicSubtracksProcessor.hpp"
#include "SeqWiresLib/Features/trackFeature.hpp"
#include "SeqWiresLib/Functions/monophonicSubtracksFunction.hpp"

#include "BabelWiresLib/Features/arrayFeature.hpp"
#include "BabelWiresLib/Features/enumFeature.hpp"
#include "BabelWiresLib/Features/featureMixins.hpp"
#include "BabelWiresLib/Features/numericFeature.hpp"

#include "BabelWiresLib/Features/Path/fieldName.hpp"
#include "BabelWiresLib/ValueNames/valueNamesImpl.hpp"

#include <set>

namespace {
    using SubtrackArrayFeature =
        babelwires::HasStaticSizeRange<babelwires::StandardArrayFeature<seqwires::TrackFeature>, 1, 16>;
} // namespace

namespace {
    struct PolicyFeature : babelwires::EnumFeatureImpl<seqwires::MonophonicSubtracksPolicyEnum> {};
} // namespace

seqwires::MonophonicSubtracksProcessor::MonophonicSubtracksProcessor() {
    m_numSubtracks =
        m_inputFeature->addField(std::make_unique<babelwires::HasStaticRange<babelwires::IntFeature, 1, 16>>(),
                                 FIELD_NAME("NumTrk", "Num subtracks", "036ba53e-fdf5-4278-a2c3-7232fc10731c"));
    m_policy = m_inputFeature->addField(std::make_unique<PolicyFeature>(),
                                        FIELD_NAME("Policy", "Policy", "6dca88e9-a6ec-4d43-adb8-78b7bfa00ab9"));
    m_trackIn = m_inputFeature->addField(std::make_unique<TrackFeature>(),
                                         FIELD_NAME("Input", "Input Track", "7e50ba70-0c5e-4493-b088-a3327d65256f"));
    m_tracksOut =
        m_outputFeature->addField(std::make_unique<SubtrackArrayFeature>(),
                                  FIELD_NAME("Sbtrks", "Mono tracks", "d3e08407-a6e1-4b1f-b6f7-8fa9be6bdf5f"));
    m_otherTrackOut = m_outputFeature->addField(std::make_unique<TrackFeature>(),
                                                FIELD_NAME("Other", "Other", "bc69fa38-8727-40c3-a90e-bfe6de8ad847"));
}

seqwires::MonophonicSubtracksProcessor::Factory::Factory()
    : CommonProcessorFactory("MonophonicSubtracksProcessor", "Monophonic subtracks", 1) {}

void seqwires::MonophonicSubtracksProcessor::process(babelwires::UserLogger& userLogger) {
    if (m_numSubtracks->isChanged(babelwires::Feature::Changes::SomethingChanged)) {
        m_tracksOut->setSize(m_numSubtracks->get());
    }
    if (m_numSubtracks->isChanged(babelwires::Feature::Changes::SomethingChanged) ||
        m_trackIn->isChanged(babelwires::Feature::Changes::SomethingChanged) ||
        m_policy->isChanged(babelwires::Feature::Changes::SomethingChanged)) {
        auto result = getMonophonicSubtracks(m_trackIn->get(), m_numSubtracks->get(),
                                             static_cast<const PolicyFeature*>(m_policy)->getAsValue());
        for (int i = 0; i < result.m_noteTracks.size(); ++i) {
            assert(m_tracksOut->getFeature(i)->as<TrackFeature>() != nullptr);
            auto trackOut = static_cast<TrackFeature*>(m_tracksOut->getFeature(i));
            trackOut->set(std::move(result.m_noteTracks[i]));
        }
        m_otherTrackOut->set(std::move(result.m_other));
    }
}

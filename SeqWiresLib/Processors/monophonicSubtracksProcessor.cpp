/**
 * A processor which repeats sequence data a number of times.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <SeqWiresLib/Processors/monophonicSubtracksProcessor.hpp>

#include <SeqWiresLib/Features/trackFeature.hpp>
#include <SeqWiresLib/Functions/monophonicSubtracksFunction.hpp>

#include <BabelWiresLib/Features/arrayFeature.hpp>
#include <BabelWiresLib/Enums/enumFeature.hpp>
#include <BabelWiresLib/Features/featureMixins.hpp>
#include <BabelWiresLib/Features/numericFeature.hpp>
#include <BabelWiresLib/Features/rootFeature.hpp>

#include <Common/Identifiers/registeredIdentifier.hpp>

#include <set>

namespace {
    using SubtrackArrayFeature =
        babelwires::HasStaticSizeRange<babelwires::StandardArrayFeature<seqwires::TrackFeature>, 1, 16>;

    using PolicyFeature = babelwires::EnumWithCppEnumFeature<seqwires::MonophonicSubtracksPolicyEnum>;
} // namespace

seqwires::MonophonicSubtracksProcessor::MonophonicSubtracksProcessor(const babelwires::ProjectContext& projectContext)
: CommonProcessor(projectContext) {
    m_numSubtracks =
        m_inputFeature->addField(std::make_unique<babelwires::HasStaticRange<babelwires::IntFeature, 1, 16>>(),
                                 BW_SHORT_ID("NumTrk", "Num subtracks", "036ba53e-fdf5-4278-a2c3-7232fc10731c"));
    m_policy = m_inputFeature->addField(std::make_unique<PolicyFeature>(),
                                        BW_SHORT_ID("Policy", "Policy", "6dca88e9-a6ec-4d43-adb8-78b7bfa00ab9"));
    m_trackIn = m_inputFeature->addField(std::make_unique<TrackFeature>(),
                                         BW_SHORT_ID("Input", "Input Track", "7e50ba70-0c5e-4493-b088-a3327d65256f"));
    m_tracksOut =
        m_outputFeature->addField(std::make_unique<SubtrackArrayFeature>(),
                                  BW_SHORT_ID("Sbtrks", "Mono tracks", "d3e08407-a6e1-4b1f-b6f7-8fa9be6bdf5f"));
    m_otherTrackOut = m_outputFeature->addField(std::make_unique<TrackFeature>(),
                                                BW_SHORT_ID("Other", "Other", "bc69fa38-8727-40c3-a90e-bfe6de8ad847"));
}

seqwires::MonophonicSubtracksProcessor::Factory::Factory()
    : CommonProcessorFactory(BW_LONG_ID("MonoSubtracksProcessor", "Monophonic subtracks", "0b131aaf-91ce-4552-9206-a680615775b1"), 1) {}

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

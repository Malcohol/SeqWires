/**
 * A processor which repeats sequence data a number of times.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <SeqWiresLib/Processors/transposeProcessor.hpp>

#include <SeqWiresLib/Features/trackFeature.hpp>
#include <SeqWiresLib/Functions/transposeFunction.hpp>

#include <BabelWiresLib/Features/arrayFeature.hpp>
#include <BabelWiresLib/Features/featureMixins.hpp>
#include <BabelWiresLib/Features/rootFeature.hpp>
#include <BabelWiresLib/Types/Int/intFeature.hpp>

#include <Common/Identifiers/registeredIdentifier.hpp>

#include <set>

namespace {
    using TransposeArrayFeature =
        babelwires::HasStaticSizeRange<babelwires::StandardArrayFeature<seqwires::TrackFeature>, 1, 16>;
} // namespace

seqwires::TransposeProcessor::TransposeProcessor(const babelwires::ProjectContext& projectContext)
    : babelwires::ParallelProcessor<seqwires::TrackFeature, seqwires::TrackFeature>(projectContext) {
    m_pitchOffset =
        m_inputFeature->addField(std::make_unique<babelwires::IntFeature2>(-127, 127),
                                 BW_SHORT_ID("Offset", "Pitch Offset", "5cfa1541-f25e-4671-ac11-2ff71c883418"));
    addArrayFeature(BW_SHORT_ID("Tracks", "Tracks", "83f05b66-7890-4542-8344-1409e50539b5"));
}

seqwires::TransposeProcessor::Factory::Factory()
    : CommonProcessorFactory(BW_LONG_ID("TransposeTracks", "Transpose", "3414f6cf-290a-421e-bce5-6a98ed0483af"), 1) {}

void seqwires::TransposeProcessor::processEntry(babelwires::UserLogger& userLogger, const seqwires::TrackFeature& input,
                                                seqwires::TrackFeature& output) const {
    auto trackOut = transposeTrack(input.get(), m_pitchOffset->get());
    output.set(std::move(trackOut));
}

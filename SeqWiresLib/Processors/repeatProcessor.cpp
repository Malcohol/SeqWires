/**
 * A processor which repeats sequence data a number of times.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <SeqWiresLib/Processors/repeatProcessor.hpp>

#include <SeqWiresLib/Features/trackFeature.hpp>
#include <SeqWiresLib/Functions/appendTrackFunction.hpp>

#include <BabelWiresLib/Features/arrayFeature.hpp>
#include <BabelWiresLib/Features/featureMixins.hpp>
#include <BabelWiresLib/Features/rootFeature.hpp>
#include <BabelWiresLib/Types/Int/intFeature.hpp>
#include <BabelWiresLib/Types/Int/intValue.hpp>

#include <Common/Identifiers/registeredIdentifier.hpp>

#include <set>

namespace {
    using RepeatArrayFeature =
        babelwires::HasStaticSizeRange<babelwires::StandardArrayFeature<seqwires::TrackFeature>, 1, 16>;
} // namespace

seqwires::RepeatProcessor::RepeatProcessor(const babelwires::ProjectContext& projectContext)
    : babelwires::ParallelProcessor<seqwires::TrackFeature, seqwires::TrackFeature>(projectContext) {
    m_count = m_inputFeature->addField(
        std::make_unique<babelwires::IntFeature>(0, std::numeric_limits<babelwires::IntValue::NativeType>::max(), 2),
        BW_SHORT_ID("Count", "Count", "f5d2ab08-4430-47fa-b26c-0ff2154826e3"));
    addArrayFeature(BW_SHORT_ID("Tracks", "Tracks", "f727937f-0215-4527-bab4-0eca269d6c5c"));
}

seqwires::RepeatProcessor::Factory::Factory()
    : CommonProcessorFactory(BW_LONG_ID("RepeatTracks", "Repeat", "6c5b3e89-bb57-4c90-8a66-1d8cdeb29db9"), 1) {}

void seqwires::RepeatProcessor::processEntry(babelwires::UserLogger& userLogger, const seqwires::TrackFeature& input,
                                             seqwires::TrackFeature& output) const {
    auto trackOut = std::make_unique<Track>();

    const Track& trackIn = input.get();
    for (int i = 0; i < m_count->get(); ++i) {
        appendTrack(*trackOut, trackIn);
    }
    output.set(std::move(trackOut));
}

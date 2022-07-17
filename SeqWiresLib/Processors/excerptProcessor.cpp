/**
 * A processor which extracts a section of sequence data from a track.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <SeqWiresLib/Processors/excerptProcessor.hpp>
#include <SeqWiresLib/Features/durationFeature.hpp>
#include <SeqWiresLib/Features/trackFeature.hpp>
#include <SeqWiresLib/Functions/excerptFunction.hpp>

#include <BabelWiresLib/Features/arrayFeature.hpp>
#include <BabelWiresLib/Features/featureMixins.hpp>
#include <BabelWiresLib/Features/numericFeature.hpp>
#include <BabelWiresLib/Features/rootFeature.hpp>

#include <Common/Identifiers/registeredIdentifier.hpp>

namespace {
    using ExcerptArrayFeature =
        babelwires::HasStaticSizeRange<babelwires::StandardArrayFeature<seqwires::TrackFeature>, 1, 16>;
} // namespace

seqwires::ExcerptProcessor::ExcerptProcessor(const babelwires::ProjectContext& projectContext)
    : babelwires::ParallelProcessor<seqwires::TrackFeature, seqwires::TrackFeature>(projectContext) {
    m_start = m_inputFeature->addField(std::make_unique<DurationFeature>(),
                                       REGISTERED_ID("Start", "Start", "4b95f5db-a542-4660-a8db-97d3a5f831ca"));
    m_duration = m_inputFeature->addField(std::make_unique<DurationFeature>(),
                                          REGISTERED_ID("Duratn", "Duration", "d83ebbc2-1492-4578-a3b8-4969eb6a2042"));
    addArrayFeature(REGISTERED_ID("Tracks", "Tracks", "983b3bcb-7086-4791-8e18-d8c7550d45d3"));
}

seqwires::ExcerptProcessor::Factory::Factory()
    : CommonProcessorFactory(REGISTERED_LONGID("TrackExcerpt", "Excerpt", "83c74dba-7861-447c-9abb-0b4439061baf"), 1) {}

void seqwires::ExcerptProcessor::processEntry(babelwires::UserLogger& userLogger, const seqwires::TrackFeature& input,
                                              seqwires::TrackFeature& output) const {
    auto trackOut = getTrackExcerpt(input.get(), m_start->get(), m_duration->get());
    output.set(std::move(trackOut));
}

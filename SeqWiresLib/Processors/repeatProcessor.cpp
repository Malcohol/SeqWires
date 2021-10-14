/**
 * A processor which repeats sequence data a number of times. 
 * 
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include "SeqWiresLib/Processors/repeatProcessor.hpp"
#include "BabelWiresLib/Features/arrayFeature.hpp"
#include "BabelWiresLib/Features/featureMixins.hpp"
#include "BabelWiresLib/Features/numericFeature.hpp"
#include "SeqWiresLib/Features/trackFeature.hpp"
#include "SeqWiresLib/Functions/appendTrackFunction.hpp"

#include "BabelWiresLib/Identifiers/declareIdentifier.hpp"

#include <set>

namespace {
    using NonNegativeIntFeature =
        babelwires::HasStaticDefault<babelwires::HasStaticRange<babelwires::IntFeature, 0, std::numeric_limits<int>::max()>, 2>;
    using RepeatArrayFeature =
        babelwires::HasStaticSizeRange<babelwires::StandardArrayFeature<seqwires::TrackFeature>, 1, 16>;
} // namespace

seqwires::RepeatProcessor::RepeatProcessor() {
    m_count = m_inputFeature->addField(std::make_unique<NonNegativeIntFeature>(),
                                       REGISTERED_ID("Count", "Count", "f5d2ab08-4430-47fa-b26c-0ff2154826e3"));
    addArrayFeature(REGISTERED_ID("Tracks", "Tracks", "f727937f-0215-4527-bab4-0eca269d6c5c"));
}

seqwires::RepeatProcessor::Factory::Factory()
    : CommonProcessorFactory("RepeatTracks", "Repeat", 1) {}

void seqwires::RepeatProcessor::processEntry(babelwires::UserLogger& userLogger, const seqwires::TrackFeature& input, seqwires::TrackFeature& output) const {
    auto trackOut = std::make_unique<Track>();

    const Track& trackIn = input.get();
    for (int i = 0; i < m_count->get(); ++i) {
        appendTrack(*trackOut, trackIn);
    }
    output.set(std::move(trackOut));
}

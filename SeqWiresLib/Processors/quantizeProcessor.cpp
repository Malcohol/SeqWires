/**
 * A processor which repeats sequence data a number of times.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <SeqWiresLib/Processors/quantizeProcessor.hpp>

#include <SeqWiresLib/Types/Track/trackFeature.hpp>
#include <SeqWiresLib/Functions/quantizeFunction.hpp>

#include <BabelWiresLib/Features/arrayFeature.hpp>
#include <BabelWiresLib/Features/rootFeature.hpp>
#include <BabelWiresLib/Types/Rational/rationalFeature.hpp>

#include <Common/Identifiers/registeredIdentifier.hpp>

#include <set>

namespace {
    using QuantizeArrayFeature =
        babelwires::HasStaticSizeRange<babelwires::StandardArrayFeature<seqwires::TrackFeature>, 1, 16>;

    // TODO: Allow ranges to describe open and closed intervals.
    struct StrictlyPositiveRational : babelwires::RationalFeature {
        StrictlyPositiveRational()
            : RationalFeature(0, std::numeric_limits<babelwires::Rational::ComponentType>::max(),
                              babelwires::Rational(1, 16)) {}

/*
        void set(babelwires::Rational value) override {
            if (value == 0) {
                throw babelwires::ModelException() << "Zero is not a valid value";
            }
            return babelwires::RationalFeature::set(value);
        }
        */
    };
} // namespace

seqwires::QuantizeProcessor::QuantizeProcessor(const babelwires::ProjectContext& projectContext)
    : babelwires::ParallelProcessor<seqwires::TrackFeature, seqwires::TrackFeature>(projectContext) {
    m_beat = m_inputFeature->addField(std::make_unique<StrictlyPositiveRational>(),
                                      BW_SHORT_ID("Beat", "Beat", "1651ab49-3313-4cd3-b92d-16742b7f5921"));
    addArrayFeature(BW_SHORT_ID("Tracks", "Tracks", "e00623bf-c0f0-4fee-b6c4-4f65df896bf3"));
}

seqwires::QuantizeProcessor::Factory::Factory()
    : CommonProcessorFactory(BW_LONG_ID("QuantizeTracks", "Quantize", "1ae89077-2cfb-4071-910c-2f5dcfc85b17"), 1) {}

void seqwires::QuantizeProcessor::processEntry(babelwires::UserLogger& userLogger, const seqwires::TrackFeature& input,
                                               seqwires::TrackFeature& output) const {
    auto trackOut = quantize(input.get(), m_beat->get());
    output.set(std::move(trackOut));
}

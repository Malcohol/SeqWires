/**
 * A processor which repeats sequence data a number of times.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <SeqWiresLib/Processors/quantizeProcessor.hpp>

#include <SeqWiresLib/Features/trackFeature.hpp>
#include <SeqWiresLib/Functions/quantizeFunction.hpp>

#include <BabelWiresLib/Features/arrayFeature.hpp>
#include <BabelWiresLib/Features/featureMixins.hpp>
#include <BabelWiresLib/Features/numericFeature.hpp>
#include <BabelWiresLib/Features/rootFeature.hpp>

#include <Common/Identifiers/registeredIdentifier.hpp>

#include <set>

namespace {
    using QuantizeArrayFeature =
        babelwires::HasStaticSizeRange<babelwires::StandardArrayFeature<seqwires::TrackFeature>, 1, 16>;

    // TODO: Allow ranges to describe open and closed intervals.
    struct StrictlyPositiveRational : babelwires::RationalFeature {
        babelwires::Range<babelwires::Rational> getRange() const override {
            return babelwires::Range<babelwires::Rational>(0);
        }

        void set(babelwires::Rational value) override {
            if (value == 0) {
                throw babelwires::ModelException() << "Zero is not a valid value";
            }
            return babelwires::RationalFeature::set(value);
        }

        void doSetToDefault() override {
            babelwires::RationalFeature::set(babelwires::Rational(1, 16));
        }
    };
} // namespace

seqwires::QuantizeProcessor::QuantizeProcessor(const babelwires::ProjectContext& projectContext)
    : babelwires::ParallelProcessor<seqwires::TrackFeature, seqwires::TrackFeature>(projectContext) {
    m_beat =
        m_inputFeature->addField(std::make_unique<StrictlyPositiveRational>(),
                                 REGISTERED_ID("Beat", "Beat", "1651ab49-3313-4cd3-b92d-16742b7f5921"));
    addArrayFeature(REGISTERED_ID("Tracks", "Tracks", "e00623bf-c0f0-4fee-b6c4-4f65df896bf3"));
}

seqwires::QuantizeProcessor::Factory::Factory()
    : CommonProcessorFactory(REGISTERED_LONGID("QuantizeTracks", "Quantize", "1ae89077-2cfb-4071-910c-2f5dcfc85b17"),
                             1) {}

void seqwires::QuantizeProcessor::processEntry(babelwires::UserLogger& userLogger, const seqwires::TrackFeature& input,
                                               seqwires::TrackFeature& output) const {
    auto trackOut = quantize(input.get(), m_beat->get());
    output.set(std::move(trackOut));
}

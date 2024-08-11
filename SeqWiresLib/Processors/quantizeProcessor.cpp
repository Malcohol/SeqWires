/**
 * A processor which quantizes sequence data a number of times.
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
#include <BabelWiresLib/Types/Rational/rationalTypeConstructor.hpp>
#include <BabelWiresLib/Types/Rational/rationalValue.hpp>

#include <Common/Identifiers/registeredIdentifier.hpp>

#include <set>

seqwires::QuantizeProcessorInput::QuantizeProcessorInput()
    : babelwires::ParallelProcessorInputBase(
          {{BW_SHORT_ID("Beat", "Beat", "1651ab49-3313-4cd3-b92d-16742b7f5921"),
            babelwires::TypeRef{babelwires::RationalTypeConstructor::getThisIdentifier(),
                                babelwires::TypeConstructorArguments{
                                    {},
                                    // TODO: Don't want to allow zero, so would like to be able to express an open interval here.
                                    {babelwires::RationalValue(0),
                                     babelwires::RationalValue(std::numeric_limits<babelwires::Rational::ComponentType>::max()),
                                     babelwires::RationalValue(babelwires::Rational(1, 16))}}}}},
          QuantizeProcessor::getCommonArrayId(), seqwires::DefaultTrackType::getThisIdentifier()) {}

seqwires::QuantizeProcessorOutput::QuantizeProcessorOutput()
    : babelwires::ParallelProcessorOutputBase(QuantizeProcessor::getCommonArrayId(),
                                                   seqwires::DefaultTrackType::getThisIdentifier()) {}

seqwires::QuantizeProcessor::QuantizeProcessor(const babelwires::ProjectContext& projectContext)
    : babelwires::ParallelProcessor(projectContext, QuantizeProcessorInput::getThisIdentifier(),
                                         QuantizeProcessorOutput::getThisIdentifier()) {}

babelwires::ShortId seqwires::QuantizeProcessor::getCommonArrayId() {
    return BW_SHORT_ID("Tracks", "Tracks", "e00623bf-c0f0-4fee-b6c4-4f65df896bf3");
}

void seqwires::QuantizeProcessor::processEntry(babelwires::UserLogger& userLogger,
                                             const babelwires::ValueFeature& inputFeature,
                                             const babelwires::ValueFeature& inputEntry,
                                             babelwires::ValueFeature& outputEntry) const {
    QuantizeProcessorInput::ConstInstance input{inputFeature};
    babelwires::ConstInstance<TrackType> entryIn{inputEntry};
    babelwires::Instance<TrackType> entryOut{outputEntry};

    entryOut.set(quantize(entryIn.get(), input.getBeat().get()));
}

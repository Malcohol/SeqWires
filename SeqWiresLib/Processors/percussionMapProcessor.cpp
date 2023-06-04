/**
 * Processor which applies a map to chord events.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <SeqWiresLib/Processors/percussionMapProcessor.hpp>

#include <SeqWiresLib/Functions/percussionMapFunction.hpp>
#include <SeqWiresLib/Percussion/abstractPercussionSet.hpp>
#include <SeqWiresLib/Percussion/builtInPercussionInstruments.hpp>

#include <BabelWiresLib/Features/rootFeature.hpp>
#include <BabelWiresLib/Project/projectContext.hpp>
#include <BabelWiresLib/TypeSystem/typeSystem.hpp>
#include <BabelWiresLib/Types/Enum/addBlankToEnum.hpp>
#include <BabelWiresLib/Types/Map/mapFeature.hpp>

#include <Common/Identifiers/registeredIdentifier.hpp>

namespace {
    struct PercussionTypeMap : babelwires::SimpleValueFeature {
        PercussionTypeMap()
            : babelwires::SimpleValueFeature(seqwires::getPercussionMapType()) {}
    };
} // namespace

seqwires::PercussionMapProcessor::PercussionMapProcessor(const babelwires::ProjectContext& context)
    : babelwires::ParallelProcessor<seqwires::TrackFeature, seqwires::TrackFeature>(context) {
    m_percussionMapFeature = m_inputFeature->addField(
        std::make_unique<PercussionTypeMap>(), BW_SHORT_ID("Map", "Map", "b8cbf8c9-579b-4292-bdef-524b7d1010bc"));
    addArrayFeature(BW_SHORT_ID("Tracks", "Tracks", "fe71b1c6-6604-430b-a731-f40b2692d2cf"));
}

seqwires::PercussionMapProcessor::Factory::Factory()
    : CommonProcessorFactory(
          BW_LONG_ID("PercussionMapProcessor", "Percussion Map", "1ab6fd2b-8176-4516-9d9a-3b2d91a53f42"), 1) {}

void seqwires::PercussionMapProcessor::processEntry(babelwires::UserLogger& userLogger,
                                                    const seqwires::TrackFeature& input,
                                                    seqwires::TrackFeature& output) const {
    const babelwires::ProjectContext& context = babelwires::RootFeature::getProjectContextAt(*m_percussionMapFeature);

    output.set(mapPercussionFunction(context.m_typeSystem, input.get(), m_percussionMapFeature->getValue()->is<babelwires::MapValue>()));
}

/**
 * Processor which applies a map to chord events.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <SeqWiresLib/Processors/percussionMapProcessor.hpp>

#include <SeqWiresLib/Functions/percussionMapFunction.hpp>
#include <SeqWiresLib/Percussion/abstractPercussionType.hpp>
#include <SeqWiresLib/Percussion/builtInPercussionInstruments.hpp>

#include <BabelWiresLib/Features/mapFeature.hpp>
#include <BabelWiresLib/Features/rootFeature.hpp>
#include <BabelWiresLib/Project/projectContext.hpp>
#include <BabelWiresLib/TypeSystem/typeSystem.hpp>

#include <Common/Identifiers/registeredIdentifier.hpp>

namespace {
    struct PercussionTypeMap : babelwires::MapFeature {
        void getAllowedSourceTypeIds(AllowedTypes& allowedTypesOut) const override {
            getAllPercussionTypes(allowedTypesOut);
        }

        void getAllowedTargetTypeIds(AllowedTypes& allowedTypesOut) const override {
            getAllPercussionTypes(allowedTypesOut);
        }

        babelwires::MapData getDefaultMapData() const override {
            return getStandardDefaultMapData(babelwires::MapEntryData::Kind::AllToSame);
        }

        void getAllPercussionTypes(AllowedTypes& allowedTypesOut) const {
            const babelwires::ProjectContext& context = babelwires::RootFeature::getProjectContextAt(*this);
            allowedTypesOut.m_typeIds =
                context.m_typeSystem.getAllSupertypes(seqwires::AbstractPercussionType::getThisIdentifier());
            const auto it = std::find(allowedTypesOut.m_typeIds.begin(), allowedTypesOut.m_typeIds.end(),
                          seqwires::BuiltInPercussionInstruments::getThisIdentifier());
            assert(it != allowedTypesOut.m_typeIds.end());
            allowedTypesOut.m_indexOfDefault = std::distance(allowedTypesOut.m_typeIds.begin(), it);
        }

        /// Cached
        AllowedTypes m_allowedTypes;
    };
} // namespace

seqwires::PercussionMapProcessor::PercussionMapProcessor(const babelwires::ProjectContext& context)
    : babelwires::ParallelProcessor<seqwires::TrackFeature, seqwires::TrackFeature>(context) {
    m_percussionMapFeature = m_inputFeature->addField(
        std::make_unique<PercussionTypeMap>(), REGISTERED_ID("Map", "Map", "b8cbf8c9-579b-4292-bdef-524b7d1010bc"));
    addArrayFeature(REGISTERED_ID("Tracks", "Tracks", "fe71b1c6-6604-430b-a731-f40b2692d2cf"));
}

seqwires::PercussionMapProcessor::Factory::Factory()
    : CommonProcessorFactory(
          REGISTERED_LONGID("PercussionMapProcessor", "Percussion Map", "1ab6fd2b-8176-4516-9d9a-3b2d91a53f42"), 1) {}

void seqwires::PercussionMapProcessor::processEntry(babelwires::UserLogger& userLogger,
                                                    const seqwires::TrackFeature& input,
                                                    seqwires::TrackFeature& output) const {
    const babelwires::ProjectContext& context = babelwires::RootFeature::getProjectContextAt(*m_percussionMapFeature);

    output.set(std::make_unique<Track>(
        mapPercussionFunction(context.m_typeSystem, input.get(), m_percussionMapFeature->get())));
}

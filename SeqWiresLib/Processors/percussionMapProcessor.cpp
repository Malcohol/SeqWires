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

#include <BabelWiresLib/Features/mapFeature.hpp>
#include <BabelWiresLib/Features/rootFeature.hpp>
#include <BabelWiresLib/Project/projectContext.hpp>
#include <BabelWiresLib/TypeSystem/typeSystem.hpp>
#include <BabelWiresLib/Types/Enum/addBlankToEnum.hpp>

#include <Common/Identifiers/registeredIdentifier.hpp>

namespace {
    struct PercussionTypeMap : babelwires::MapFeature {
        void getAllowedSourceTypeRefs(AllowedTypes& allowedTypesOut) const override {
            getAllPercussionTypes(allowedTypesOut);
        }

        void getAllowedTargetTypeRefs(AllowedTypes& allowedTypesOut) const override {
            getAllPercussionTypes(allowedTypesOut);
            for (auto& typeRef : allowedTypesOut.m_typeRefs) {
                typeRef = babelwires::TypeRef(babelwires::AddBlankToEnum::getThisIdentifier(), typeRef);
            }
        }

        babelwires::MapValue getDefaultMapValue() const override {
            return getStandardDefaultMapValue(babelwires::MapEntryData::Kind::All2Sm);
        }

        void getAllPercussionTypes(AllowedTypes& allowedTypesOut) const {
            const babelwires::ProjectContext& context = babelwires::RootFeature::getProjectContextAt(*this);
            auto superTypes =
                context.m_typeSystem.getAllSupertypes(seqwires::AbstractPercussionSet::getThisIdentifier());
            std::for_each(superTypes.begin(), superTypes.end(), [&allowedTypesOut](babelwires::PrimitiveTypeId typeId) {
                allowedTypesOut.m_typeRefs.emplace_back(typeId);
            });

            // Maybe remove the abstract types here.
            const auto it = std::find(allowedTypesOut.m_typeRefs.begin(), allowedTypesOut.m_typeRefs.end(),
                                      seqwires::BuiltInPercussionInstruments::getThisIdentifier());
            assert(it != allowedTypesOut.m_typeRefs.end());
            allowedTypesOut.m_indexOfDefault = std::distance(allowedTypesOut.m_typeRefs.begin(), it);
        }
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

    output.set(mapPercussionFunction(context.m_typeSystem, input.get(), m_percussionMapFeature->get()));
}

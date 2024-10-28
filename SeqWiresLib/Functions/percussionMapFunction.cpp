/**
 * Function which applies maps to chord events.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <SeqWiresLib/Functions/percussionMapFunction.hpp>

#include <SeqWiresLib/Percussion/percussionTypeTag.hpp>
#include <SeqWiresLib/Percussion/builtInPercussionInstruments.hpp>
#include <SeqWiresLib/Types/Track/TrackEvents/percussionEvents.hpp>
#include <SeqWiresLib/Types/Track/TrackEvents/trackEventHolder.hpp>

#include <BabelWiresLib/ValueTree/modelExceptions.hpp>
#include <BabelWiresLib/TypeSystem/typeSystem.hpp>
#include <BabelWiresLib/Types/Enum/addBlankToEnum.hpp>
#include <BabelWiresLib/Types/Map/Helpers/enumSourceMapApplicator.hpp>
#include <BabelWiresLib/Types/Map/Helpers/enumValueAdapters.hpp>
#include <BabelWiresLib/Types/Map/Helpers/unorderedMapApplicator.hpp>
#include <BabelWiresLib/Types/Map/mapTypeConstructor.hpp>
#include <BabelWiresLib/Types/Map/SumOfMaps/sumOfMapsType.hpp>

std::unique_ptr<babelwires::Type>
seqwires::PercussionMapType::constructType(const babelwires::TypeSystem& typeSystem, babelwires::TypeRef newTypeRef,
                                           const std::vector<const babelwires::Type*>& typeArguments,
                                           const std::vector<babelwires::EditableValueHolder>& valueArguments) const {

    std::vector<babelwires::TypeRef> sourceSummands;
    auto percussionTypes = typeSystem.getTaggedPrimitiveTypes(seqwires::percussionTypeTag());
    std::for_each(percussionTypes.begin(), percussionTypes.end(),
                  [&sourceSummands](babelwires::PrimitiveTypeId typeId) { sourceSummands.emplace_back(typeId); });

    const auto it =
        std::find(sourceSummands.begin(), sourceSummands.end(), seqwires::BuiltInPercussionInstruments::getThisIdentifier());
    assert(it != sourceSummands.end());
    unsigned int indexOfDefault = std::distance(sourceSummands.begin(), it);

    std::vector<babelwires::TypeRef> targetSummands;
    targetSummands.reserve(sourceSummands.size());
    for (const auto& s : sourceSummands) {
        targetSummands.emplace_back(babelwires::TypeRef(babelwires::AddBlankToEnum::getThisIdentifier(), s));
    }

    return std::make_unique<babelwires::ConstructedType<babelwires::SumOfMapsType>>(std::move(newTypeRef),
                                                                              std::move(sourceSummands), std::move(targetSummands), indexOfDefault, indexOfDefault);
}

babelwires::TypeRef seqwires::getPercussionMapType() {
    return babelwires::TypeRef(PercussionMapType::getThisIdentifier(), babelwires::TypeConstructorArguments{});
}

seqwires::Track seqwires::mapPercussionFunction(const babelwires::TypeSystem& typeSystem, const Track& trackIn,
                                                const babelwires::MapValue& percussionMapValue) {

    if (!percussionMapValue.isValid(typeSystem)) {
        throw babelwires::ModelException() << "The Percussion Map is not valid.";
    }

    const babelwires::EnumToIdentifierValueAdapter enumToIdentifierAdapter;
    babelwires::UnorderedMapApplicator<babelwires::ShortId, babelwires::ShortId> mapApplicator{
        percussionMapValue, enumToIdentifierAdapter, enumToIdentifierAdapter};

    Track trackOut;
    // If an event is dropped, then we need to carry its time forward for the next event.
    ModelDuration timeFromDroppedEvent;

    for (auto it = trackIn.begin(); it != trackIn.end(); ++it) {
        const TrackEvent::GroupingInfo info = it->getGroupingInfo();
        if (info.m_category == PercussionEvent::s_percussionEventCategory) {
            TrackEventHolder holder(*it);
            PercussionEvent& percussionEvent = static_cast<PercussionEvent&>(*holder);
            babelwires::ShortId newInstrument = mapApplicator[percussionEvent.getInstrument()];
            if (newInstrument != babelwires::AddBlankToEnum::getBlankValue()) {
                percussionEvent.setInstrument(newInstrument);
                percussionEvent.setTimeSinceLastEvent(holder->getTimeSinceLastEvent() + timeFromDroppedEvent);
                timeFromDroppedEvent = 0;
                trackOut.addEvent(holder.release());
            } else {
                timeFromDroppedEvent += it->getTimeSinceLastEvent();
            }
        } else if (timeFromDroppedEvent > 0) {
            TrackEventHolder holder(*it);
            holder->setTimeSinceLastEvent(holder->getTimeSinceLastEvent() + timeFromDroppedEvent);
            timeFromDroppedEvent = 0;
            trackOut.addEvent(holder.release());
        } else {
            trackOut.addEvent(*it);
        }
    }
    trackOut.setDuration(trackIn.getDuration());

    return trackOut;
}

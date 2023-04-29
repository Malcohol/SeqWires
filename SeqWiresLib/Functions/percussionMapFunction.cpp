/**
 * Function which applies maps to chord events.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <SeqWiresLib/Functions/percussionMapFunction.hpp>

#include <SeqWiresLib/Types/Track/TrackEvents/percussionEvents.hpp>
#include <SeqWiresLib/Types/Track/TrackEvents/trackEventHolder.hpp>
#include <SeqWiresLib/Percussion/builtInPercussionInstruments.hpp>

#include <BabelWiresLib/Types/Enum/addBlankToEnum.hpp>
#include <BabelWiresLib/Features/mapFeature.hpp>
#include <BabelWiresLib/Features/modelExceptions.hpp>
#include <BabelWiresLib/Maps/Helpers/enumSourceMapApplicator.hpp>
#include <BabelWiresLib/Maps/Helpers/unorderedMapApplicator.hpp>
#include <BabelWiresLib/Maps/Helpers/enumValueAdapters.hpp>
#include <BabelWiresLib/TypeSystem/typeSystem.hpp>

seqwires::Track seqwires::mapPercussionFunction(const babelwires::TypeSystem& typeSystem, const Track& trackIn,
                                                const babelwires::MapData& percussionMapData) {

    if (!percussionMapData.isValid(typeSystem)) {
        throw babelwires::ModelException() << "The Percussion Map is not valid.";
    }

    const babelwires::EnumToIdentifierValueAdapter enumToIdentifierAdapter;
    babelwires::UnorderedMapApplicator<babelwires::ShortId, babelwires::ShortId> mapApplicator{ percussionMapData, enumToIdentifierAdapter, enumToIdentifierAdapter };

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

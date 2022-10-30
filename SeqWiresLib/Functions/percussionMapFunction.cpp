/**
 * Function which applies maps to chord events.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <SeqWiresLib/Functions/percussionMapFunction.hpp>

#include <SeqWiresLib/Tracks/noteEvents.hpp>
#include <SeqWiresLib/Tracks/trackEventHolder.hpp>
#include <SeqWiresLib/percussion.hpp>

#include <BabelWiresLib/Features/mapFeature.hpp>
#include <BabelWiresLib/Features/modelExceptions.hpp>
#include <BabelWiresLib/Maps/Helpers/enumSourceMapApplicator.hpp>
#include <BabelWiresLib/Maps/Helpers/enumValueAdapters.hpp>
#include <BabelWiresLib/TypeSystem/typeSystem.hpp>

namespace {
    using MapType = babelwires::EnumSourceMapApplicator<seqwires::GM2StandardPercussionKit, seqwires::GM2StandardPercussionKit::Value>;

    bool mapPercussionEvent(const seqwires::GM2StandardPercussionKit& percussionType, MapType& mapApplicator,
                            seqwires::NoteEvent& noteEvent) {
        const seqwires::Pitch originalPitch = noteEvent.getPitch();
        seqwires::GM2StandardPercussionKit::Value originalValue;
        if (percussionType.tryGetValueFromPitch(originalPitch, originalValue)) {
            const seqwires::GM2StandardPercussionKit::Value newValue = mapApplicator[originalValue];
            const seqwires::Pitch newPitch = seqwires::GM2StandardPercussionKit::getPitchFromValue(newValue);
            noteEvent.setPitch(newPitch);
            return true;
        }
        // Not a known percussion event.
        // TODO: Probably would be better to use default here.
        return false;
    }
} // namespace

seqwires::Track seqwires::mapPercussionFunction(const babelwires::TypeSystem& typeSystem, const Track& trackIn,
                                                const babelwires::MapData& percussionMapData) {

    if (!percussionMapData.isValid(typeSystem)) {
        throw babelwires::ModelException() << "The Percussion Map is not valid.";
    }
    const GM2StandardPercussionKit& percussionType =
        typeSystem.getEntryByIdentifier(seqwires::GM2StandardPercussionKit::getThisIdentifier())->is<GM2StandardPercussionKit>();

    const babelwires::EnumToValueValueAdapter<GM2StandardPercussionKit> targetAdapter{percussionType};
    MapType mapApplicator(percussionMapData, percussionType, targetAdapter);

    Track trackOut;
    // If an event is dropped, then we need to carry its time forward for the next event.
    ModelDuration timeFromDroppedEvent;

    for (auto it = trackIn.begin(); it != trackIn.end(); ++it) {
        const TrackEvent::GroupingInfo info = it->getGroupingInfo();
        if (info.m_category == NoteEvent::s_noteEventCategory) {
            TrackEventHolder holder(*it);
            if (mapPercussionEvent(percussionType, mapApplicator, static_cast<NoteEvent&>(*holder))) {
                holder->setTimeSinceLastEvent(holder->getTimeSinceLastEvent() + timeFromDroppedEvent);
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

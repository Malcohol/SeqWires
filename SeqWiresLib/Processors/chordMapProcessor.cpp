/**
 * Processor which applies a map to chord events.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <SeqWiresLib/Processors/chordMapProcessor.hpp>

#include <SeqWiresLib/Tracks/chordEvents.hpp>
#include <SeqWiresLib/Tracks/trackEventHolder.hpp>
#include <SeqWiresLib/chord.hpp>

#include <BabelWiresLib/Features/mapFeature.hpp>
#include <BabelWiresLib/Features/rootFeature.hpp>
#include <BabelWiresLib/Maps/Helpers/enumValueAdapters.hpp>
#include <BabelWiresLib/Maps/Helpers/enumSourceMapApplicator.hpp>
#include <BabelWiresLib/Project/projectContext.hpp>
#include <BabelWiresLib/TypeSystem/typeSystem.hpp>

#include <Common/Identifiers/registeredIdentifier.hpp>

namespace {
    struct ChordTypeMap : babelwires::MapFeature {
        ChordTypeMap()
            : babelwires::MapFeature(seqwires::ChordType::getThisIdentifier(), seqwires::ChordType::getThisIdentifier()) {}
    };
} // namespace

seqwires::ChordMapProcessor::ChordMapProcessor(const babelwires::ProjectContext& context)
    : babelwires::ParallelProcessor<seqwires::TrackFeature, seqwires::TrackFeature>(context) {
    m_chordTypeMapFeature = m_inputFeature->addField(
        std::make_unique<ChordTypeMap>(), REGISTERED_ID("TypMap", "Type map", "6054b8e9-5f48-4e9f-8807-b6377d36d6aa"));
    addArrayFeature(REGISTERED_ID("Tracks", "Tracks", "24e56b0d-eb1e-4c93-97fd-ba4d639e112a"));
}

seqwires::ChordMapProcessor::Factory::Factory()
    : CommonProcessorFactory(
          REGISTERED_LONGID("ChordMapProcessor", "Chord Map", "b7227130-8274-4451-bd60-8fe34a74c4b6"), 1) {}

void seqwires::ChordMapProcessor::processEntry(babelwires::UserLogger& userLogger, const seqwires::TrackFeature& input,
                                               seqwires::TrackFeature& output) const {
    const babelwires::MapData& mapData = m_chordTypeMapFeature->get();
    const babelwires::ProjectContext& context = babelwires::RootFeature::getProjectContextAt(*m_chordTypeMapFeature);
    if (!mapData.isValid(context.m_typeSystem)) {
        throw babelwires::ModelException() << "The Map is not valid.";
    }

    const ChordType& chordType =
        context.m_typeSystem.getEntryByIdentifier(seqwires::ChordType::getThisIdentifier())->is<ChordType>();

    const babelwires::EnumToValueValueAdapter<ChordType> targetAdapter{chordType};

    babelwires::EnumSourceMapApplicator<ChordType, ChordType::Value> applicator(mapData, chordType, targetAdapter);

    const Track& trackIn = input.get();
    Track trackOut;

    for (auto it = trackIn.begin(); it != trackIn.end(); ++it) {
        if (it->as<ChordOnEvent>()) {
            TrackEventHolder holder(*it);
            ChordType::Value& type = holder->is<ChordOnEvent>().m_chord.m_chordType;
            type = applicator[type];
            trackOut.addEvent(holder.release());
        } else {
            trackOut.addEvent(*it);
        }
    }
    trackOut.setDuration(trackIn.getDuration());
}

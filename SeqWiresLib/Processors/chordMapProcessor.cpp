/**
 * Processor which applies a map to chord events.
 * 
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include "SeqWiresLib/Processors/chordMapProcessor.hpp"

#include "BabelWiresLib/Features/mapFeature.hpp"

#include "Common/Identifiers/registeredIdentifier.hpp"

namespace {
    using ChordTypeMap = babelwires::MapFeature;
}

seqwires::ChordMapProcessor::ChordMapProcessor() {
    m_chordTypeMapFeature = m_inputFeature->addField(std::make_unique<ChordTypeMap>(),
                                          REGISTERED_ID("TypMap", "Type map", "6054b8e9-5f48-4e9f-8807-b6377d36d6aa"));
    addArrayFeature(REGISTERED_ID("Tracks", "Tracks", "24e56b0d-eb1e-4c93-97fd-ba4d639e112a"));
}

seqwires::ChordMapProcessor::Factory::Factory()
    : CommonProcessorFactory(REGISTERED_LONGID("ChordMapProcessor", "Chord Map", "b7227130-8274-4451-bd60-8fe34a74c4b6"), 1) {}

void seqwires::ChordMapProcessor::processEntry(babelwires::UserLogger& userLogger, const seqwires::TrackFeature& input, seqwires::TrackFeature& output) const {
    
}


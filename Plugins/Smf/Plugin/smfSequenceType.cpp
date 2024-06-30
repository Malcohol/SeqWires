/**
 * Representation of a Standard MIDI File as a tree of Features.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <Plugins/Smf/Plugin/smfSequenceType.hpp>

#include <Common/Identifiers/registeredIdentifier.hpp>

namespace {
    const babelwires::IdentifiersSource formatIdentifiersSource = {
        {"SMF0", "0", "82835685-a66e-417e-b076-5d706d03a989"}, {"SMF1", "1", "ce16e042-445b-423d-a493-874ed4c238b3"}};
}

smf::SmfType::SmfType()
    : babelwires::RecordWithVariantsType(REGISTERED_ID_VECTOR(formatIdentifiersSource),
                                         {{BW_SHORT_ID("Meta", "Metadata", "72bbbcee-2b53-4fb2-bfb8-4f5e495f9166"),
                                           MidiMetadata::getThisIdentifier(),
                                           {}},
                                          {BW_SHORT_ID("Tracks", "Tracks", "38ae4e20-1468-4dce-890b-981454e6dbe0"),
                                           MidiTrackAndChannelArray::getThisIdentifier(),
                                           {}}}) {}
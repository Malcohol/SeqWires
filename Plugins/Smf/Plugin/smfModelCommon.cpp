/**
 * Representation of a Standard MIDI File as a tree of Features.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <Plugins/Smf/Plugin/smfModelCommon.hpp>

#include <Plugins/Smf/Plugin/smfFormat.hpp>

#include <SeqWiresLib/Types/Track/trackFeature.hpp>
#include <SeqWiresLib/Types/tempo.hpp>

#include <BabelWiresLib/TypeSystem/typeSystem.hpp>
#include <BabelWiresLib/Types/Enum/enumValue.hpp>
#include <BabelWiresLib/Types/Enum/enumWithCppEnum.hpp>
#include <BabelWiresLib/Types/Int/intFeature.hpp>
#include <BabelWiresLib/Types/String/stringType.hpp>
#include <BabelWiresLib/Types/String/stringValue.hpp>

#include <Common/Identifiers/registeredIdentifier.hpp>

namespace {
    const char specMetadataId[] = "Spec";
    const char tempoMetadataId[] = "Tempo";
    const char nameMetadataId[] = "Name";
    const char copyrightMetadataId[] = "CopyR";
} // namespace

smf::MidiMetadata::MidiMetadata()
    : babelwires::RecordType(
          {{BW_SHORT_ID(specMetadataId, "MIDI Spec", "15a9fa85-f2c6-4e68-8691-fefd64ca1233"),
            GMSpecType::getThisIdentifier()},
           {BW_SHORT_ID(tempoMetadataId, "Tempo", "3ef804e9-e34a-4a25-b6bf-ce7597d9d90b"),
            seqwires::Tempo::getThisIdentifier(), babelwires::RecordType::Optionality::optionalDefaultInactive},
           {BW_SHORT_ID(nameMetadataId, "Name", "c2e4910f-d006-4a93-97a7-ae5973157ec8"),
            babelwires::StringType::getThisIdentifier(), babelwires::RecordType::Optionality::optionalDefaultInactive},
           {BW_SHORT_ID(copyrightMetadataId, "Copyright", "a59dc914-d060-4f03-be83-5804fc4d6b6a"),
            babelwires::StringType::getThisIdentifier(),
            babelwires::RecordType::Optionality::optionalDefaultInactive}}) {}

smf::MidiMetadataFeature::MidiMetadataFeature()
    : babelwires::SimpleValueFeature(MidiMetadata::getThisIdentifier()) {}

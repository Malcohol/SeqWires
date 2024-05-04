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

const babelwires::ValueFeature& smf::MidiMetadata::getChild(const babelwires::ValueFeature& recordFeature,
                                                            babelwires::ShortId id) {
    const int index = recordFeature.getChildIndexFromStep(babelwires::PathStep(id));
    assert(index >= 0);
    return recordFeature.getFeature(index)->is<babelwires::ValueFeature>();
}

babelwires::ValueFeature& smf::MidiMetadata::getChild(babelwires::ValueFeature& recordFeature, babelwires::ShortId id) {
    const int index = recordFeature.getChildIndexFromStep(babelwires::PathStep(id));
    assert(index >= 0);
    return recordFeature.getFeature(index)->is<babelwires::ValueFeature>();
}

const babelwires::ValueFeature* smf::MidiMetadata::tryGetChild(const babelwires::ValueFeature& recordFeature,
                                                               babelwires::ShortId id) {
    const int index = recordFeature.getChildIndexFromStep(babelwires::PathStep(id));
    if (index >= 0) {
        return &recordFeature.getFeature(index)->is<babelwires::ValueFeature>();
    } else {
        return nullptr;
    }
}

babelwires::ValueFeature& smf::MidiMetadata::activateAndGetChild(babelwires::ValueFeature& recordFeature,
                                                                 babelwires::ShortId id) {
    const babelwires::TypeSystem& typeSystem = babelwires::RootFeature::getTypeSystemAt(recordFeature);
    const auto& recordType = typeSystem.getEntryByType<MidiMetadata>();
    babelwires::ValueHolder recordValue = recordFeature.getValue();
    if (recordType.isActivated(recordValue, id)) {
        recordType.activateField(typeSystem, recordValue, id);
        recordFeature.setValue(recordValue);
    }
    return getChild(recordFeature, id);
}

smf::GMSpecType::Value smf::MidiMetadata::getSpecValue(const babelwires::ValueFeature& midiMetadataFeature) {
    const auto& childFeature = getChild(midiMetadataFeature, specMetadataId);
    const auto& enumFeature = childFeature.is<babelwires::EnumWithCppEnumFeature<GMSpecType>>();
    return enumFeature.getAsValue();
}

void smf::MidiMetadata::setSpecValue(babelwires::ValueFeature& midiMetadataFeature, GMSpecType::Value newSpec) {
    auto& childFeature = getChild(midiMetadataFeature, specMetadataId);
    auto& enumFeature = childFeature.is<babelwires::EnumWithCppEnumFeature<GMSpecType>>();
    enumFeature.setFromValue(newSpec);
}

std::optional<int> smf::MidiMetadata::tryGetTempo(const babelwires::ValueFeature& midiMetadataFeature) {
    if (const auto* childFeature = tryGetChild(midiMetadataFeature, specMetadataId)) {
        const auto& intFeature = childFeature->is<babelwires::IntFeature>();
        return intFeature.get();
    }
    return {};
}

void smf::MidiMetadata::setTempo(babelwires::ValueFeature& midiMetadataFeature, int newTempo) {
    auto& childFeature = activateAndGetChild(midiMetadataFeature, specMetadataId);
    auto& intFeature = childFeature.is<babelwires::IntFeature>();
    intFeature.set(newTempo);
}

std::optional<std::string> smf::MidiMetadata::tryGetCopyright(const babelwires::ValueFeature& midiMetadataFeature) {
    if (const auto* childFeature = tryGetChild(midiMetadataFeature, copyrightMetadataId)) {
        const auto& stringFeature = childFeature->is<babelwires::StringFeature>();
        return stringFeature.get();
    }
    return {};
}

void smf::MidiMetadata::setCopyright(babelwires::ValueFeature& midiMetadataFeature, std::string newCopyright) {
    auto& childFeature = activateAndGetChild(midiMetadataFeature, copyrightMetadataId);
    auto& intFeature = childFeature.is<babelwires::StringFeature>();
    intFeature.set(newCopyright);
}

std::optional<std::string> smf::MidiMetadata::tryGetSequenceName(const babelwires::ValueFeature& midiMetadataFeature) {
    if (const auto* childFeature = tryGetChild(midiMetadataFeature, nameMetadataId)) {
        const auto& stringFeature = childFeature->is<babelwires::StringFeature>();
        return stringFeature.get();
    }
    return {};
}

void smf::MidiMetadata::setSequenceName(babelwires::ValueFeature& midiMetadataFeature, std::string newSequenceName) {
    auto& childFeature = activateAndGetChild(midiMetadataFeature, nameMetadataId);
    auto& intFeature = childFeature.is<babelwires::StringFeature>();
    intFeature.set(newSequenceName);
}

smf::MidiMetadataFeature::MidiMetadataFeature()
    : babelwires::SimpleValueFeature(MidiMetadata::getThisIdentifier()) {}

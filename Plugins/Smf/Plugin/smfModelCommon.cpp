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

#include <BabelWiresLib/Types/String/stringType.hpp>
#include <BabelWiresLib/Types/Enum/enumValue.hpp>

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

smf::GMSpecType::Value smf::MidiMetadata::getSpec(const babelwires::TypeSystem& typeSystem, const babelwires::ValueHolder& spec) const {
    const int index = getChildIndexFromStep(spec, babelwires::PathStep(specMetadataId));
    assert(index >= 0);
    const auto& [childValue, _, childTypeRef] = getChild(spec, index);
    const GMSpecType& childType = childTypeRef.resolve(typeSystem).is<GMSpecType>();
    const babelwires::EnumValue& enumValue = (*childValue)->is<babelwires::EnumValue>();
    const babelwires::ShortId enumId = enumValue.get();
    return childType.getValueFromIdentifier(enumId);
}

void smf::MidiMetadata::setSpec(const babelwires::TypeSystem& typeSystem, babelwires::ValueHolder& value, GMSpecType::Value newSpec) const {
    if (!isActivated(value, specMetadataId)) {
        activateField(typeSystem, value, specMetadataId);
    }
    const int index = getChildIndexFromStep(value, babelwires::PathStep(specMetadataId));
    assert(index >= 0);
    auto childInfo = getChildNonConst(value, index);
    babelwires::ValueHolder& newValue = *std::get<0>(childInfo);
    babelwires::EnumValue& enumValue = newValue.copyContentsAndGetNonConst().is<babelwires::EnumValue>();
    const GMSpecType& childType = std::get<2>(childInfo).resolve(typeSystem).is<GMSpecType>();
    const babelwires::ShortId newEnumId = childType.getIdentifierFromValue(newSpec);
    enumValue.set(newEnumId);
}


smf::MidiMetadataFeature::MidiMetadataFeature() {
    m_specFeature = addField(std::make_unique<GmSpecTypeFeature>(),
                             BW_SHORT_ID("Spec", "MIDI Spec", "15a9fa85-f2c6-4e68-8691-fefd64ca1233"));
    m_tempo = addOptionalField(std::make_unique<seqwires::TempoFeature>(),
                               BW_SHORT_ID(tempoMetadataId, "Tempo", "3ef804e9-e34a-4a25-b6bf-ce7597d9d90b"));
    m_sequenceName = addOptionalField(std::make_unique<babelwires::StringFeature>(),
                                      BW_SHORT_ID(nameMetadataId, "Name", "c2e4910f-d006-4a93-97a7-ae5973157ec8"));
    m_copyright =
        addOptionalField(std::make_unique<babelwires::StringFeature>(),
                         BW_SHORT_ID(copyrightMetadataId, "Copyright", "a59dc914-d060-4f03-be83-5804fc4d6b6a"));
}

const smf::GmSpecTypeFeature* smf::MidiMetadataFeature::getSpecFeature() const {
    return m_specFeature;
}

smf::GmSpecTypeFeature* smf::MidiMetadataFeature::getSpecFeature() {
    return m_specFeature;
}

seqwires::TempoFeature& smf::MidiMetadataFeature::getActivatedTempoFeature() {
    if (!isActivated(tempoMetadataId)) {
        activateField(tempoMetadataId);
    }
    return *m_tempo;
}

const babelwires::StringFeature* smf::MidiMetadataFeature::getCopyright() const {
    return isActivated(copyrightMetadataId) ? m_copyright : nullptr;
}

const babelwires::StringFeature* smf::MidiMetadataFeature::getSequenceName() const {
    return isActivated(nameMetadataId) ? m_sequenceName : nullptr;
}

const seqwires::TempoFeature* smf::MidiMetadataFeature::getTempoFeature() const {
    return isActivated(tempoMetadataId) ? m_tempo : nullptr;
}

babelwires::StringFeature& smf::MidiMetadataFeature::getActivatedCopyright() {
    if (!isActivated(copyrightMetadataId)) {
        activateField(copyrightMetadataId);
    }
    return *m_copyright;
}

babelwires::StringFeature& smf::MidiMetadataFeature::getActivatedSequenceName() {
    if (!isActivated(nameMetadataId)) {
        activateField(nameMetadataId);
    }
    return *m_sequenceName;
}

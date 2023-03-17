/**
 * Representation of a Standard MIDI File as a tree of Features.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <Plugins/Smf/Plugin/smfModelCommon.hpp>

#include <Plugins/Smf/Plugin/smfFormat.hpp>

#include <BabelWiresLib/Features/featureMixins.hpp>

#include <SeqWiresLib/Features/trackFeature.hpp>

#include <Common/Identifiers/registeredIdentifier.hpp>


namespace {
    const char tempoMetadataId[] = "Tempo";
    const char nameMetadataId[] = "Name";
    const char copyrightMetadataId[] = "CopyR";
}

smf::MidiMetadata::MidiMetadata() {
    m_specFeature = addField(std::make_unique<GmSpecTypeFeature>(), BW_SHORT_ID("Spec", "MIDI Spec", "15a9fa85-f2c6-4e68-8691-fefd64ca1233"));
    m_tempo = addOptionalField(std::make_unique<seqwires::TempoFeature>(),
                       BW_SHORT_ID(tempoMetadataId, "Tempo", "3ef804e9-e34a-4a25-b6bf-ce7597d9d90b"));
    m_sequenceName = addOptionalField(std::make_unique<babelwires::StringFeature>(),
                              BW_SHORT_ID(nameMetadataId, "Name", "c2e4910f-d006-4a93-97a7-ae5973157ec8"));
    m_copyright = addOptionalField(std::make_unique<babelwires::StringFeature>(),
                           BW_SHORT_ID(copyrightMetadataId, "Copyright", "a59dc914-d060-4f03-be83-5804fc4d6b6a"));
}

const smf::GmSpecTypeFeature* smf::MidiMetadata::getSpecFeature() const {
    return m_specFeature;
}

smf::GmSpecTypeFeature* smf::MidiMetadata::getSpecFeature() {
    return m_specFeature;
}

seqwires::TempoFeature& smf::MidiMetadata::getActivatedTempoFeature() {
    if (!isActivated(tempoMetadataId)) {
        activateField(tempoMetadataId);
    }
    return *m_tempo;
}

const babelwires::StringFeature* smf::MidiMetadata::getCopyright() const {
    return isActivated(copyrightMetadataId) ? m_copyright : nullptr;
}

const babelwires::StringFeature* smf::MidiMetadata::getSequenceName() const {
    return isActivated(nameMetadataId) ? m_sequenceName : nullptr;
}

const seqwires::TempoFeature* smf::MidiMetadata::getTempoFeature() const {
    return isActivated(tempoMetadataId) ? m_tempo : nullptr;
}

babelwires::StringFeature& smf::MidiMetadata::getActivatedCopyright() {
    if (!isActivated(copyrightMetadataId)) {
        activateField(copyrightMetadataId);
    }
    return *m_copyright;
}

babelwires::StringFeature& smf::MidiMetadata::getActivatedSequenceName() {
    if (!isActivated(nameMetadataId)) {
        activateField(nameMetadataId);
    }
    return *m_sequenceName;
}

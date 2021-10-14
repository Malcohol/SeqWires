/**
 * Representation of a Standard MIDI File as a tree of Features.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include "BabelWiresPlugins/Smf/Plugin/smfModelCommon.hpp"

#include "BabelWiresLib/Features/featureMixins.hpp"
#include "BabelWiresPlugins/Smf/Plugin/smfFormat.hpp"
#include "SeqWiresLib/Features/trackFeature.hpp"

#include "BabelWiresLib/Identifiers/declareIdentifier.hpp"


namespace {
    const char tempoMetadataId[] = "Tempo";
    const char nameMetadataId[] = "Name";
    const char copyrightMetadataId[] = "CopyR";
}

smf::MidiMetadata::MidiMetadata() {
    m_tempo = addOptionalField(std::make_unique<seqwires::TempoFeature>(),
                       FIELD_NAME(tempoMetadataId, "Tempo", "3ef804e9-e34a-4a25-b6bf-ce7597d9d90b"));
    m_sequenceName = addOptionalField(std::make_unique<babelwires::StringFeature>(),
                              FIELD_NAME(nameMetadataId, "Name", "c2e4910f-d006-4a93-97a7-ae5973157ec8"));
    m_copyright = addOptionalField(std::make_unique<babelwires::StringFeature>(),
                           FIELD_NAME(copyrightMetadataId, "Copyright", "a59dc914-d060-4f03-be83-5804fc4d6b6a"));
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

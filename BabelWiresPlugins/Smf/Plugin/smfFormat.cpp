/**
 * Formats which support Standard MIDI Files.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <BabelWiresPlugins/Smf/Plugin/smfFormat.hpp>

#include <BabelWiresPlugins/Smf/Plugin/smfParser.hpp>
#include <BabelWiresPlugins/Smf/Plugin/smfWriter.hpp>

namespace {

    const char s_formatIdentifier[] = "StandardMIDIFile";
    const char s_manufacturerName[] = "MIDI Manufacturers Association";
    const char s_productName[] = "Standard MIDI File";

} // namespace

smf::SmfSourceFormat::SmfSourceFormat()
    : SourceFileFormat(
          REGISTERED_LONGID(s_formatIdentifier, "Standard MIDI file (in)", "418b8238-c184-4885-a369-b24c4e0d06ec"), 1,
          Extensions{"mid", "smf"}) {}

babelwires::LongIdentifier smf::SmfSourceFormat::getThisIdentifier() {
    return s_formatIdentifier;
}

std::string smf::SmfSourceFormat::getManufacturerName() const {
    return s_manufacturerName;
}

std::string smf::SmfSourceFormat::getProductName() const {
    return s_productName;
}

std::unique_ptr<babelwires::FileFeature>
smf::SmfSourceFormat::loadFromFile(babelwires::DataSource& dataSource, const babelwires::ProjectContext& projectContext,
                                   babelwires::UserLogger& userLogger) const {
    return parseSmfSequence(dataSource, projectContext, userLogger);
}

smf::SmfTargetFormat::SmfTargetFormat()
    : TargetFileFormat(REGISTERED_LONGID("SmfFile", "Standard MIDI file (out)", "f29cd3b0-8a46-4a21-bb7d-53acd6702944"),
                       1, Extensions{"mid", "smf"}) {}

std::string smf::SmfTargetFormat::getManufacturerName() const {
    return s_manufacturerName;
}

std::string smf::SmfTargetFormat::getProductName() const {
    return s_productName;
}

std::unique_ptr<babelwires::FileFeature>
smf::SmfTargetFormat::createNewFeature(const babelwires::ProjectContext& projectContext) const {
    return std::make_unique<target::SmfFeature>(projectContext);
}

void smf::SmfTargetFormat::writeToFile(const babelwires::ProjectContext& projectContext,
                                       babelwires::UserLogger& userLogger, const babelwires::FileFeature& sequence,
                                       std::ostream& os) const {
    writeToSmf(os, *sequence.as<target::SmfFeature>());
}

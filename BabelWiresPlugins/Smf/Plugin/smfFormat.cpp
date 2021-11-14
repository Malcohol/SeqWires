/**
 * Formats which support Standard MIDI Files.
 * 
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include "BabelWiresPlugins/Smf/Plugin/smfFormat.hpp"
#include "BabelWiresPlugins/Smf/Plugin/smfParser.hpp"
#include "BabelWiresPlugins/Smf/Plugin/smfWriter.hpp"

namespace {

    const char s_formatIdentifier[] = "StandardMIDIFile";
    const char s_manufacturerName[] = "MIDI Manufacturers Association";
    const char s_productName[] = "Standard MIDI File";

} // namespace

smf::SmfSourceFormat::SmfSourceFormat()
    : SourceFileFormat(REGISTERED_LONGID(s_formatIdentifier, "Standard MIDI file", "418b8238-c184-4885-a369-b24c4e0d06ec"), 1, Extensions{"mid", "smf"}) {}

babelwires::LongIdentifier smf::SmfSourceFormat::getThisIdentifier() {
    return s_formatIdentifier;
}

std::string smf::SmfSourceFormat::getManufacturerName() const {
    return s_manufacturerName;
}

std::string smf::SmfSourceFormat::getProductName() const {
    return s_productName;
}

std::unique_ptr<babelwires::FileFeature> smf::SmfSourceFormat::loadFromFile(babelwires::DataSource& dataSource,
                                                                      babelwires::UserLogger& userLogger) const {
    return parseSmfSequence(dataSource, userLogger);
}

smf::SmfFormat0TargetFormat::SmfFormat0TargetFormat()
    : TargetFileFormat(REGISTERED_LONGID("Format0SmfFile", "Standard MIDI file (Format 0)", "f29cd3b0-8a46-4a21-bb7d-53acd6702944"), 1, Extensions{"mid", "smf"}) {}

std::string smf::SmfFormat0TargetFormat::getManufacturerName() const {
    return s_manufacturerName;
}

std::string smf::SmfFormat0TargetFormat::getProductName() const {
    return s_productName;
}

std::unique_ptr<babelwires::FileFeature> smf::SmfFormat0TargetFormat::createNewFeature() const {
    return std::make_unique<target::Format0SmfFeature>();
}

void smf::SmfFormat0TargetFormat::writeToFile(const babelwires::FileFeature& sequence, std::ostream& os,
                                 babelwires::UserLogger& userLogger) const {
    writeToSmfFormat0(os, *sequence.as<target::Format0SmfFeature>());
}

smf::SmfFormat1TargetFormat::SmfFormat1TargetFormat()
    : TargetFileFormat(REGISTERED_LONGID("Format1SmfFile", "Standard MIDI file (Format 1)", "ca3e9832-61f9-4ef7-8c11-509d3f76e7e9"), 1, Extensions{"mid", "smf"}) {}

std::string smf::SmfFormat1TargetFormat::getManufacturerName() const {
    return s_manufacturerName;
}

std::string smf::SmfFormat1TargetFormat::getProductName() const {
    return s_productName;
}

std::unique_ptr<babelwires::FileFeature> smf::SmfFormat1TargetFormat::createNewFeature() const {
    return std::make_unique<target::Format1SmfFeature>();
}

void smf::SmfFormat1TargetFormat::writeToFile(const babelwires::FileFeature& sequence, std::ostream& os,
                                 babelwires::UserLogger& userLogger) const {
    writeToSmfFormat1(os, *sequence.as<target::Format1SmfFeature>());
}

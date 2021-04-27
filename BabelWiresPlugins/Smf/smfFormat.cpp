/**
 * Formats which support Standard MIDI Files.
 * 
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include "BabelWiresPlugins/Smf/smfFormat.hpp"
#include "BabelWiresPlugins/Smf/smfParser.hpp"
#include "BabelWiresPlugins/Smf/smfWriter.hpp"

namespace {

    const char s_formatIdentifier[] = "StandardMIDIFile";
    const char s_manufacturerName[] = "MIDI Manufacturers Association";
    const char s_productName[] = "Standard MIDI File";

} // namespace

smf::SmfFormat::SmfFormat()
    : SourceFileFormat(s_formatIdentifier, "Standard MIDI file", 1, Extensions{"mid", "smf"}) {}

std::string smf::SmfFormat::getThisIdentifier() {
    return s_formatIdentifier;
}

std::string smf::SmfFormat::getManufacturerName() const {
    return s_manufacturerName;
}

std::string smf::SmfFormat::getProductName() const {
    return s_productName;
}

std::unique_ptr<babelwires::FileFeature> smf::SmfFormat::loadFromFile(babelwires::DataSource& dataSource,
                                                                      babelwires::UserLogger& userLogger) const {
    return parseSmfSequence(dataSource);
}

smf::Format0FactoryFormat::Format0FactoryFormat()
    : TargetFileFactory("Format0SmfFile", "Format 0 SMF file", 1, Extensions{"mid", "smf"}) {}

std::string smf::Format0FactoryFormat::getManufacturerName() const {
    return s_manufacturerName;
}

std::string smf::Format0FactoryFormat::getProductName() const {
    return s_productName;
}

std::unique_ptr<babelwires::FileFeature> smf::Format0FactoryFormat::createNewFeature() const {
    return std::make_unique<Format0Sequence>();
}

void smf::Format0FactoryFormat::writeToFile(const babelwires::FileFeature& sequence, std::ostream& os,
                                 babelwires::UserLogger& userLogger) const {
    writeToSmf(os, dynamic_cast<const SmfSequence&>(sequence));
}

smf::Format1FactoryFormat::Format1FactoryFormat()
    : TargetFileFactory("Format1SmfFile", "Format 1 SMF file", 1, Extensions{"mid", "smf"}) {}

std::string smf::Format1FactoryFormat::getManufacturerName() const {
    return s_manufacturerName;
}

std::string smf::Format1FactoryFormat::getProductName() const {
    return s_productName;
}

std::unique_ptr<babelwires::FileFeature> smf::Format1FactoryFormat::createNewFeature() const {
    return std::make_unique<Format1Sequence>();
}

void smf::Format1FactoryFormat::writeToFile(const babelwires::FileFeature& sequence, std::ostream& os,
                                 babelwires::UserLogger& userLogger) const {
    writeToSmf(os, dynamic_cast<const SmfSequence&>(sequence));
}

/**
 * Formats which support Standard MIDI Files.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <Plugins/Smf/Plugin/smfFormat.hpp>

#include <BabelWiresLib/Project/projectContext.hpp>
#include <BabelWiresLib/Types/File/fileTypeT.hpp>

#include <Plugins/Smf/Plugin/smfParser.hpp>
#include <Plugins/Smf/Plugin/smfWriter.hpp>

#include <Common/IO/fileDataSource.hpp>
#include <Common/IO/outFileStream.hpp>

namespace {

    const char s_formatIdentifier[] = "StandardMIDIFile";
    const char s_manufacturerName[] = "MIDI Manufacturers Association";
    const char s_productName[] = "Standard MIDI File";

} // namespace

smf::SmfSourceFormat::SmfSourceFormat()
    : SourceFileFormat(
          BW_LONG_ID(s_formatIdentifier, "Standard MIDI file (in)", "418b8238-c184-4885-a369-b24c4e0d06ec"), 1,
          Extensions{"mid", "smf"}) {}

babelwires::LongId smf::SmfSourceFormat::getThisIdentifier() {
    return s_formatIdentifier;
}

std::string smf::SmfSourceFormat::getManufacturerName() const {
    return s_manufacturerName;
}

std::string smf::SmfSourceFormat::getProductName() const {
    return s_productName;
}

std::unique_ptr<babelwires::ValueTreeRoot>
smf::SmfSourceFormat::loadFromFile(const std::filesystem::path& path, const babelwires::ProjectContext& projectContext,
                                   babelwires::UserLogger& userLogger) const {
    babelwires::FileDataSource dataSource(path);
    return parseSmfSequence(dataSource, projectContext, userLogger);
}

smf::SmfTargetFormat::SmfTargetFormat()
    : TargetFileFormat(BW_LONG_ID("SmfFile", "Standard MIDI file (out)", "f29cd3b0-8a46-4a21-bb7d-53acd6702944"), 1,
                       Extensions{"mid", "smf"}) {}

std::string smf::SmfTargetFormat::getManufacturerName() const {
    return s_manufacturerName;
}

std::string smf::SmfTargetFormat::getProductName() const {
    return s_productName;
}

std::unique_ptr<babelwires::ValueTreeRoot>
smf::SmfTargetFormat::createNewValue(const babelwires::ProjectContext& projectContext) const {
    return std::make_unique<babelwires::ValueTreeRoot>(projectContext.m_typeSystem,
                                                       babelwires::FileTypeT<SmfSequence>::getThisType());
}

void smf::SmfTargetFormat::writeToFile(const babelwires::ProjectContext& projectContext,
                                       babelwires::UserLogger& userLogger, const babelwires::ValueTreeRoot& contents,
                                       const std::filesystem::path& path) const {
    babelwires::OutFileStream outStream(path);
    writeToSmf(projectContext, userLogger, contents, outStream);
    outStream.close();
}

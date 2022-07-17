/**
 * A TapeFileFormat knows how to convert certain types of data between a TapeFile::DataFile and audio.
 * 
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <Seq2tapeLib/tapeFileFormat.hpp>

seq2tape::TapeFileFormat::TapeFileFormat(babelwires::LongIdentifier identifier, babelwires::VersionNumber version,
                                         Extensions extensions)
    : babelwires::FileTypeEntry(identifier, 1, std::move(extensions)) {}

seq2tape::TapeFileFormatRegistry::TapeFileFormatRegistry()
    : babelwires::FileTypeRegistry<TapeFileFormat>("Tape File Format Registry") {}

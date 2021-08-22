/**
 * Parse a Standard MIDI File into a tree of Features.
 * 
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include "BabelWiresLib/FileFormat/fileFeature.hpp"
#include "BabelWiresPlugins/Smf/Plugin/smfSourceModel.hpp"
#include "Common/IO/dataSource.hpp"
#include "SeqWiresLib/musicTypes.hpp"
#include "Common/Log/userLogger.hpp"

#include <cstdint>
#include <memory>
#include <sstream>
#include <vector>

namespace smf {

    class SmfParser {
      public:
        SmfParser(babelwires::DataSource& dataSource, babelwires::UserLogger& log);

        void parse();
        std::unique_ptr<babelwires::FileFeature> getResult() { return std::move(m_result); }

      protected:
        babelwires::Byte getNext();
        babelwires::Byte peekNext();

        /// Read the expected byte sequence.
        void readByteSequence(const char* seq);

        /// Reads the header chunk and set some metadata.
        void readHeaderChunk();

        void readFormat0Sequence(source::Format0SmfFeature& sequence);
        void readFormat1Sequence(source::Format1SmfFeature& sequence);

        void readTrack(int i, source::ChannelGroup& tracks, MidiMetadata& metadata);

        seqwires::ModelDuration readModelDuration();

        void readTempoEvent(seqwires::TempoFeature& tempo);

        std::string readTextMetaEvent(int length);

        void skipBytes(int numBytes);

        template<typename STREAMLIKE>
        void logByteSequence(STREAMLIKE log, int length);

        ///
        std::uint16_t readU16();
        std::uint32_t readU24();
        std::uint32_t readU32();
        std::uint32_t readVariableLengthQuantity();

      private:
        babelwires::DataSource& m_dataSource;
        babelwires::UserLogger& m_userLogger;
        std::unique_ptr<babelwires::FileFeature> m_result;

        source::SmfFeature::Format m_sequenceType;
        int m_numTracks;
        int m_division;
    };

    std::unique_ptr<babelwires::FileFeature> parseSmfSequence(babelwires::DataSource& dataSource, babelwires::UserLogger& userLogger);

} // namespace smf

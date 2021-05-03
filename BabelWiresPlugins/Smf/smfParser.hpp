/**
 * Parse a Standard MIDI File into a tree of Features.
 * 
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include "BabelWires/FileFormat/fileFeature.hpp"
#include "BabelWiresPlugins/Smf/smfSourceModel.hpp"
#include "Common/IO/dataSource.hpp"
#include "SeqWiresLib/musicTypes.hpp"

#include <cstdint>
#include <memory>
#include <sstream>
#include <vector>

namespace smf {

    class SmfParser {
      public:
        SmfParser(babelwires::DataSource& dataSource);

        static source::SmfSequence::Format getSequenceType(babelwires::DataSource& dataSource);

        void parse();
        std::unique_ptr<babelwires::FileFeature> getResult() { return std::move(m_result); }

      protected:
        babelwires::Byte getNext();
        babelwires::Byte peekNext();

        /// Read the expected byte sequence.
        void readByteSequence(const char* seq);

        /// Reads the header chunk and set some metadata.
        void readHeaderChunk();

        void readFormat0Sequence(source::Format0Sequence& sequence);
        void readFormat1Sequence(source::Format1Sequence& sequence);

        void readTrack(int i, source::ChannelGroup& tracks, seqwires::TempoFeature& tempo, babelwires::StringFeature* copyright,
                       babelwires::StringFeature* sequenceOrTrackName);

        seqwires::ModelDuration readModelDuration();

        void readTempoEvent(seqwires::TempoFeature& tempo);

        std::string readTextMetaEvent(int length);

        void skipBytes(int numBytes);

        ///
        std::uint16_t readU16();
        std::uint32_t readU24();
        std::uint32_t readU32();
        std::uint32_t readVariableLengthQuantity();

      private:
        babelwires::DataSource& m_dataSource;
        std::ostringstream m_log;
        std::unique_ptr<babelwires::FileFeature> m_result;

        source::SmfSequence::Format m_sequenceType;
        int m_numTracks;
        int m_division;
    };

    std::unique_ptr<babelwires::FileFeature> parseSmfSequence(babelwires::DataSource& dataSource);

} // namespace smf

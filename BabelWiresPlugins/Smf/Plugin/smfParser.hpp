/**
 * Parse a Standard MIDI File into a tree of Features.
 * 
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <BabelWiresPlugins/Smf/Plugin/smfSourceModel.hpp>
#include <BabelWiresPlugins/Smf/Plugin/gmSpec.hpp>

#include <SeqWiresLib/musicTypes.hpp>

#include <BabelWiresLib/FileFormat/fileFeature.hpp>

#include <Common/IO/dataSource.hpp>
#include <Common/Log/userLogger.hpp>

#include <cstdint>
#include <memory>
#include <sstream>
#include <vector>

namespace smf {

    class SmfParser {
      public:
        SmfParser(babelwires::DataSource& dataSource, const babelwires::ProjectContext& projectContext, babelwires::UserLogger& log);

        void parse();
        std::unique_ptr<babelwires::FileFeature> getResult() { return std::move(m_result); }

      protected:
        babelwires::Byte getNext();
        babelwires::Byte peekNext();

        void setGMSpec(GMSpecType::Value spec);

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

        void readSysExEvent();
        void readSysExEventContinuation();
        void readSequencerSpecificEvent(int length);

        ///
        std::uint16_t readU16();
        std::uint32_t readU24();
        std::uint32_t readU32();
        std::uint32_t readVariableLengthQuantity();

        /// Try to read length bytes into the buffer.
        void readFullMessageIntoBuffer(std::uint32_t length);

        /// A -1 in the message is allowed to be anything.
        template<std::size_t N>
        bool isMessageBufferMessage(const std::array<std::int16_t, N>& message) const;

        template<typename STREAMLIKE>
        void logMessageBuffer(STREAMLIKE log) const;

      private:
        const babelwires::ProjectContext& m_projectContext;
        babelwires::DataSource& m_dataSource;
        babelwires::UserLogger& m_userLogger;
        std::unique_ptr<babelwires::FileFeature> m_result;
        std::vector<babelwires::Byte> m_messageBuffer;

        source::SmfFeature::Format m_sequenceType;
        int m_numTracks;
        int m_division;

        // The specification which applies to the data.
        GMSpecType::Value m_gmSpec;
    };

    std::unique_ptr<babelwires::FileFeature> parseSmfSequence(babelwires::DataSource& dataSource, const babelwires::ProjectContext& projectContext, babelwires::UserLogger& userLogger);

} // namespace smf

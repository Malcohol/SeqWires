/**
 * Parse a Standard MIDI File into a tree of Features.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <Plugins/Smf/Plugin/gmSpec.hpp>
#include <Plugins/Smf/Plugin/smfSourceModel.hpp>
#include <Plugins/Smf/Plugin/Percussion/standardPercussionSets.hpp>

#include <SeqWiresLib/musicTypes.hpp>

#include <BabelWiresLib/FileFormat/fileFeature.hpp>

#include <Common/IO/dataSource.hpp>
#include <Common/Log/userLogger.hpp>

#include <cstdint>
#include <memory>
#include <sstream>
#include <vector>

namespace seqwires {
    class PercussionSetWithPitchMap;
}

namespace smf {

    class SmfParser {
      public:
        SmfParser(babelwires::DataSource& dataSource, const babelwires::ProjectContext& projectContext,
                  babelwires::UserLogger& log);
        virtual ~SmfParser();

        void parse();
        std::unique_ptr<source::SmfFeature> getResult() { return std::move(m_result); }

      protected:
        babelwires::Byte getNext();
        babelwires::Byte peekNext();

        GMSpecType::Value getGMSpec() const;
        void setGMSpec(GMSpecType::Value spec);

        /// Read the expected byte sequence.
        void readByteSequence(const char* seq);

        /// Reads the header chunk and set some metadata.
        void readHeaderChunk();

        void readFormat0Sequence(source::Format0SmfFeature& sequence);
        void readFormat1Sequence(source::Format1SmfFeature& sequence);

        void readTrack(int trackIndex, source::ChannelGroup& tracks, MidiMetadata* metadata = nullptr);

        seqwires::ModelDuration readModelDuration();

        void readTempoEvent(MidiMetadata* metadata);

        std::string readTextMetaEvent(int length);

        void skipBytes(int numBytes);

        template <typename STREAMLIKE> void logByteSequence(STREAMLIKE log, int length);

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
        template <std::size_t N> bool isMessageBufferMessage(const std::array<std::int16_t, N>& message) const;

        template <typename STREAMLIKE> void logMessageBuffer(STREAMLIKE log) const;

        class TrackSplitter;

        void readControlChange(unsigned int channelNumber);
        void readProgramChange(unsigned int channelNumber);
        void setBankMSB(unsigned int channelNumber, const babelwires::Byte msbValue);
        void setBankLSB(unsigned int channelNumber, const babelwires::Byte lsbValue);
        void setProgram(unsigned int channelNumber, const babelwires::Byte value);

        // Note: blockNumber is not the same as channel number.
        void setGsPartMode(unsigned int blockNumber, babelwires::Byte value);
        
        void onChangeProgram(unsigned int channelNumber);

        enum KnownPercussionSets { GM_PERCUSSION_KIT, GM2_STANDARD_PERCUSSION_KIT, NUM_KNOWN_PERCUSSION_KITS };

      private:
        const babelwires::ProjectContext& m_projectContext;
        babelwires::DataSource& m_dataSource;
        babelwires::UserLogger& m_userLogger;
        std::unique_ptr<source::SmfFeature> m_result;
        std::vector<babelwires::Byte> m_messageBuffer;

        source::SmfFeature::Format m_sequenceType;
        int m_numTracks;
        int m_division;

        /// Knowledge of how pitches map to percussion instruments.
        StandardPercussionSets m_standardPercussionSets;

        /// Currently just used to determine which tracks are percussion tracks.
        struct ChannelSetup {
            StandardPercussionSets::ChannelSetupInfo m_channelSetupInfo;
            // This is non-null when the pitches in the data should be interpreted as percussion events from the given
            // kit.
            const seqwires::PercussionSetWithPitchMap* m_kitIfPercussion = nullptr;
        };

        std::array<ChannelSetup, 16> m_channelSetup;
    };

    std::unique_ptr<source::SmfFeature> parseSmfSequence(babelwires::DataSource& dataSource,
                                                              const babelwires::ProjectContext& projectContext,
                                                              babelwires::UserLogger& userLogger);

} // namespace smf

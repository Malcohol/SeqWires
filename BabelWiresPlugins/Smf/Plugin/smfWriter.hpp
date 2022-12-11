/**
 * Write a Standard MIDI File based on the contents of a tree of Features.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <BabelWiresPlugins/Smf/Plugin/smfTargetModel.hpp>
#include <BabelWiresPlugins/Smf/Plugin/Percussion/percussionKit.hpp>

#include <SeqWiresLib/musicTypes.hpp>

#include <cstdint>
#include <ostream>

namespace babelwires {
    class UserLogger;
}

namespace seqwires {
    class Track;
    class TrackEvent;
} // namespace seqwires

namespace smf {

    class SmfWriter {
      public:
        SmfWriter(const babelwires::ProjectContext& projectContext, babelwires::UserLogger& userLogger,
                  const target::SmfFormatFeature& sequence, std::ostream& output);

        void write();

      protected:
        void writeUint16(std::uint16_t i);
        void writeUint24(std::uint32_t i);
        void writeUint32(std::uint32_t i);
        void writeVariableLengthQuantity(std::uint32_t i);
        void writeModelDuration(const seqwires::ModelDuration& d);

        /// Returns true if the event was written.
        enum class WriteTrackEventResult {
          Written,
          WrongCategory,
          NotInPercussionKit
        };
        WriteTrackEventResult writeTrackEvent(int channelNumber, seqwires::ModelDuration timeSinceLastEvent, const seqwires::TrackEvent& e);

        void writeTempoEvent(int bpm);

        /// type is the integer 0..15 which defines which type of text meta-event should be issued.
        void writeTextMetaEvent(int type, std::string text);

        void writeNotes(const target::ChannelGroup& track);

        void writeHeaderChunk();

        /// Tempo feature can be null.
        void writeTrack(const target::ChannelGroup* track, const MidiMetadata& metadata);

        // TODO Unify with parser code
        enum KnownPercussionKits {
            GM_PERCUSSION_KIT,
            GM2_STANDARD_PERCUSSION_KIT,
            NUM_KNOWN_PERCUSSION_KITS,
            NOT_PERCUSSION = NUM_KNOWN_PERCUSSION_KITS
        };

        /// Determine from the events in the tracks what percussion kit (allowed for the channelNumber) includes the largest number of the events.
        void setUpPercussionKit(std::vector<const seqwires::Track*> tracks, int channelNumber);

        void setUpPercussionKits();

      private:
        const babelwires::ProjectContext& m_projectContext;
        babelwires::UserLogger& m_userLogger;
        const target::SmfFormatFeature& m_smfFormatFeature;
        std::ostream& m_ostream;
        std::ostream* m_os;
        /// Always use metrical time. Quater-note division.
        int m_division;

        std::array<const smf::PercussionKit*, NUM_KNOWN_PERCUSSION_KITS> m_knownKits;

                /// Currently just used to determine which tracks are percussion tracks.
        struct ChannelSetup {
            //babelwires::Byte m_bankMSB = 0;
            //babelwires::Byte m_bankLSB = 0;
            //babelwires::Byte m_program = 0;
            // This is the part corresponding to this channel, irrespective of the part mapping.
            //babelwires::Byte m_gsPartMode = 0;
            // This is non-null when the pitches in the data should be interpreted as percussion events from the given
            // kit.
            const smf::PercussionKit* m_kitIfPercussion = nullptr;
        };

        std::array<ChannelSetup, 16> m_channelSetup;
    };

    void writeToSmf(const babelwires::ProjectContext& projectContext, babelwires::UserLogger& userLogger,
                    const target::SmfFormatFeature& smfFormatFeature, std::ostream& output);

} // namespace smf

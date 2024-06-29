/**
 * Write a Standard MIDI File based on the contents of a tree of Features.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <Plugins/Smf/Plugin/smfTargetModel.hpp>
#include <Plugins/Smf/Plugin/Percussion/standardPercussionSets.hpp>

#include <SeqWiresLib/musicTypes.hpp>

#include <cstdint>
#include <ostream>

namespace babelwires {
    struct UserLogger;
}

namespace seqwires {
    class Track;
    class TrackEvent;
} // namespace seqwires

namespace smf {

    class SmfWriter {
      public:
        SmfWriter(const babelwires::ProjectContext& projectContext, babelwires::UserLogger& userLogger,
                  const target::SmfFeature& sequence, std::ostream& output);

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
          NotInPercussionSet
        };
        WriteTrackEventResult writeTrackEvent(int channelNumber, seqwires::ModelDuration timeSinceLastEvent, const seqwires::TrackEvent& e);

        void writeTempoEvent(int bpm);

        /// type is the integer 0..15 which defines which type of text meta-event should be issued.
        void writeTextMetaEvent(int type, std::string text);

        // TODO It should be easier to express this.
        using TrackFeatureWrapper = babelwires::FeatureWrapper<const babelwires::ValueFeature, MidiTrackAndChannel>;

        void writeNotes(const std::vector<TrackFeatureWrapper>& tracks);

        void writeHeaderChunk();

        /// Write the events for the given track.
        void writeTrack(const std::vector<TrackFeatureWrapper>& tracks, bool includeGlobalSetup);

        /// Write non-channel-specific setup information.
        void writeGlobalSetup();

        /// Determine from the events in the tracks what percussion kit (allowed for the channelNumber) includes the largest number of the events.
        void setUpPercussionKit(const std::unordered_set<babelwires::ShortId>& instrumentsInUse, int channelNumber);

        void setUpPercussionSets();

        template <std::size_t N> void writeMessage(const std::array<std::uint8_t, N>& message);

      private:
        const babelwires::ProjectContext& m_projectContext;
        babelwires::UserLogger& m_userLogger;
        const target::SmfFeature& m_smfFeature;
        std::ostream& m_ostream;
        std::ostream* m_os;
        /// Always use metrical time. Quater-note division.
        int m_division;

        StandardPercussionSets m_standardPercussionSets;

        /// Currently just used to determine which tracks are percussion tracks.
        struct ChannelSetup {
            // This is non-null when the pitches in the data should be interpreted as percussion events from the given
            // kit.
            const seqwires::PercussionSetWithPitchMap* m_kitIfPercussion = nullptr;
            // Has channel set-up information been written for this channel yet?
            // (Since more than one track can correspond to a channel, we only want to write this for the first track for each channel.)
            bool m_setupWritten = false;
        };

        std::array<ChannelSetup, 16> m_channelSetup;
    };

    void writeToSmf(const babelwires::ProjectContext& projectContext, babelwires::UserLogger& userLogger,
                    const target::SmfFeature& smfFeature, std::ostream& output);

} // namespace smf

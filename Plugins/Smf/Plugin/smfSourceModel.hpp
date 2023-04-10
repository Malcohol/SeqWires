/**
 * Representation of a Standard MIDI File as a tree of Features.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <BabelWiresLib/Features/arrayFeature.hpp>
#include <BabelWiresLib/Features/recordWithOptionalsFeature.hpp>
#include <BabelWiresLib/FileFormat/fileFeature.hpp>
#include <BabelWiresLib/Types/Int/intFeature.hpp>
#include <BabelWiresLib/Types/String/stringFeature.hpp>

#include <SeqWiresLib/Features/tempoFeature.hpp>

#include <Plugins/Smf/Plugin/smfModelCommon.hpp>

namespace seqwires {
    class TrackFeature;
}

namespace smf {

    namespace source {
        /// An abstraction for features which organize tracks for different MIDI channels.
        /// This is used since we target a different structure when parsing Format 0 and Format 1 files.
        class ChannelGroup {
          public:
            /// If necessary, special-case this track. This must be called before tracks are added.
            virtual void setPrivilegedTrack(int c) {}
            /// c is the MIDI channel, not index.
            virtual seqwires::TrackFeature* addTrack(int c) = 0;
        };

        /// Organizes the tracks corresponding to channels as a record with field names such as "Ch 3".
        /// For Format 0 files, channel number gives identity to the TrackFeatures.
        class RecordChannelGroup : public babelwires::RecordFeature, public ChannelGroup {
          public:
            /// c is the MIDI channel, not index.
            seqwires::TrackFeature* addTrack(int c) override;
        };

        /// The tracks of format 1 files usually carry events for one channel. However:
        /// (a) more than one track can have data for the same channel.
        /// (b) a track can carry events for multiple channels.
        /// See discussion here: https://www.midi.org/forum/8793-smf-format-1-tracks-and-channels
        /// Because of (a), I chose track number rather than channel number to give identity to the
        /// TrackFeatures. That isn't a complete solution because of (b). However, since (b) is
        /// very rare, I believe the following to be an acceptable compromise:
        /// We pick the channel in each track which has the most events, and give it an identity
        /// which just depends on the track number. This should be pretty stable.
        /// All other channels in the track are given an identity which depends on the track number
        /// and channel. This is less stable, but OK. Problems will arise in the very rare situation
        /// where a track had a similar number of events for two channels, and on reload, the
        /// the smaller became the larger.
        class ExtensibleChannelGroup : public babelwires::RecordFeature, public ChannelGroup {
          public:
            void setPrivilegedTrack(int c) override;
            /// c is the MIDI channel, not index.
            seqwires::TrackFeature* addTrack(int c) override;

          protected:
            babelwires::IntFeature* m_channelNum = nullptr;
            seqwires::TrackFeature* m_trackFeature = nullptr;
        };

        /// Common to all formats of SmfFeature.
        class SmfFeature : public babelwires::FileFeature {
          public:
            enum Format { SMF_FORMAT_0, SMF_FORMAT_1, SMF_FORMAT_2, SMF_UNKNOWN_FORMAT };
            Format getFormat() const;

            virtual int getNumMidiTracks() const = 0;
            virtual const ChannelGroup& getMidiTrack(int i) const = 0;

            MidiMetadata& getMidiMetadata();
            const MidiMetadata& getMidiMetadata() const;

          protected:
            SmfFeature(const babelwires::ProjectContext& projectContext, Format f);

          protected:
            Format m_format;
            MidiMetadata* m_metadata;
            babelwires::StringFeature* m_sequenceName;
            babelwires::StringFeature* m_copyright;
            seqwires::TempoFeature* m_tempo;
        };

        /// All note data for each channel is mixed into one midi track.
        class Format0SmfFeature : public SmfFeature {
          public:
            Format0SmfFeature(const babelwires::ProjectContext& projectContext);

            virtual int getNumMidiTracks() const override;
            virtual const ChannelGroup& getMidiTrack(int i) const override;

            ChannelGroup* getMidiTrack0();

          protected:
            ChannelGroup* m_channelGroup;
        };

        class TrackArray : public babelwires::ArrayFeature {
          protected:
            virtual std::unique_ptr<Feature> createNextEntry() const override;
        };

        /// Note data is organized into tracks, but the events may belong to different channels.
        class Format1SmfFeature : public SmfFeature {
          public:
            Format1SmfFeature(const babelwires::ProjectContext& projectContext);

            virtual int getNumMidiTracks() const override;
            virtual const ChannelGroup& getMidiTrack(int i) const override;

            ChannelGroup* addMidiTrack();

          protected:
            TrackArray* m_tracks;
        };
    } // namespace source
} // namespace smf

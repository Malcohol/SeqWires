/**
 * Representation of a Standard MIDI File as a tree of Features.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include "BabelWiresLib/Features/arrayFeature.hpp"
#include "BabelWiresLib/FileFormat/fileFeature.hpp"
#include "BabelWiresPlugins/Smf/Plugin/smfModelCommon.hpp"

namespace seqwires {
    class TrackFeature;
}

namespace smf {
    namespace target {
        class ChannelTrackFeature;

        /// An abstraction for track data for multiple midi channels.
        /// This abstraction is useful in the writer, since the data is structured differently
        /// in Format 0 and Format 1 files.
        class ChannelGroup {
          public:
            virtual int getNumTracks() const = 0;
            virtual const ChannelTrackFeature& getTrack(int i) const = 0;
        };

        /// A track and its MIDI channel number.
        /// This can also act as a ChannelGroup containing one ChannelTrackFeature (itself).
        class ChannelTrackFeature : public babelwires::RecordFeature, public ChannelGroup {
          public:
            ChannelTrackFeature();

          public:
            virtual int getNumTracks() const override;
            virtual const ChannelTrackFeature& getTrack(int i) const override;

          public:
            babelwires::IntFeature* m_channelNum;
            seqwires::TrackFeature* m_trackFeature;
        };

        /// An array of tracks and their MIDI channels.
        class ArrayChannelGroup : public babelwires::ArrayFeature, public ChannelGroup {
          public:
            virtual int getNumTracks() const override;
            virtual const ChannelTrackFeature& getTrack(int i) const override;

          protected:
            std::unique_ptr<Feature> createNextEntry() const override;
            babelwires::Range<unsigned int> doGetSizeRange() const override;
        };

        /// Common to all formats of SmfFeature.
        class SmfFeature : public babelwires::FileFeature {
          public:
            enum Format { SMF_FORMAT_0, SMF_FORMAT_1, SMF_FORMAT_2, SMF_UNKNOWN_FORMAT };
            Format getFormat() const;

            virtual int getNumMidiTracks() const = 0;
            virtual const ChannelGroup& getMidiTrack(int i) const = 0;

            const MidiMetadata& getMidiMetadata() const;
          protected:
            SmfFeature(Format f);

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
            Format0SmfFeature();

            virtual int getNumMidiTracks() const override;
            virtual const ChannelGroup& getMidiTrack(int i) const override;

          protected:
            ArrayChannelGroup* m_channelGroup;
        };

        /// Note data is organized into tracks, but the events may belong to different channels.
        /// For creating Format 1 files, we do not support MIDI tracks with events for multiple channels.
        class Format1SmfFeature : public SmfFeature {
          public:
            Format1SmfFeature();

            virtual int getNumMidiTracks() const override;
            virtual const ChannelGroup& getMidiTrack(int i) const override;

          protected:
            ArrayChannelGroup* m_channelGroup;
        };
    } // namespace target
} // namespace smf

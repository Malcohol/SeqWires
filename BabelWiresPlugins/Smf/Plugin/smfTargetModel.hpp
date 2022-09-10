/**
 * Representation of a Standard MIDI File as a tree of Features.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <BabelWiresLib/Features/arrayFeature.hpp>
#include <BabelWiresLib/FileFormat/fileFeature.hpp>
#include <BabelWiresLib/Features/unionFeature.hpp>
#include <BabelWiresPlugins/Smf/Plugin/smfModelCommon.hpp>

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

        class SmfFormatFeature : public babelwires::UnionFeature {
          public:
            SmfFormatFeature();

            const MidiMetadata& getMidiMetadata() const;
            int getNumMidiTracks() const;
            const ChannelGroup& getMidiTrack(int i) const;

            Style getStyle() const override;

          protected:
            MidiMetadata* m_metadata;
            babelwires::StringFeature* m_sequenceName;
            babelwires::StringFeature* m_copyright;
            seqwires::TempoFeature* m_tempo;
            ArrayChannelGroup* m_channelGroup;
        };

        /// Common to all formats of SmfFeature.
        class SmfFeature : public babelwires::FileFeature {
          public:
            SmfFeature(const babelwires::ProjectContext& projectContext);

            const SmfFormatFeature& getFormatFeature() const;
          protected:
            SmfFormatFeature* m_formatFeature;
        };
    } // namespace target
} // namespace smf

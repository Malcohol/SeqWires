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
    class Track;
    class TrackFeature;
}

namespace smf {
    namespace target {
        class ChannelTrackFeature;

        /// A track and its MIDI channel number.
        /// This can also act as a ChannelGroup containing one ChannelTrackFeature (itself).
        class ChannelTrackFeature : public babelwires::RecordFeature {
          public:
            ChannelTrackFeature();

            int getChannelNumber() const;
            const seqwires::Track& getTrack() const;

          public:
            babelwires::IntFeature* m_channelNum;
            seqwires::TrackFeature* m_trackFeature;
        };

        /// An array of tracks and their MIDI channels.
        class ArrayChannelGroup : public babelwires::ArrayFeature {
          public:
            int getNumTracks() const;
            const ChannelTrackFeature& getTrack(int i) const;

          protected:
            std::unique_ptr<Feature> createNextEntry() const override;
            babelwires::Range<unsigned int> doGetSizeRange() const override;
        };

        class SmfFormatFeature : public babelwires::UnionFeature {
          public:
            SmfFormatFeature();

            const MidiMetadata& getMidiMetadata() const;

            // Convenience: Dispatches to the ArrayChannelGroup.
            int getNumMidiTracks() const;
            const ChannelTrackFeature& getMidiTrack(int i) const;

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

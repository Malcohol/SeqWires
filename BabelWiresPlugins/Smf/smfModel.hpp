/**
 * Representation of a Standard MIDI File as a tree of Features.
 * 
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include "BabelWires/Features/arrayFeature.hpp"
#include "BabelWires/Features/stringFeature.hpp"
#include "BabelWires/FileFormat/fileFeature.hpp"
#include "SeqWiresLib/Features/tempoFeature.hpp"

namespace seqwires {
    class TrackFeature;
}

namespace smf {

    /// A track with its MIDI channel number.
    class ChannelTrackFeature : public babelwires::RecordFeature {
      public:
        ChannelTrackFeature();

      public:
        babelwires::IntFeature* m_channelNum;
        seqwires::TrackFeature* m_noteTrackFeature;
    };

    /// An array tracks for different MIDI channels.
    class ChannelGroup : public babelwires::ArrayFeature {
      public:
        /// c is the MIDI channel, not index.
        const seqwires::TrackFeature* getTrack(int c) const;
        seqwires::TrackFeature* addTrack(int c);

      protected:
        virtual std::unique_ptr<Feature> createNextEntry() const override;
    };

    /// Common to all formats of SmfSequence.
    class SmfSequence : public babelwires::FileFeature {
      public:
        enum Format { FORMAT_0_SEQUENCE, FORMAT_1_SEQUENCE, FORMAT_2_SEQUENCE, UNKNOWN_SEQUENCE_TYPE };
        Format getFormat() const;

        seqwires::TempoFeature* getTempoFeature();
        const seqwires::TempoFeature* getTempoFeature() const;

        virtual int getNumMidiTracks() const = 0;
        virtual const ChannelGroup& getMidiTrack(int i) const = 0;

        babelwires::StringFeature* getCopyright();
        const babelwires::StringFeature* getCopyright() const;

        babelwires::StringFeature* getSequenceName();
        const babelwires::StringFeature* getSequenceName() const;

      protected:
        SmfSequence(Format f);

      protected:
        Format m_format;
        babelwires::StringFeature* m_sequenceName;
        babelwires::StringFeature* m_copyright;
        seqwires::TempoFeature* m_tempo;
    };

    /// All note data for each channel is mixed into one midi track.
    class Format0Sequence : public SmfSequence {
      public:
        Format0Sequence();

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
    class Format1Sequence : public SmfSequence {
      public:
        Format1Sequence();

        virtual int getNumMidiTracks() const override;
        ;
        virtual const ChannelGroup& getMidiTrack(int i) const override;
        ;

        ChannelGroup* addMidiTrack();

      protected:
        TrackArray* m_tracks;
    };

} // namespace smf

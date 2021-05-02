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
namespace import {
    /// An abstraction for features which organize tracks for different MIDI channels.
    /// Much of structure of imported and exported data is similar.
    /// However, one difference is that a track of events for different channels is exposed
    /// as a record for importer, but an array for export.
    class ChannelGroup {
      public:
        /// If necessary, special-case the first channel encountered.
        virtual void setFirstChannelEncountered(int c) {}
        /// c is the MIDI channel, not index.
        virtual seqwires::TrackFeature* addTrack(int c) = 0;
    };

    /// Organizes the tracks as a record with field names such as "Ch 3".
    /// This is good for import, where channel number better identifies a track than its
    /// position in an array.
    /// This is not suitable for export since there's no UI for adding fields to a record.
    class RecordChannelGroup : public babelwires::RecordFeature, public ChannelGroup {
      public:
        /// c is the MIDI channel, not index.
        seqwires::TrackFeature* addTrack(int c) override;
    };

    /// This carries one channel (the first encountered) which is treated specially,
    /// and if there are others, they fall back to the RecordChannelGroup parent.
    /// This handles the rare situation in Format 1 files, where a track can have data
    /// for more than one channel.
    class ExtensibleChannelGroup : public babelwires::RecordFeature, public ChannelGroup {
      public:
        void setFirstChannelEncountered(int c) override;
        /// c is the MIDI channel, not index.
        seqwires::TrackFeature* addTrack(int c) override;
      protected:
        babelwires::IntFeature* m_channelNum = nullptr;
        seqwires::TrackFeature* m_noteTrackFeature = nullptr;
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
        virtual const ChannelGroup& getMidiTrack(int i) const override;

        ChannelGroup* addMidiTrack();

      protected:
        TrackArray* m_tracks;
    };
}
} // namespace smf

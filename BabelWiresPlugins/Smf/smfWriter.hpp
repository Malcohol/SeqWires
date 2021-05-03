/**
 * Write a Standard MIDI File based on the contents of a tree of Features.
 * 
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include "BabelWiresPlugins/Smf/smfTargetModel.hpp"
#include "SeqWiresLib/musicTypes.hpp"

#include <cstdint>
#include <ostream>

namespace seqwires {
    class TrackEvent;
}

namespace smf {

    class SmfWriter {
      public:
        SmfWriter(std::ostream& output);

        void writeHeaderChunk(const target::SmfSequence& sequence);

        /// Tempo feature can be null.
        void writeTrack(const target::ChannelGroup* track, const seqwires::TempoFeature* tempo,
                            const babelwires::StringFeature* copyright,
                            const babelwires::StringFeature* sequenceOrTrackName);

      protected:
        void writeUint16(std::uint16_t i);
        void writeUint24(std::uint32_t i);
        void writeUint32(std::uint32_t i);
        void writeVariableLengthQuantity(std::uint32_t i);
        void writeModelDuration(const seqwires::ModelDuration& d);

        void writeNoteEvent(int channel, seqwires::ModelDuration timeSinceLastEvent, const seqwires::TrackEvent& e);
        void writeTempoEvent(int bpm);
        /// type is the integer 0..15 which defines which type of text meta-event should be issued.
        void writeTextMetaEvent(int type, std::string text);
        void writeNotes(const target::ChannelGroup& track);

      private:
        std::ostream& m_ostream;
        std::ostream* m_os;
        /// Always use metrical time. Quater-note division.
        int m_division;
    };

    void writeToSmfFormat0(std::ostream& output, const target::Format0Sequence& sequence);
    void writeToSmfFormat1(std::ostream& output, const target::Format1Sequence& sequence);

} // namespace smf

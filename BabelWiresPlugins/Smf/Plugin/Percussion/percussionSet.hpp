/**
 * An Enum of percussion instruments which can map to pitch values.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <SeqWiresLib/musicTypes.hpp>
#include <SeqWiresLib/builtInPercussionInstruments.hpp>

namespace smf {
    /// An base class for Enums of percussion instruments which can be mapped to pitches.
    class PercussionSet : public babelwires::Enum {
      public:
        /// A block of instruments which have contiguous pitches.
        /// The block may contain duplicates.
        struct InstrumentBlock {
          std::vector<babelwires::Identifier> m_instruments;
          seqwires::Pitch m_pitchOfLowestInstrument;
        };

        /// This constructor expects instruments to be in a single contiguous block.
        PercussionSet(babelwires::LongIdentifier identifier, babelwires::VersionNumber version, InstrumentBlock instruments, seqwires::Pitch pitchOfDefaultValue);

        /// Get the pitch of an instrument if it is in this set.
        /// The highest pitch of any duplicate instrument is returned.
        std::optional<seqwires::Pitch> tryGetPitchFromInstrument(babelwires::Identifier identifier) const;

        /// Get the instrument corresponding to the pitch, if it is in this set.
        std::optional<babelwires::Identifier> tryGetInstrumentFromPitch(seqwires::Pitch pitch) const;

      private:
        std::unordered_map<seqwires::Pitch, babelwires::Identifier> m_pitchToInstrument;
        std::unordered_map<babelwires::Identifier, seqwires::Pitch> m_instrumentToPitch;
    };
}

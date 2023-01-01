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
        /// Unlike EnumValues, the instruments vector may contain duplicates.
        /// This constructor expects instruments to be in a contiguous block starting at pitchOfLowestInstrument.
        PercussionSet(babelwires::LongIdentifier identifier, babelwires::VersionNumber version, std::vector<babelwires::Identifier> instruments,
                      unsigned int indexOfDefaultValue, seqwires::Pitch pitchOfLowestInstrument);

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

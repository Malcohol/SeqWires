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
    /// An base class for Enums of percussion instruments which can be mapped to pitches in a contiguous block.
    class PercussionSet : public babelwires::Enum {
      public:
        PercussionSet(babelwires::LongIdentifier identifier, babelwires::VersionNumber version, EnumValues values,
                      unsigned int indexOfDefaultValue, seqwires::Pitch pitchOfLowestInstrument);

        std::optional<seqwires::Pitch> tryGetPitchFromInstrument(babelwires::Identifier identifier) const;

        std::optional<babelwires::Identifier> tryGetInstrumentFromPitch(seqwires::Pitch pitch) const;

      private:
        seqwires::Pitch m_pitchOfLowestInstrument;
    };
}

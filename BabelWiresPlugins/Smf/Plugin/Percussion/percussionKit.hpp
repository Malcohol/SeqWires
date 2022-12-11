/**
 * An abstraction for Enums of percussion instruments which can map to pitch values.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <SeqWiresLib/musicTypes.hpp>
#include <SeqWiresLib/percussion.hpp>

namespace smf {
    /// An enum of percussion instruments which can be mapped to pitches in a contiguous block.
    class PercussionKit : public babelwires::Enum {
      public:
        PercussionKit(babelwires::LongIdentifier identifier, babelwires::VersionNumber version, EnumValues values,
                      unsigned int indexOfDefaultValue, seqwires::Pitch pitchOfLowestInstrument);

        std::optional<seqwires::Pitch> tryGetPitchFromInstrument(babelwires::Identifier identifier) const;

        std::optional<babelwires::Identifier> tryGetInstrumentFromPitch(seqwires::Pitch pitch) const;
      private:
        seqwires::Pitch m_pitchOfLowestInstrument;
    };
}

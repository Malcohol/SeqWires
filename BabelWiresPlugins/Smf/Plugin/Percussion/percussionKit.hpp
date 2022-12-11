/**
 * An enum which defines the available MIDI specifications.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <SeqWiresLib/musicTypes.hpp>
#include <SeqWiresLib/percussion.hpp>

namespace smf {
    /// An enum of percussion instruments which has a 1-1 mapping to pitch values.
    /// This abstraction could potentially be moved into SeqWiresLib.
    class PercussionKit : public babelwires::Enum {
      public:
        PercussionKit(babelwires::LongIdentifier identifier, babelwires::VersionNumber version, EnumValues values,
                      unsigned int indexOfDefaultValue);

        virtual std::optional<seqwires::Pitch> tryGetPitchFromInstrument(babelwires::Identifier identifier) const = 0;

        /// If the pitch is in range, set indexOut and return true.
        virtual std::optional<babelwires::Identifier> tryGetInstrumentFromPitch(seqwires::Pitch pitch) const = 0;
    };
}

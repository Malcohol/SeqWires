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

    // TODO GS, XG percussion, with appropriate subtyping.

    /// A PercussionKit corresponding to the original General MIDI percussion set.
    class GMPercussionKit : public PercussionKit {
      public:
        GMPercussionKit(const seqwires::BuiltInPercussionInstruments& builtInInstruments);

        static babelwires::LongIdentifier getThisIdentifier();

        std::optional<seqwires::Pitch> tryGetPitchFromInstrument(babelwires::Identifier identifier) const override;

        std::optional<babelwires::Identifier> tryGetInstrumentFromPitch(seqwires::Pitch pitch) const override;
    };

    /// A PercussionKit corresponding to the instruments of GM2 standard percussion set.
    class GM2StandardPercussionKit : public PercussionKit {
      public:
        GM2StandardPercussionKit(const seqwires::BuiltInPercussionInstruments& builtInInstruments);

        static babelwires::LongIdentifier getThisIdentifier();

        std::optional<seqwires::Pitch> tryGetPitchFromInstrument(babelwires::Identifier identifier) const override;

        std::optional<babelwires::Identifier> tryGetInstrumentFromPitch(seqwires::Pitch pitch) const override;
    };

} // namespace smf
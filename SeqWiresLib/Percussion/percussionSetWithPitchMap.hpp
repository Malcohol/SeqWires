/**
 * An Enum of percussion instruments which can map to pitch values.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <SeqWiresLib/musicTypes.hpp>
#include <SeqWiresLib/Percussion/builtInPercussionInstruments.hpp>

#include <variant>

namespace seqwires {
    /// An convenience class for Enums of percussion instruments which provides support for mapping between instruments and pitch.
    /// Note: Using this class is an implementation detail, and does not affect an Enum's position in the TypeSystem.
    /// (This contrasts with AbstractPercussionType, which can be used to declare an enum as containing percussion instruments).
    /// In particular, not every enum of percussion instruments is required to derive from PercussionSet.
    /// Instruments may be duplicated (i.e. correspond to more than one pitch), but the lowest such pitch is always
    /// used when converting to pitches.
    /// PercussionSets should use as many instruments from the BuiltInPercussionInstruments Enum as possible,
    /// although instruments registered elsewhere are also supported.
    class PercussionSetWithPitchMap : public babelwires::Enum {
      public:
        /// A block of instruments which have contiguous pitches.
        struct InstrumentBlock {
          /// For convenience, you can directly reference built-in percussion instruments.
          std::vector<std::variant<babelwires::Identifier, seqwires::BuiltInPercussionInstruments::Value>> m_instruments;
          seqwires::Pitch m_pitchOfLowestInstrument;
          // Only needed if values are present in m_instruments.
          const seqwires::BuiltInPercussionInstruments* m_builtInPercussionInstruments = nullptr;
        };

        /// This constructor expects instruments to be in a single contiguous block.
        PercussionSetWithPitchMap(babelwires::LongIdentifier identifier, babelwires::VersionNumber version, InstrumentBlock instruments, seqwires::Pitch pitchOfDefaultValue);

        /// This constructor allows more than one, non-overlapping block of instruments.
        PercussionSetWithPitchMap(babelwires::LongIdentifier identifier, babelwires::VersionNumber version, std::vector<InstrumentBlock> instruments, seqwires::Pitch pitchOfDefaultValue);

        /// Get the pitch of an instrument if it is in this set.
        /// The highest pitch of any duplicate instrument is returned.
        std::optional<seqwires::Pitch> tryGetPitchFromInstrument(babelwires::Identifier identifier) const;

        /// Get the instrument corresponding to the pitch, if it is in this set.
        std::optional<babelwires::Identifier> tryGetInstrumentFromPitch(seqwires::Pitch pitch) const;

      private:
        // Private class just used to pass complex calculated arguments to the base class constructor.
        class ComplexConstructorArguments;

        PercussionSetWithPitchMap(babelwires::LongIdentifier identifier, babelwires::VersionNumber version, ComplexConstructorArguments&& removeDuplicates);

      private:
        std::unordered_map<seqwires::Pitch, babelwires::Identifier> m_pitchToInstrument;
        std::unordered_map<babelwires::Identifier, seqwires::Pitch> m_instrumentToPitch;
    };
}

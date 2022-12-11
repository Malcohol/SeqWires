/**
 * An enum which defines the available MIDI specifications.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <BabelWiresPlugins/Smf/Plugin/Percussion/percussionKit.hpp>

namespace smf {
    // TODO GS, XG percussion, with appropriate subtyping.

    /// A PercussionKit corresponding to the original General MIDI percussion set.
    class GMPercussionKit : public PercussionKit {
      public:
        GMPercussionKit(const seqwires::BuiltInPercussionInstruments& builtInInstruments);

        static babelwires::LongIdentifier getThisIdentifier();

        std::optional<seqwires::Pitch> tryGetPitchFromInstrument(babelwires::Identifier identifier) const override;

        std::optional<babelwires::Identifier> tryGetInstrumentFromPitch(seqwires::Pitch pitch) const override;
    };
} // namespace smf
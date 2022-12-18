/**
 * A PercussionSet corresponding to the original General MIDI percussion set.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <BabelWiresPlugins/Smf/Plugin/Percussion/percussionSet.hpp>

namespace smf {
    // TODO GS, XG percussion, with appropriate subtyping.

    /// A PercussionSet corresponding to the original General MIDI percussion set.
    class GMPercussionSet : public PercussionSet {
      public:
        GMPercussionSet(const seqwires::BuiltInPercussionInstruments& builtInInstruments);

        static babelwires::LongIdentifier getThisIdentifier();
    };
} // namespace smf
/**
 * A PercussionSet corresponding to the original General MIDI Percussion Set.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <SeqWiresLib/Percussion/percussionSet.hpp>

namespace smf {
    // TODO GS, XG percussion, with appropriate subtyping.

    /// A PercussionSet corresponding to the original General MIDI percussion set.
    class GMPercussionSet : public seqwires::PercussionSet {
      public:
        GMPercussionSet(const seqwires::BuiltInPercussionInstruments& builtInInstruments);

        static babelwires::LongIdentifier getThisIdentifier();
    };
} // namespace smf
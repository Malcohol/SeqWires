/**
 * A PercussionSet corresponding to the instruments of GM2 Brush Percussion Set.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <SeqWiresLib/Percussion/percussionSet.hpp>

namespace smf {
    /// A PercussionSet corresponding to the instruments of GM2 Brush Percussion Set.
    class GM2BrushPercussionSet : public seqwires::PercussionSet {
      public:
        GM2BrushPercussionSet(const seqwires::BuiltInPercussionInstruments& builtInInstruments);

        static babelwires::LongIdentifier getThisIdentifier();
    };
} // namespace smf

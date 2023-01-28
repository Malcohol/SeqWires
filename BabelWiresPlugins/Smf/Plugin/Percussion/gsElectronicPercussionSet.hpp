/**
 * A PercussionSet corresponding to the instruments of GS Electronic Percussion Set.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <SeqWiresLib/Percussion/percussionSet.hpp>

namespace smf {
    /// A PercussionSet corresponding to the instruments of GS Electronic Percussion Set.
    /// This can also serve for the GS Dance set.
    class GsElectronicPercussionSet : public seqwires::PercussionSet {
      public:
        GsElectronicPercussionSet(const seqwires::BuiltInPercussionInstruments& builtInInstruments);

        static babelwires::LongIdentifier getThisIdentifier();
    };
} // namespace smf
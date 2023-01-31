/**
 * A PercussionSet corresponding to the instruments of GM2 Electronic Percussion Set.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <SeqWiresLib/Percussion/percussionSetWithPitchMap.hpp>

namespace smf {
    /// A PercussionSet corresponding to the instruments of GM2 Electronic Percussion Set.
    class GM2ElectronicPercussionSet : public seqwires::PercussionSetWithPitchMap {
      public:
        GM2ElectronicPercussionSet(const seqwires::BuiltInPercussionInstruments& builtInInstruments);

        static babelwires::LongIdentifier getThisIdentifier();
    };
} // namespace smf
/**
 * A PercussionSet corresponding to the instruments of GM2 Orchestra Percussion Set.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <SeqWiresLib/Percussion/percussionSetWithPitchMap.hpp>

namespace smf {
    /// A PercussionSet corresponding to the instruments of GM2 Orchestra percussion set.
    class GM2OrchestraPercussionSet : public seqwires::PercussionSetWithPitchMap {
      public:
        GM2OrchestraPercussionSet(const seqwires::BuiltInPercussionInstruments& builtInInstruments);

        static babelwires::LongIdentifier getThisIdentifier();
    };
} // namespace smf
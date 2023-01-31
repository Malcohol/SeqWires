/**
 * A PercussionSet corresponding to the instruments of GS Orchestra Percussion Set.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <SeqWiresLib/Percussion/percussionSetWithPitchMap.hpp>

namespace smf {
    /// A PercussionSet corresponding to the instruments of GS Orchestra percussion set.
    class GsOrchestraPercussionSet : public seqwires::PercussionSetWithPitchMap {
      public:
        GsOrchestraPercussionSet(const seqwires::BuiltInPercussionInstruments& builtInInstruments);

        static babelwires::LongIdentifier getThisIdentifier();
    };
} // namespace smf
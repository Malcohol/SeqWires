/**
 * A PercussionSet corresponding to the instruments of GS Jazz Percussion Set.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <BabelWiresPlugins/Smf/Plugin/Percussion/percussionSet.hpp>

namespace smf {
    /// A PercussionSet corresponding to the instruments of GS Jazz Percussion set.
    class GsJazzPercussionSet : public PercussionSet {
      public:
        GsJazzPercussionSet(const seqwires::BuiltInPercussionInstruments& builtInInstruments);

        static babelwires::LongIdentifier getThisIdentifier();
    };
} // namespace smf
/**
 * A PercussionSet corresponding to the instruments of XG Standard 1 Percussion Set.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <BabelWiresPlugins/Smf/Plugin/Percussion/percussionSet.hpp>

namespace smf {
    /// A PercussionSet corresponding to the instruments of XG Standard 1 percussion set.
    class XgStandard1PercussionSet : public PercussionSet {
      public:
        XgStandard1PercussionSet(const seqwires::BuiltInPercussionInstruments& builtInInstruments);

        static babelwires::LongIdentifier getThisIdentifier();
    };
} // namespace smf
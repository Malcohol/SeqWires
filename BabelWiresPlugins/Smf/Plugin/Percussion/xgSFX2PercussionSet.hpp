/**
 * A PercussionSet corresponding to the instruments of XG SFX 1 Percussion Set.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <BabelWiresPlugins/Smf/Plugin/Percussion/percussionSet.hpp>

namespace smf {
    /// A PercussionSet corresponding to the instruments of XG SFX 1 percussion set.
    /// This also works as the XG Standard 2 percussion set.
    class XgSFX2PercussionSet : public PercussionSet {
      public:
        XgSFX2PercussionSet(const seqwires::BuiltInPercussionInstruments& builtInInstruments);

        static babelwires::LongIdentifier getThisIdentifier();
    };
} // namespace smf
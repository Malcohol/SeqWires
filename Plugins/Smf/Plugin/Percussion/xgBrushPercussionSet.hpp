/**
 * A PercussionSet corresponding to the instruments of XG Standard 1 Percussion Set.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <SeqWiresLib/Percussion/percussionSetWithPitchMap.hpp>

namespace smf {
    /// A PercussionSet corresponding to the instruments of XG Standard 1 percussion set.
    /// This also works as the XG Standard 2 percussion set.
    class XgBrushPercussionSet : public seqwires::PercussionSetWithPitchMap {
      public:
        XgBrushPercussionSet(const seqwires::BuiltInPercussionInstruments& builtInInstruments);

        static babelwires::LongIdentifier getThisIdentifier();
    };
} // namespace smf
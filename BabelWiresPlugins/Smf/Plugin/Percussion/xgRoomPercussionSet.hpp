/**
 * A PercussionSet corresponding to the instruments of XG Room Percussion Set.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <SeqWiresLib/Percussion/percussionSet.hpp>

namespace smf {
    /// A PercussionSet corresponding to the instruments of XG Room percussion set.
    class XgRoomPercussionSet : public seqwires::PercussionSet {
      public:
        XgRoomPercussionSet(const seqwires::BuiltInPercussionInstruments& builtInInstruments);

        static babelwires::LongIdentifier getThisIdentifier();
    };
} // namespace smf
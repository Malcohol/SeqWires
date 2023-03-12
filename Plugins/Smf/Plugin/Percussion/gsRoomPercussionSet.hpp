/**
 * A PercussionSet corresponding to the instruments of GS Room Percussion Set.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <SeqWiresLib/Percussion/percussionSetWithPitchMap.hpp>

#include <BabelWiresLib/TypeSystem/primitiveType.hpp>

namespace smf {
    /// A PercussionSet corresponding to the instruments of GS standard percussion set.
    class GsRoomPercussionSet : public seqwires::PercussionSetWithPitchMap {
      public:
        PRIMITIVE_TYPE("GSRoomPerc", "Roland GS Room Percussion", "ea4fe75f-71ff-42c7-9480-99486b513d37", 1);
        GsRoomPercussionSet(const seqwires::BuiltInPercussionInstruments& builtInInstruments);
    };
} // namespace smf
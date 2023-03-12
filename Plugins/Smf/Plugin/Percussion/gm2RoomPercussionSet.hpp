/**
 * A PercussionSet corresponding to the instruments of GM2 Room Percussion Set.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <SeqWiresLib/Percussion/percussionSetWithPitchMap.hpp>

#include <BabelWiresLib/TypeSystem/primitiveType.hpp>

namespace smf {
    /// A PercussionSet corresponding to the instruments of GM2 Standard Percussion Set.
    class GM2RoomPercussionSet : public seqwires::PercussionSetWithPitchMap {
      public:
        PRIMITIVE_TYPE("GM2RoomPerc", "General MIDI 2 Room Percussion", "2192afce-6dea-4ca0-a5d9-551f9e852e5e", 1);

        GM2RoomPercussionSet(const seqwires::BuiltInPercussionInstruments& builtInInstruments);
    };
} // namespace smf
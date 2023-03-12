/**
 * A PercussionSet corresponding to the instruments of GM2 Electronic Percussion Set.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <SeqWiresLib/Percussion/percussionSetWithPitchMap.hpp>

#include <BabelWiresLib/TypeSystem/primitiveType.hpp>

namespace smf {
    /// A PercussionSet corresponding to the instruments of GM2 Electronic Percussion Set.
    class GM2ElectronicPercussionSet : public seqwires::PercussionSetWithPitchMap {
      public:
        PRIMITIVE_TYPE("GM2ElectronicPerc", "General MIDI 2 Electronic Percussion", "cd9a66f3-baa8-49f0-844b-c41642e945d5", 1);
        GM2ElectronicPercussionSet(const seqwires::BuiltInPercussionInstruments& builtInInstruments);
    };
} // namespace smf
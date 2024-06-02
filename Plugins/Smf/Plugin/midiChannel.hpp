/**
 *
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <BabelWiresLib/TypeSystem/primitiveType.hpp>
#include <BabelWiresLib/Types/Int/intType.hpp>

namespace smf {
    /// Carries the enum of GM Spec values.
    class MidiChannel : public babelwires::IntType {
      public:
        PRIMITIVE_TYPE("MidiChannel", "Midi Channel", "85f19ba3-f5be-4c81-9632-9227f85baa4e", 1);
        MidiChannel()
            : IntType({1, 16}, 1) {}
    };
} // namespace smf

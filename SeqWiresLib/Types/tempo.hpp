/**
 * Tempo is an IntType for holding a tempo value in bpm.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <BabelWiresLib/Types/Int/intType.hpp>

namespace seqwires {

    /// Tempo is an IntType for holding a tempo value in bpm.
    /// Tempos are limited to integral bpm.
    /// The default tempo is 120.
    class Tempo : public babelwires::IntType {
      public:
        PRIMITIVE_TYPE("tempo", "Tempo", "6ee26c7f-ced6-400d-a927-9464a143b39c", 1);
        Tempo() : babelwires::IntType({0, 255}, 120) {}

        using TempoT = std::uint8_t;
        
        TempoT get(const babelwires::ValueHolder& value) const { 
            return static_cast<TempoT>(value->is<babelwires::IntValue>().get());
        }
    };

} // namespace seqwires

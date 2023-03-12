/**
 * A PercussionSet corresponding to the instruments of XG Electro Percussion Set.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <SeqWiresLib/Percussion/percussionSetWithPitchMap.hpp>

#include <BabelWiresLib/TypeSystem/primitiveType.hpp>

namespace smf {
    /// A PercussionSet corresponding to the instruments of XG Electro percussion set.
    class XgElectroPercussionSet : public seqwires::PercussionSetWithPitchMap {
      public:
        PRIMITIVE_TYPE("XGElectroPerc", "Yamaha XG Electro Percussion", "eb1c03a3-292c-4bc9-b932-ce1c989e02ca", 1);

        XgElectroPercussionSet(const seqwires::BuiltInPercussionInstruments& builtInInstruments);
    };
} // namespace smf
/**
 * A PercussionSet corresponding to the instruments of GS Electronic Percussion Set.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <SeqWiresLib/Percussion/percussionSetWithPitchMap.hpp>

#include <BabelWiresLib/TypeSystem/primitiveType.hpp>

namespace smf {
    /// A PercussionSet corresponding to the instruments of GS Electronic Percussion Set.
    /// This can also serve for the GS Dance set.
    class GsElectronicPercussionSet : public seqwires::PercussionSetWithPitchMap {
      public:
        PRIMITIVE_TYPE("GSElectronicPerc", "Roland GS Electronic Percussion", "9ed6fdd8-f4d3-4944-b24f-a34d85d67fdb",
                       1);

        GsElectronicPercussionSet(const seqwires::BuiltInPercussionInstruments& builtInInstruments);
    };
} // namespace smf
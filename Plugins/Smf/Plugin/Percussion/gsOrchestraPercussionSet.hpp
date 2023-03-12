/**
 * A PercussionSet corresponding to the instruments of GS Orchestra Percussion Set.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <SeqWiresLib/Percussion/percussionSetWithPitchMap.hpp>

#include <BabelWiresLib/TypeSystem/primitiveType.hpp>

namespace smf {
    /// A PercussionSet corresponding to the instruments of GS Orchestra percussion set.
    class GsOrchestraPercussionSet : public seqwires::PercussionSetWithPitchMap {
      public:
        PRIMITIVE_TYPE("GSOrchPerc", "Roland GS Orchestra Percussion", "31152c6f-c782-4c02-9125-d2cbcc49ca7e", 1);

        GsOrchestraPercussionSet(const seqwires::BuiltInPercussionInstruments& builtInInstruments);
    };
} // namespace smf
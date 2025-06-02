/**
 * A PercussionSet corresponding to the instruments of GS 808/909 Percussion Set.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <MusicLib/Percussion/percussionSetWithPitchMap.hpp>

#include <BabelWiresLib/TypeSystem/primitiveType.hpp>

namespace smf {
    /// A PercussionSet corresponding to the instruments of GS 808/909 percussion set.
    class Gs808909PercussionSet : public seqwires::PercussionSetWithPitchMap {
      public:
        PRIMITIVE_TYPE("GS808909Perc", "Roland GS 808/909 Percussion", "fc25e9e7-0462-4581-adb6-924e3aee22be", 1);

        Gs808909PercussionSet(const seqwires::BuiltInPercussionInstruments& builtInInstruments);
    };
} // namespace smf
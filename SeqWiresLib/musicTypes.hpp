/**
 * Some common type definitions for musical data.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <SeqWiresLib/pitchClass.hpp>

#include "BabelWiresLib/Enums/enumWithCppEnum.hpp"

#include "Common/Math/rational.hpp"
#include "Common/types.hpp"

#include <cstdint>
#include <string>

namespace seqwires {

    typedef babelwires::Rational ModelDuration;
    typedef babelwires::Byte Pitch;
    typedef babelwires::Byte Velocity;

    PitchClass::Value pitchToPitchClass(Pitch p);

    std::string pitchToString(Pitch p);
    Pitch stringToPitch(std::string_view s);
    std::string durationToString(ModelDuration d);

    /// The percussion map from GM spec.
    enum PercussionInstrumentMap {
        PERCUSSION_BASS_DRUM_1 = 36,
        PERCUSSION_ELECTRIC_SNARE = 40,
        PERCUSSION_HIGH_MID_TOM = 48,
        PERCUSSION_RIDE_CYMBAL_1 = 51,
        PERCUSSION_CLOSED_HI_HAT = 42
    };

    static constexpr ModelDuration s_maxDuration = std::numeric_limits<seqwires::ModelDuration>::max();
} // namespace seqwires

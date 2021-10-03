/**
 * Some common type definitions for musical data.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include "BabelWiresLib/Enums/enumWithCppEnum.hpp"
#include "Common/Math/rational.hpp"
#include "Common/types.hpp"

#include <string>
#include <cstdint>

namespace seqwires {

    typedef babelwires::Rational ModelDuration;
    typedef babelwires::Byte Pitch;
    typedef babelwires::Byte Velocity;

    /// Note: Not trying to match the representation from XF, because I don't currently know how to transpose those
    /// values.
    enum PitchClass : std::uint8_t {
        PITCH_CLASS_C,
        PITCH_CLASS_C_SHARP,
        PITCH_CLASS_D,
        PITCH_CLASS_D_SHARP,
        PITCH_CLASS_E,
        PITCH_CLASS_F,
        PITCH_CLASS_F_SHARP,
        PITCH_CLASS_G,
        PITCH_CLASS_G_SHARP,
        PITCH_CLASS_A,
        PITCH_CLASS_A_SHARP,
        PITCH_CLASS_B,

        NUM_PITCH_CLASSES
    };

    std::string pitchClassToString(PitchClass p);
    PitchClass pitchToPitchClass(Pitch p);

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

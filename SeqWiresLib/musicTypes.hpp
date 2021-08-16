/**
 * Some common type definitions for musical data.
 * 
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include "Common/Math/rational.hpp"
#include "Common/types.hpp"
#include <string>

#include <cstdint>

namespace seqwires {

    typedef babelwires::Rational ModelDuration;
    typedef babelwires::Byte Pitch;
    typedef babelwires::Byte Velocity;

    /// Defines the assignment of chord types to int values.
    enum ChordType : std::uint8_t {
        CHORD_TYPE_MAJOR,
        CHORD_TYPE_MINOR,
        CHORD_TYPE_DOMINANT_7TH,
        CHORD_TYPE_MINOR_7TH,
        CHORD_TYPE_MAJOR_7TH,
        CHORD_TYPE_MAJOR_6Th,
        CHORD_TYPE_MINOR_6TH,
        CHORD_TYPE_AUGMENTED_5TH,
        CHORD_TYPE_DOMINANT_7TH_FLAT_5TH,
        CHORD_TYPE_DIMINISHED,
        CHORD_TYPE_SUSPENDED_4TH,
        CHORD_TYPE_MAJOR_9TH,
        CHORD_TYPE_MINOR_9TH,

        NUM_CHORD_TYPES
    };

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

    std::string pitchToString(Pitch p);
    Pitch stringToPitch(std::string_view s);
    std::string durationToString(ModelDuration d);
    std::string chordTypeToString(ChordType t);
    std::string pitchClassToString(PitchClass p);

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

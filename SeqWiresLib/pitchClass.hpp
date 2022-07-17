/**
 * Defines a data type for pitch class.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <BabelWiresLib/Enums/enumWithCppEnum.hpp>
#include <Common/Math/rational.hpp>
#include <Common/types.hpp>

#include <cstdint>
#include <string>

/// Note: Not trying to match the representation from XF, because I don't currently know how to transpose those
/// values.
#define PITCH_CLASS_VALUES(X)                                                                                          \
    X(C, "C", "7484e798-52b4-42d0-a521-6dc31e9c3bea")                                                                  \
    X(CSharp, "C#", "2ed22c5e-cb89-4791-b689-f6b301f7679f")                                                            \
    X(D, "D", "9710dcc6-a644-4f28-ac6b-975bf656a004")                                                                  \
    X(DSharp, "D#", "0279e570-3a6b-456a-b20e-38aab3ebfd3f")                                                            \
    X(E, "E", "c4ee1866-976a-4842-b24c-82d753820a82")                                                                  \
    X(F, "F", "4a7fd0f1-914e-45ef-8887-b86a0bc23bd2")                                                                  \
    X(FSharp, "F#", "240d9d71-8dd7-4af1-89c8-f15d7a4ac837")                                                            \
    X(G, "G", "ed4c9566-3464-47e4-b3f2-fc118efba774")                                                                  \
    X(GSharp, "G#", "b0a9cf82-dc9c-4a7e-a38b-06cff90cadb7")                                                            \
    X(A, "A", "de1a1f2c-6bda-4469-a66d-2962c28d2a08")                                                                  \
    X(ASharp, "A#", "0af2d2b4-c553-4f99-8635-906753824c98")                                                            \
    X(B, "B", "ddca8184-107f-463e-adc4-73ced59bb76c")

namespace seqwires {

    class PitchClass : public babelwires::Enum {
      public:
        PitchClass();

        static babelwires::LongIdentifier getThisIdentifier();

        ENUM_DEFINE_CPP_ENUM(PITCH_CLASS_VALUES);

        static std::string valueToString(Value p);
    };

} // namespace seqwires

/**
 * Defines a data type for chords.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include "BabelWiresLib/Enums/enumWithCppEnum.hpp"
#include "SeqWiresLib/musicTypes.hpp"

/// Use the same biolerplate pattern as for Enums, since it is likely I will add an Enum for this eventually.
/// These match the "Chord type" values from the XF Format Specifications v2.01
#define CHORD_TYPE_VALUES(X)                                                                                           \
    X(Maj, "Maj", "1ef34c5c-e8d4-4cac-b189-e4ac2fffefd4")                                                              \
    X(Maj6, "Maj6", "b8290240-fce4-4747-b9e4-07132222d0d0")                                                            \
    X(Maj7, "Maj7", "5e1563bc-5fe8-40e5-977c-e33f9620a129")                                                            \
    X(Maj7se, "Maj7(#11)", "b7b5fa80-1376-408d-baf5-fab17a59b752")                                                     \
    X(Maj9, "Maj(9)", "ab7816b3-970d-4c94-ba4c-04c28994729b")                                                          \
    X(Maj79, "Maj7(9)", "ceba875d-d854-4c51-96ee-bf3cdbd3cb57")                                                        \
    X(Maj69, "Maj6(9)", "eeb85cec-cb05-4474-b33c-c48e91cf9cca")                                                        \
    X(aug, "aug", "430cf87e-5e5f-4075-b4fa-bee132085f8b")                                                              \
    X(min, "min", "59870eb3-9bb9-459e-8f81-601dd2c4d1f4")                                                              \
    X(min6, "min6", "1f1d80ea-f34a-4bc6-a3ec-0984088a502a")                                                            \
    X(min7, "min7", "d7d0154d-799f-4f96-afaf-23cda7de5b20")                                                            \
    X(min7b5, "min7b5", "7fa7e272-25f9-4cc7-afdd-0c4f2dfed16b")                                                        \
    X(min9, "min(9)", "7c12b85b-e490-420f-97de-dad20d31082c")                                                          \
    X(min79, "min7(9)", "82258e7a-49cd-40a1-bf02-069c3cb5875a")                                                        \
    X(min7e, "min7(11)", "a3b6be34-a1f9-4046-8962-a264b335b124")                                                       \
    X(mnMj7, "minMaj7", "1fd06260-acbf-48bf-821c-b9b9d04aa8f3")                                                        \
    X(mnMj79, "minMaj7(9)", "7babdce4-bf2c-4230-84bd-71c10aecb936")                                                    \
    X(dim, "dim", "f95f1002-53e8-4c69-a73d-e51665f713ca")                                                              \
    X(dim7, "dim7", "58bbfa30-d398-4f32-a471-6f839d268679")                                                            \
    X(svth, "7th", "0cfbeb19-5df4-4670-9b1b-9b91fae9d57e")                                                             \
    X(svsus4, "7sus4", "2597226d-7c9e-4fdf-b9fc-f3bdb5c47559")                                                         \
    X(svb5, "7b5", "c2577a55-1ad1-486c-b219-5551c96269e6")                                                             \
    X(sv9, "7(9)", "c5325da7-1b80-40a7-ab08-ab9c6a38102b")                                                             \
    X(svs11, "7(#11)", "047a442a-aace-42ab-8902-ad3d5e3f2b41")                                                         \
    X(sv13, "7(13)", "b5662a4f-02a2-4230-991b-9ce9e5fb72a9")                                                           \
    X(svb9, "7(b9)", "980802bb-eada-4985-9458-f7715cb0ef83")                                                           \
    X(svb13, "7(b13)", "1b369c34-2243-464a-8d4b-92258216be22")                                                         \
    X(svs9, "7(#9)", "d5c917b5-079b-487b-ac0e-ceb33bed5fd6")                                                           \
    X(Mj7aug, "Maj7aug", "8f6b9f0a-9ce5-4b62-9a04-23610f7c7781")                                                       \
    X(svaug, "7aug", "3c848378-1755-4788-bb8c-3f5c49dbcb47")                                                           \
    X(onep8, "1+8", "63f443e4-93d5-4609-b91a-5a6491ac19be")                                                            \
    X(onep5, "1+5", "a513bc99-e978-4699-bc7c-7e30d71f33d0")                                                            \
    X(sus4, "sus4", "ce218a4b-4600-47de-8a77-45a490457ff4")                                                            \
    X(op2p5, "1+2+5", "28875d31-45c8-488f-9cc3-9172c0ad7929")
// I'm guessing the "cc" XF value means "cancel chord", so it doesn't need to be represented.

#define CHORD_TYPE_SELECT_FIRST_ARGUMENT(A, B, C) A,

namespace seqwires {

    typedef babelwires::Rational ModelDuration;
    typedef babelwires::Byte Pitch;
    typedef babelwires::Byte Velocity;

    /// Carries the enum of chord types.
    /// This can become an "Enum" when needed.
    class ChordType {
    public:
        enum class Value : std::uint8_t {
            CHORD_TYPE_VALUES(CHORD_TYPE_SELECT_FIRST_ARGUMENT) NotAChord,
            NUM_CHORD_TYPES = NotAChord
        };
    };

    std::string chordTypeToString(ChordType::Value t);

    /// Defines the state of a chord event.
    // Note: This does not currently include all the data in a XF chord event.
    struct Chord {
        PitchClass m_root = seqwires::PITCH_CLASS_C;
        ChordType::Value m_chordType = seqwires::ChordType::Value::NotAChord;

        bool operator==(Chord other) const {
            return (m_root == other.m_root) && (m_chordType == other.m_chordType);
        }
        
        bool operator!=(Chord other) const {
            return (m_root != other.m_root) || (m_chordType != other.m_chordType);
        }
    };

} // namespace seqwires

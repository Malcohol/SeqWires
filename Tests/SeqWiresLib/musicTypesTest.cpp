#include <gtest/gtest.h>

#include <SeqWiresLib/musicTypes.hpp>

#include <Common/exceptions.hpp>

TEST(SeqWiresLib, enumNames) {
    EXPECT_EQ(seqwires::PitchClass::valueToString(seqwires::PitchClass::Value::C), "C");

    EXPECT_EQ(seqwires::pitchToString(0), "C-1");
    EXPECT_EQ(seqwires::pitchToString(1), "C#-1");
    EXPECT_EQ(seqwires::pitchToString(60), "C4");
    EXPECT_EQ(seqwires::pitchToString(61), "C#4");
    EXPECT_EQ(seqwires::pitchToString(62), "D4");
    EXPECT_EQ(seqwires::pitchToString(63), "D#4");
    EXPECT_EQ(seqwires::pitchToString(64), "E4");
    EXPECT_EQ(seqwires::pitchToString(65), "F4");
    EXPECT_EQ(seqwires::pitchToString(66), "F#4");
    EXPECT_EQ(seqwires::pitchToString(67), "G4");
    EXPECT_EQ(seqwires::pitchToString(68), "G#4");
    EXPECT_EQ(seqwires::pitchToString(69), "A4");
    EXPECT_EQ(seqwires::pitchToString(70), "A#4");
    EXPECT_EQ(seqwires::pitchToString(71), "B4");
    EXPECT_EQ(seqwires::pitchToString(72), "C5");
    EXPECT_EQ(seqwires::pitchToString(127), "G9");

    EXPECT_EQ(seqwires::stringToPitch("C-1"), 0);
    EXPECT_EQ(seqwires::stringToPitch("C#-1"), 1);
    EXPECT_EQ(seqwires::stringToPitch("C4"), 60);
    EXPECT_EQ(seqwires::stringToPitch("C#4"), 61);
    EXPECT_EQ(seqwires::stringToPitch("D4"), 62);
    EXPECT_EQ(seqwires::stringToPitch("D#4"), 63);
    EXPECT_EQ(seqwires::stringToPitch("E4"), 64);
    EXPECT_EQ(seqwires::stringToPitch("F4"), 65);
    EXPECT_EQ(seqwires::stringToPitch("F#4"), 66);
    EXPECT_EQ(seqwires::stringToPitch("G4"), 67);
    EXPECT_EQ(seqwires::stringToPitch("G#4"), 68);
    EXPECT_EQ(seqwires::stringToPitch("A4"), 69);
    EXPECT_EQ(seqwires::stringToPitch("A#4"), 70);
    EXPECT_EQ(seqwires::stringToPitch("B4"), 71);
    EXPECT_EQ(seqwires::stringToPitch("C5"), 72);
    EXPECT_EQ(seqwires::stringToPitch("G9"), 127);

    EXPECT_EQ(seqwires::stringToPitch("  C-1  "), 0);

    EXPECT_THROW(seqwires::stringToPitch("G#9"), babelwires::ParseException);
    EXPECT_THROW(seqwires::stringToPitch("A9"), babelwires::ParseException);
    EXPECT_THROW(seqwires::stringToPitch("A#9"), babelwires::ParseException);
    EXPECT_THROW(seqwires::stringToPitch("B9"), babelwires::ParseException);
    EXPECT_THROW(seqwires::stringToPitch(""), babelwires::ParseException);
    EXPECT_THROW(seqwires::stringToPitch("  "), babelwires::ParseException);
    EXPECT_THROW(seqwires::stringToPitch("C 4"), babelwires::ParseException);
    EXPECT_THROW(seqwires::stringToPitch("C #4"), babelwires::ParseException);
    EXPECT_THROW(seqwires::stringToPitch("C# 4"), babelwires::ParseException);
    EXPECT_THROW(seqwires::stringToPitch("Z2"), babelwires::ParseException);
    EXPECT_THROW(seqwires::stringToPitch("DA"), babelwires::ParseException);
    EXPECT_THROW(seqwires::stringToPitch("C##2"), babelwires::ParseException);
    EXPECT_THROW(seqwires::stringToPitch("G-2"), babelwires::ParseException);
    EXPECT_THROW(seqwires::stringToPitch("C10"), babelwires::ParseException);
    EXPECT_THROW(seqwires::stringToPitch("C-"), babelwires::ParseException);
}

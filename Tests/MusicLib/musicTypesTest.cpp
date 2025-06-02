#include <gtest/gtest.h>

#include <MusicLib/musicTypes.hpp>

#include <Common/exceptions.hpp>

TEST(MusicLib, enumNames) {
    EXPECT_EQ(seqwires::PitchClass::valueToString(seqwires::PitchClass::Value::C), "C");

    EXPECT_EQ(seqwires::pitchToString(0), "C0");
    EXPECT_EQ(seqwires::pitchToString(1), "C#0");
    EXPECT_EQ(seqwires::pitchToString(60), "C5");
    EXPECT_EQ(seqwires::pitchToString(61), "C#5");
    EXPECT_EQ(seqwires::pitchToString(62), "D5");
    EXPECT_EQ(seqwires::pitchToString(63), "D#5");
    EXPECT_EQ(seqwires::pitchToString(64), "E5");
    EXPECT_EQ(seqwires::pitchToString(65), "F5");
    EXPECT_EQ(seqwires::pitchToString(66), "F#5");
    EXPECT_EQ(seqwires::pitchToString(67), "G5");
    EXPECT_EQ(seqwires::pitchToString(68), "G#5");
    EXPECT_EQ(seqwires::pitchToString(69), "A5");
    EXPECT_EQ(seqwires::pitchToString(70), "A#5");
    EXPECT_EQ(seqwires::pitchToString(71), "B5");
    EXPECT_EQ(seqwires::pitchToString(72), "C6");
    EXPECT_EQ(seqwires::pitchToString(127), "G10");

    EXPECT_EQ(seqwires::stringToPitch("C0"), 0);
    EXPECT_EQ(seqwires::stringToPitch("C#0"), 1);
    EXPECT_EQ(seqwires::stringToPitch("C5"), 60);
    EXPECT_EQ(seqwires::stringToPitch("C#5"), 61);
    EXPECT_EQ(seqwires::stringToPitch("D5"), 62);
    EXPECT_EQ(seqwires::stringToPitch("D#5"), 63);
    EXPECT_EQ(seqwires::stringToPitch("E5"), 64);
    EXPECT_EQ(seqwires::stringToPitch("F5"), 65);
    EXPECT_EQ(seqwires::stringToPitch("F#5"), 66);
    EXPECT_EQ(seqwires::stringToPitch("G5"), 67);
    EXPECT_EQ(seqwires::stringToPitch("G#5"), 68);
    EXPECT_EQ(seqwires::stringToPitch("A5"), 69);
    EXPECT_EQ(seqwires::stringToPitch("A#5"), 70);
    EXPECT_EQ(seqwires::stringToPitch("B5"), 71);
    EXPECT_EQ(seqwires::stringToPitch("C6"), 72);
    EXPECT_EQ(seqwires::stringToPitch("G10"), 127);

    EXPECT_EQ(seqwires::stringToPitch("  C0  "), 0);

    EXPECT_THROW(seqwires::stringToPitch("G#10"), babelwires::ParseException);
    EXPECT_THROW(seqwires::stringToPitch("A10"), babelwires::ParseException);
    EXPECT_THROW(seqwires::stringToPitch("A#10"), babelwires::ParseException);
    EXPECT_THROW(seqwires::stringToPitch("B10"), babelwires::ParseException);
    EXPECT_THROW(seqwires::stringToPitch(""), babelwires::ParseException);
    EXPECT_THROW(seqwires::stringToPitch("  "), babelwires::ParseException);
    EXPECT_THROW(seqwires::stringToPitch("C 4"), babelwires::ParseException);
    EXPECT_THROW(seqwires::stringToPitch("C #4"), babelwires::ParseException);
    EXPECT_THROW(seqwires::stringToPitch("C# 4"), babelwires::ParseException);
    EXPECT_THROW(seqwires::stringToPitch("Z2"), babelwires::ParseException);
    EXPECT_THROW(seqwires::stringToPitch("DA"), babelwires::ParseException);
    EXPECT_THROW(seqwires::stringToPitch("C##2"), babelwires::ParseException);
    EXPECT_THROW(seqwires::stringToPitch("G-2"), babelwires::ParseException);
    EXPECT_THROW(seqwires::stringToPitch("C11"), babelwires::ParseException);
    EXPECT_THROW(seqwires::stringToPitch("C-1"), babelwires::ParseException);
}

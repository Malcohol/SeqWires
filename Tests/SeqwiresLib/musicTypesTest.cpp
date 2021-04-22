#include <gtest/gtest.h>

#include "SeqwiresLib/musicTypes.hpp"

TEST(SeqwiresLib, enumNames) {
    EXPECT_EQ(seqwires::pitchClassToString(seqwires::PITCH_CLASS_C), "C");
}

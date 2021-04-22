#include <gtest/gtest.h>

#include "SeqWiresLib/musicTypes.hpp"

TEST(SeqWiresLib, enumNames) {
    EXPECT_EQ(seqwires::pitchClassToString(seqwires::PITCH_CLASS_C), "C");
}

#include <gtest/gtest.h>

#include <SeqWiresLib/Functions/repeatFunction.hpp>
#include <SeqWiresLib/Tracks/noteEvents.hpp>

#include <Tests/TestUtils/seqTestUtils.hpp>

TEST(RepeatProcessorTest, funcSimpleZero) {
    seqwires::Track trackIn;

    testUtils::addSimpleNotes(std::vector<seqwires::Pitch>{60, 62, 64, 65}, trackIn);

    auto trackOut = seqwires::repeatTrack(trackIn, 0);

    EXPECT_EQ(trackOut->getNumEvents(), 0);
    EXPECT_EQ(trackOut->getDuration(), 0);
}

TEST(RepeatProcessorTest, funcSimpleOnce) {
    seqwires::Track trackIn;

    testUtils::addSimpleNotes(std::vector<seqwires::Pitch>{60, 62, 64, 65}, trackIn);

    auto trackOut = seqwires::repeatTrack(trackIn, 1);

    testUtils::testSimpleNotes(std::vector<seqwires::Pitch>{60, 62, 64, 65}, *trackOut);
}

TEST(RepeatProcessorTest, funcSimpleTwice) {
    seqwires::Track trackIn;

    testUtils::addSimpleNotes(std::vector<seqwires::Pitch>{60, 62, 64, 65}, trackIn);

    auto trackOut = seqwires::repeatTrack(trackIn, 2);

    testUtils::testSimpleNotes(std::vector<seqwires::Pitch>{60, 62, 64, 65, 60, 62, 64, 65}, *trackOut);
}


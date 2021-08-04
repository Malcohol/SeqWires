#include <gtest/gtest.h>

#include <SeqWiresLib/Functions/excerptFunction.hpp>

#include <Tests/TestUtils/seqTestUtils.hpp>

TEST(ExcerptProcessorTest, testExcerptFunctionSimple) {
    seqwires::Track trackIn;

    testUtils::addSimpleNotes(std::vector<seqwires::Pitch>{60, 62, 64, 65, 67, 69, 71, 72}, trackIn);

    auto trackOut = seqwires::getTrackExcerpt(trackIn, babelwires::Rational(1, 2), 1);

    testUtils::testSimpleNotes(std::vector<seqwires::Pitch>{64, 65, 67, 69}, *trackOut);
}

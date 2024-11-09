#include <gtest/gtest.h>

#include <BabelWiresLib/ValueTree/valueTreeRoot.hpp>

#include <SeqWiresLib/Functions/appendTrackFunction.hpp>
#include <SeqWiresLib/Processors/concatenateProcessor.hpp>
#include <SeqWiresLib/Types/Track/TrackEvents/noteEvents.hpp>
#include <SeqWiresLib/libRegistration.hpp>

#include <Tests/BabelWiresLib/TestUtils/testEnvironment.hpp>
#include <Tests/TestUtils/seqTestUtils.hpp>

TEST(ConcatenateProcessorTest, appendFuncSimple) {
    seqwires::Track trackA;
    testUtils::addSimpleNotes(std::vector<seqwires::Pitch>{60, 62, 64, 65}, trackA);

    seqwires::Track trackB;
    testUtils::addSimpleNotes(std::vector<seqwires::Pitch>{67, 69, 71, 72}, trackB);

    appendTrack(trackA, trackB);

    testUtils::testSimpleNotes(std::vector<seqwires::Pitch>{60, 62, 64, 65, 67, 69, 71, 72}, trackA);
}

TEST(ConcatenateProcessorTest, appendFuncGaps) {
    seqwires::Track trackA;
    const std::vector<testUtils::NoteInfo> noteInfosA{{60, 1, babelwires::Rational(1, 4)},
                                                      {62, 0, babelwires::Rational(1, 4)},
                                                      {64, 0, babelwires::Rational(1, 4)},
                                                      {65, 0, babelwires::Rational(1, 4)}};
    testUtils::addNotes(noteInfosA, trackA);
    trackA.setDuration(3);

    seqwires::Track trackB;
    const std::vector<testUtils::NoteInfo> noteInfosB{
        {67, 1, babelwires::Rational(1, 4)},
        {69, 0, babelwires::Rational(1, 4)},
        {71, 0, babelwires::Rational(1, 4)},
        {72, 0, babelwires::Rational(1, 4)},
    };
    testUtils::addNotes(noteInfosB, trackB);
    trackB.setDuration(3);

    appendTrack(trackA, trackB);

    EXPECT_EQ(trackA.getDuration(), 6);

    const std::vector<testUtils::NoteInfo> expectedNoteInfos{
        {60, 1, babelwires::Rational(1, 4)}, {62, 0, babelwires::Rational(1, 4)}, {64, 0, babelwires::Rational(1, 4)},
        {65, 0, babelwires::Rational(1, 4)}, {67, 2, babelwires::Rational(1, 4)}, {69, 0, babelwires::Rational(1, 4)},
        {71, 0, babelwires::Rational(1, 4)}, {72, 0, babelwires::Rational(1, 4)},
    };

    testUtils::testNotes(expectedNoteInfos, trackA);
}

TEST(ConcatenateProcessorTest, processor) {
    testUtils::TestEnvironment testEnvironment;
    seqwires::registerLib(testEnvironment.m_projectContext);

    seqwires::ConcatenateProcessor processor(testEnvironment.m_projectContext);

    processor.getInput().setToDefault();
    processor.getOutput().setToDefault();

    auto input = seqwires::ConcatenateProcessorInput::Instance(processor.getInput());
    const auto output = seqwires::ConcatenateProcessorOutput::ConstInstance(processor.getOutput());

    ASSERT_EQ(input.getInput().getSize(), 2);
    EXPECT_EQ(input.getInput().getEntry(0).get().getDuration(), 0);
    EXPECT_EQ(input.getInput().getEntry(1).get().getDuration(), 0);

    {
        seqwires::Track track;
        testUtils::addSimpleNotes(std::vector<seqwires::Pitch>{60, 62, 64, 65}, track);
        input.getInput().getEntry(0).set(std::move(track));
    }

    processor.process(testEnvironment.m_log);

    testUtils::testSimpleNotes(std::vector<seqwires::Pitch>{60, 62, 64, 65}, output.getOutput().get());

    processor.getInput().clearChanges();
    {
        seqwires::Track track;
        testUtils::addSimpleNotes(std::vector<seqwires::Pitch>{67, 69, 71, 72}, track);
        input.getInput().getEntry(1).set(std::move(track));
    }
    processor.process(testEnvironment.m_log);

    testUtils::testSimpleNotes(std::vector<seqwires::Pitch>{60, 62, 64, 65, 67, 69, 71, 72}, output.getOutput().get());

    processor.getInput().clearChanges();
    // Insert a new track at position 1.
    {
        input.getInput().setSize(3);
        input.getInput().getEntry(2).set(input.getInput().getEntry(1)->getValue());
        {
            seqwires::Track track;
            testUtils::addSimpleNotes(std::vector<seqwires::Pitch>{67, 65}, track);
            input.getInput().getEntry(1).set(std::move(track));
        }
    }
    processor.process(testEnvironment.m_log);

    testUtils::testSimpleNotes(std::vector<seqwires::Pitch>{60, 62, 64, 65, 67, 65, 67, 69, 71, 72},
                               output.getOutput().get());
}

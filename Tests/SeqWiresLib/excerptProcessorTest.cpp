#include <gtest/gtest.h>

#include <BabelWiresLib/ValueTree/valueTreeRoot.hpp>

#include <SeqWiresLib/Functions/excerptFunction.hpp>
#include <SeqWiresLib/Processors/excerptProcessor.hpp>
#include <SeqWiresLib/Types/Track/TrackEvents/noteEvents.hpp>
#include <SeqWiresLib/libRegistration.hpp>
#include <SeqWiresLib/Types/Track/trackInstance.hpp>

#include <Tests/BabelWiresLib/TestUtils/testEnvironment.hpp>
#include <Tests/TestUtils/seqTestUtils.hpp>

TEST(ExcerptProcessorTest, funcSimple) {
    seqwires::Track trackIn;

    testUtils::addSimpleNotes(std::vector<seqwires::Pitch>{60, 62, 64, 65, 67, 69, 71, 72}, trackIn);

    auto trackOut = seqwires::getTrackExcerpt(trackIn, babelwires::Rational(1, 2), 1);

    testUtils::testSimpleNotes(std::vector<seqwires::Pitch>{64, 65, 67, 69}, trackOut);
}

TEST(ExcerptProcessorTest, funcEmptyBefore) {
    seqwires::Track trackIn;
    seqwires::NoteOnEvent note;
    note.m_pitch = 64;
    note.m_velocity = 100;
    note.setTimeSinceLastEvent(10);
    trackIn.addEvent(note);

    auto trackOut = seqwires::getTrackExcerpt(trackIn, 3, 4);

    EXPECT_EQ(trackOut.getDuration(), 4);
    EXPECT_EQ(trackOut.getNumEvents(), 0);
}

TEST(ExcerptProcessorTest, funcEmptyAfter) {
    seqwires::Track trackIn;

    auto trackOut = seqwires::getTrackExcerpt(trackIn, 2, 2);

    EXPECT_EQ(trackOut.getDuration(), 2);
    EXPECT_EQ(trackOut.getNumEvents(), 0);
}

TEST(ExcerptProcessorTest, funcEmptyBetween) {
    seqwires::Track trackIn;
    seqwires::NoteOnEvent noteOn;
    noteOn.m_pitch = 64;
    noteOn.m_velocity = 100;
    noteOn.setTimeSinceLastEvent(1);
    trackIn.addEvent(noteOn);
    seqwires::NoteOffEvent noteOff;
    noteOff.m_pitch = 64;
    noteOff.m_velocity = 100;
    noteOff.setTimeSinceLastEvent(10);
    trackIn.addEvent(noteOff);

    auto trackOut = seqwires::getTrackExcerpt(trackIn, 2, 2);

    EXPECT_EQ(trackOut.getDuration(), 2);
    EXPECT_EQ(trackOut.getNumEvents(), 0);
}

TEST(ExcerptProcessorTest, funcDropSpanningGroup) {
    seqwires::Track trackIn;
    seqwires::NoteOnEvent noteOn;
    noteOn.m_pitch = 40;
    noteOn.m_velocity = 100;
    noteOn.setTimeSinceLastEvent(1);
    trackIn.addEvent(noteOn);
    testUtils::addSimpleNotes(std::vector<seqwires::Pitch>{60, 62, 64, 65, 67, 69, 71, 72}, trackIn);
    seqwires::NoteOffEvent noteOff;
    noteOff.m_pitch = 40;
    noteOff.m_velocity = 100;
    noteOff.setTimeSinceLastEvent(1);
    trackIn.addEvent(noteOff);

    auto trackOut = seqwires::getTrackExcerpt(trackIn, babelwires::Rational(3, 2), 1);

    testUtils::testSimpleNotes(std::vector<seqwires::Pitch>{64, 65, 67, 69}, trackOut);
}

TEST(ExcerptProcessorTest, funcDropInitialGroup) {
    seqwires::Track trackIn;

    seqwires::NoteOnEvent noteOn;
    noteOn.m_pitch = 40;
    noteOn.m_velocity = 100;
    noteOn.setTimeSinceLastEvent(1);
    trackIn.addEvent(noteOn);
    testUtils::addSimpleNotes(std::vector<seqwires::Pitch>{60, 62, 64, 65}, trackIn);

    seqwires::NoteOffEvent noteOff;
    noteOff.m_pitch = 40;
    noteOff.m_velocity = 100;
    noteOff.setTimeSinceLastEvent(0);
    trackIn.addEvent(noteOff);

    testUtils::addSimpleNotes(std::vector<seqwires::Pitch>{67, 69, 71, 72}, trackIn);

    auto trackOut = seqwires::getTrackExcerpt(trackIn, babelwires::Rational(3, 2), 1);

    testUtils::testSimpleNotes(std::vector<seqwires::Pitch>{64, 65, 67, 69}, trackOut);
}

TEST(ExcerptProcessorTest, funcGaps) {
    seqwires::Track trackIn;

    testUtils::addSimpleNotes(std::vector<seqwires::Pitch>{60, 62, 64, 65, 67, 69, 71, 72}, trackIn);

    auto trackOut = seqwires::getTrackExcerpt(trackIn, babelwires::Rational(3, 8), 1);

    const std::vector<testUtils::NoteInfo> expectedNoteInfos{
        {64, babelwires::Rational(1, 8), babelwires::Rational(1, 4)},
        {65, 0, babelwires::Rational(1, 4)},
        {67, 0, babelwires::Rational(1, 4)},
        {69, 0, babelwires::Rational(1, 8)}};

    testUtils::testNotes(expectedNoteInfos, trackOut);
}

TEST(ExcerptProcessorTest, processor) {
    testUtils::TestEnvironment testEnvironment;
    seqwires::registerLib(testEnvironment.m_projectContext);

    seqwires::ExcerptProcessor processor(testEnvironment.m_projectContext);

    processor.getInput().setToDefault();
    processor.getOutput().setToDefault();

    babelwires::ValueTreeNode& input = processor.getInput();
    const babelwires::ValueTreeNode& output = processor.getOutput();

    babelwires::ValueTreeNode& inputArray =
        input.getChildFromStep(babelwires::PathStep(seqwires::ExcerptProcessor::getCommonArrayId()))
            .is<babelwires::ValueTreeNode>();
    const babelwires::ValueTreeNode& outputArray =
        output.getChildFromStep(babelwires::PathStep(seqwires::ExcerptProcessor::getCommonArrayId()))
            .is<babelwires::ValueTreeNode>();

    babelwires::ArrayInstanceImpl<babelwires::ValueTreeNode, seqwires::TrackType> inArray(inputArray);
    const babelwires::ArrayInstanceImpl<const babelwires::ValueTreeNode, seqwires::TrackType> outArray(outputArray);

    seqwires::ExcerptProcessorInput::Instance in(input);

    EXPECT_EQ(inArray.getSize(), 1);
    EXPECT_EQ(outArray.getSize(), 1);

    EXPECT_EQ(inArray.getEntry(0).get().getDuration(), 0);
    EXPECT_EQ(outArray.getEntry(0).get().getDuration(), 0);

    in.getDuratn().set(1);

    processor.process(testEnvironment.m_log);

    EXPECT_EQ(outArray.getEntry(0).get().getDuration(), 1);
    EXPECT_EQ(outArray.getEntry(0).get().getNumEvents(), 0);

    processor.getInput().clearChanges();
    {
        babelwires::BackupScope scope(processor.getInput().is<babelwires::ValueTreeRoot>());
        seqwires::Track track;
        testUtils::addSimpleNotes({60, 62, 64, 65, 67, 69, 71, 72}, track);
        inArray.getEntry(0).set(std::move(track));
    }
    processor.process(testEnvironment.m_log);

    EXPECT_EQ(outArray.getEntry(0).get().getDuration(), 1);
    testUtils::testSimpleNotes(std::vector<seqwires::Pitch>{60, 62, 64, 65}, outArray.getEntry(0).get());

    processor.getInput().clearChanges();
    {
        babelwires::BackupScope scope(processor.getInput().is<babelwires::ValueTreeRoot>());
        in.getStart().set(1);
    }
    processor.process(testEnvironment.m_log);

    EXPECT_EQ(outArray.getEntry(0).get().getDuration(), 1);
    testUtils::testSimpleNotes(std::vector<seqwires::Pitch>{67, 69, 71, 72}, outArray.getEntry(0).get());

    //TODO: Would like this test to confirm that adding a track entry does not cause existing entries to be changed.
    // However that functionality didn't survive the switch to types/values.
    processor.getInput().clearChanges();
    {
        babelwires::BackupScope scope(processor.getInput().is<babelwires::ValueTreeRoot>());
        inArray.setSize(2);
        {
            seqwires::Track track;
            testUtils::addSimpleNotes(std::vector<seqwires::Pitch>{48, 50, 52, 53, 55, 57, 59, 60}, track);
            inArray.getEntry(1).set(std::move(track));
        }
    }
    processor.process(testEnvironment.m_log);

    ASSERT_EQ(outArray->getNumChildren(), 2);
    EXPECT_EQ(outArray.getEntry(0).get().getDuration(), 1);
    EXPECT_EQ(outArray.getEntry(1).get().getDuration(), 1);
    testUtils::testSimpleNotes(std::vector<seqwires::Pitch>{67, 69, 71, 72}, outArray.getEntry(0).get());
    testUtils::testSimpleNotes(std::vector<seqwires::Pitch>{55, 57, 59, 60}, outArray.getEntry(1).get());

    // Check that input changes to an entry only affect its output entry. For this test, we clear the output changes too.
    processor.getInput().clearChanges();
    processor.getOutput().clearChanges();
    {
        babelwires::BackupScope scope(processor.getInput().is<babelwires::ValueTreeRoot>());
        // Set it to empty.
        inArray.getEntry(0).set(seqwires::Track());
    }
    processor.process(testEnvironment.m_log);

    ASSERT_EQ(outArray->getNumChildren(), 2);
    EXPECT_EQ(outArray.getEntry(0).get().getDuration(), 1);
    EXPECT_EQ(outArray.getEntry(1).get().getDuration(), 1);
    EXPECT_EQ(outArray.getEntry(0).get().getNumEvents(), 0);

    // The other entry is untouched.
    EXPECT_FALSE(outArray.getEntry(1)->isChanged(babelwires::ValueTreeNode::Changes::SomethingChanged));
    testUtils::testSimpleNotes(std::vector<seqwires::Pitch>{55, 57, 59, 60}, outArray.getEntry(1).get());
}

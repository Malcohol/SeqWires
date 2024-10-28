#include <gtest/gtest.h>

#include <BabelWiresLib/ValueTree/valueTreeRoot.hpp>

#include <SeqWiresLib/Functions/transposeFunction.hpp>
#include <SeqWiresLib/Processors/transposeProcessor.hpp>
#include <SeqWiresLib/Types/Track/TrackEvents/noteEvents.hpp>
#include <SeqWiresLib/Types/Track/trackInstance.hpp>
#include <SeqWiresLib/libRegistration.hpp>

#include <Tests/BabelWiresLib/TestUtils/testEnvironment.hpp>
#include <Tests/TestUtils/seqTestUtils.hpp>

TEST(TransposeProcessorTest, funcSimpleZero) {
    seqwires::Track trackIn;

    testUtils::addSimpleNotes(std::vector<seqwires::Pitch>{60, 62, 64, 65}, trackIn);

    auto trackOut = seqwires::transposeTrack(trackIn, 0);

    testUtils::testSimpleNotes(std::vector<seqwires::Pitch>{60, 62, 64, 65}, trackOut);
}

TEST(TransposeProcessorTest, funcSimplePositive) {
    seqwires::Track trackIn;

    testUtils::addSimpleNotes(std::vector<seqwires::Pitch>{60, 62, 64, 65}, trackIn);

    auto trackOut = seqwires::transposeTrack(trackIn, 1);

    testUtils::testSimpleNotes(std::vector<seqwires::Pitch>{61, 63, 65, 66}, trackOut);
}

TEST(TransposeProcessorTest, funcSimpleNegative) {
    seqwires::Track trackIn;

    testUtils::addSimpleNotes(std::vector<seqwires::Pitch>{60, 62, 64, 65}, trackIn);

    auto trackOut = seqwires::transposeTrack(trackIn, -1);

    testUtils::testSimpleNotes(std::vector<seqwires::Pitch>{59, 61, 63, 64}, trackOut);
}

TEST(TransposeProcessorTest, funcSimplePositiveLimit) {
    seqwires::Track trackIn;

    testUtils::addSimpleNotes(std::vector<seqwires::Pitch>{60, 62, 64, 65}, trackIn);

    auto trackOut = seqwires::transposeTrack(trackIn, 127);

    testUtils::testSimpleNotes(std::vector<seqwires::Pitch>{127, 127, 127, 127}, trackOut);
}

TEST(TransposeProcessorTest, funcSimpleNegativeLimit) {
    seqwires::Track trackIn;

    testUtils::addSimpleNotes(std::vector<seqwires::Pitch>{60, 62, 64, 65}, trackIn);

    auto trackOut = seqwires::transposeTrack(trackIn, -127);

    testUtils::testSimpleNotes(std::vector<seqwires::Pitch>{0, 0, 0, 0}, trackOut);
}

TEST(TransposeProcessorTest, funcSimpleChordsZero) {
    seqwires::Track trackIn;

    // Some random chords.
    testUtils::addChords(
        {
            {seqwires::PitchClass::PitchClass::Value::C, seqwires::ChordType::ChordType::Value::M},
            {seqwires::PitchClass::PitchClass::Value::E, seqwires::ChordType::ChordType::Value::m},
            {seqwires::PitchClass::PitchClass::Value::Gsh, seqwires::ChordType::ChordType::Value::dim},
            {seqwires::PitchClass::PitchClass::Value::A, seqwires::ChordType::ChordType::Value::aug},
        },
        trackIn);

    auto trackOut = seqwires::transposeTrack(trackIn, 0);

    testUtils::testChords(
        {
            {seqwires::PitchClass::PitchClass::Value::C, seqwires::ChordType::ChordType::Value::M},
            {seqwires::PitchClass::PitchClass::Value::E, seqwires::ChordType::ChordType::Value::m},
            {seqwires::PitchClass::PitchClass::Value::Gsh, seqwires::ChordType::ChordType::Value::dim},
            {seqwires::PitchClass::PitchClass::Value::A, seqwires::ChordType::ChordType::Value::aug},
        },
        trackIn);
}

TEST(TransposeProcessorTest, funcSimpleChordsPositive) {
    seqwires::Track trackIn;

    // Some random chords.
    testUtils::addChords(
        {
            {seqwires::PitchClass::PitchClass::Value::C, seqwires::ChordType::ChordType::Value::M},
            {seqwires::PitchClass::PitchClass::Value::E, seqwires::ChordType::ChordType::Value::m},
            {seqwires::PitchClass::PitchClass::Value::Gsh, seqwires::ChordType::ChordType::Value::dim},
            {seqwires::PitchClass::PitchClass::Value::B, seqwires::ChordType::ChordType::Value::aug},
        },
        trackIn);

    auto trackOut = seqwires::transposeTrack(trackIn, 1);

    testUtils::testChords(
        {
            {seqwires::PitchClass::PitchClass::Value::Csh, seqwires::ChordType::ChordType::Value::M},
            {seqwires::PitchClass::PitchClass::Value::F, seqwires::ChordType::ChordType::Value::m},
            {seqwires::PitchClass::PitchClass::Value::A, seqwires::ChordType::ChordType::Value::dim},
            {seqwires::PitchClass::PitchClass::Value::C, seqwires::ChordType::ChordType::Value::aug},
        },
        trackOut);
}

TEST(TransposeProcessorTest, funcSimpleChordsNegative) {
    seqwires::Track trackIn;

    // Some random chords.
    testUtils::addChords(
        {
            {seqwires::PitchClass::PitchClass::Value::C, seqwires::ChordType::ChordType::Value::M},
            {seqwires::PitchClass::PitchClass::Value::E, seqwires::ChordType::ChordType::Value::m},
            {seqwires::PitchClass::PitchClass::Value::Gsh, seqwires::ChordType::ChordType::Value::dim},
            {seqwires::PitchClass::PitchClass::Value::B, seqwires::ChordType::ChordType::Value::aug},
        },
        trackIn);

    auto trackOut = seqwires::transposeTrack(trackIn, -1);

    testUtils::testChords(
        {
            {seqwires::PitchClass::PitchClass::Value::B, seqwires::ChordType::ChordType::Value::M},
            {seqwires::PitchClass::PitchClass::Value::Dsh, seqwires::ChordType::ChordType::Value::m},
            {seqwires::PitchClass::PitchClass::Value::G, seqwires::ChordType::ChordType::Value::dim},
            {seqwires::PitchClass::PitchClass::Value::Ash, seqwires::ChordType::ChordType::Value::aug},
        },
        trackOut);
}

TEST(TransposeProcessorTest, processor) {
    testUtils::TestEnvironment testEnvironment;
    seqwires::registerLib(testEnvironment.m_projectContext);

    seqwires::TransposeProcessor processor(testEnvironment.m_projectContext);

    processor.getInput().setToDefault();
    processor.getOutput().setToDefault();

    babelwires::ValueTreeNode& input = processor.getInput();
    const babelwires::ValueTreeNode& output = processor.getOutput();

    babelwires::ValueTreeNode& inputArray =
        input.getChildFromStep(babelwires::PathStep(seqwires::TransposeProcessor::getCommonArrayId()))
            .is<babelwires::ValueTreeNode>();
    const babelwires::ValueTreeNode& outputArray =
        output.getChildFromStep(babelwires::PathStep(seqwires::TransposeProcessor::getCommonArrayId()))
            .is<babelwires::ValueTreeNode>();

    babelwires::ArrayInstanceImpl<babelwires::ValueTreeNode, seqwires::TrackType> inArray(inputArray);
    const babelwires::ArrayInstanceImpl<const babelwires::ValueTreeNode, seqwires::TrackType> outArray(
        outputArray);

    seqwires::TransposeProcessorInput::Instance in(input);

    EXPECT_EQ(in.getOffset().get(), 0);

    EXPECT_EQ(inArray.getSize(), 1);
    EXPECT_EQ(outArray.getSize(), 1);

    EXPECT_EQ(inArray.getEntry(0).get().getDuration(), 0);
    EXPECT_EQ(outArray.getEntry(0).get().getDuration(), 0);

    {
        babelwires::BackupScope scope(processor.getInput().is<babelwires::ValueTreeRoot>());
        seqwires::Track track;
        testUtils::addSimpleNotes({60, 62, 64, 65}, track);
        inArray.getEntry(0).set(std::move(track));
    }
    processor.process(testEnvironment.m_log);
    testUtils::testSimpleNotes({60, 62, 64, 65}, outArray.getEntry(0).get());

    processor.getInput().clearChanges();
    {
        babelwires::BackupScope scope(processor.getInput().is<babelwires::ValueTreeRoot>());
        in.getOffset().set(1);
    }
    processor.process(testEnvironment.m_log);
    testUtils::testSimpleNotes({61, 63, 65, 66}, outArray.getEntry(0).get());

    processor.getInput().clearChanges();
    {
        babelwires::BackupScope scope(processor.getInput().is<babelwires::ValueTreeRoot>());
        inArray.setSize(2);
        {
            seqwires::Track track;
            testUtils::addSimpleNotes(std::vector<seqwires::Pitch>{48, 50, 52, 53}, track);
            inArray.getEntry(1).set(std::move(track));
        }
    }
    processor.process(testEnvironment.m_log);

    ASSERT_EQ(outArray.getSize(), 2);
    EXPECT_EQ(outArray.getEntry(0).get().getDuration(), 1);
    EXPECT_EQ(outArray.getEntry(1).get().getDuration(), 1);
    testUtils::testSimpleNotes(std::vector<seqwires::Pitch>{61, 63, 65, 66}, outArray.getEntry(0).get());
    testUtils::testSimpleNotes(std::vector<seqwires::Pitch>{49, 51, 53, 54}, outArray.getEntry(1).get());
}
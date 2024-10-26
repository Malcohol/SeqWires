#include <gtest/gtest.h>

#include <BabelWiresLib/Features/valueTreeRoot.hpp>

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

    processor.getInputFeature().setToDefault();
    processor.getOutputFeature().setToDefault();

    babelwires::ValueTreeNode& inputValueFeature = processor.getInputFeature();
    const babelwires::ValueTreeNode& outputValueFeature = processor.getOutputFeature();

    babelwires::ValueTreeNode& inputArrayFeature =
        inputValueFeature.getChildFromStep(babelwires::PathStep(seqwires::TransposeProcessor::getCommonArrayId()))
            .is<babelwires::ValueTreeNode>();
    const babelwires::ValueTreeNode& outputArrayFeature =
        outputValueFeature.getChildFromStep(babelwires::PathStep(seqwires::TransposeProcessor::getCommonArrayId()))
            .is<babelwires::ValueTreeNode>();

    babelwires::ArrayInstanceImpl<babelwires::ValueTreeNode, seqwires::TrackType> inputArray(inputArrayFeature);
    const babelwires::ArrayInstanceImpl<const babelwires::ValueTreeNode, seqwires::TrackType> outputArray(
        outputArrayFeature);

    seqwires::TransposeProcessorInput::Instance input(inputValueFeature);

    EXPECT_EQ(input.getOffset().get(), 0);

    EXPECT_EQ(inputArray.getSize(), 1);
    EXPECT_EQ(outputArray.getSize(), 1);

    EXPECT_EQ(inputArray.getEntry(0).get().getDuration(), 0);
    EXPECT_EQ(outputArray.getEntry(0).get().getDuration(), 0);

    {
        babelwires::BackupScope scope(processor.getInputFeature().is<babelwires::ValueTreeRoot>());
        seqwires::Track track;
        testUtils::addSimpleNotes({60, 62, 64, 65}, track);
        inputArray.getEntry(0).set(std::move(track));
    }
    processor.process(testEnvironment.m_log);
    testUtils::testSimpleNotes({60, 62, 64, 65}, outputArray.getEntry(0).get());

    processor.getInputFeature().clearChanges();
    {
        babelwires::BackupScope scope(processor.getInputFeature().is<babelwires::ValueTreeRoot>());
        input.getOffset().set(1);
    }
    processor.process(testEnvironment.m_log);
    testUtils::testSimpleNotes({61, 63, 65, 66}, outputArray.getEntry(0).get());

    processor.getInputFeature().clearChanges();
    {
        babelwires::BackupScope scope(processor.getInputFeature().is<babelwires::ValueTreeRoot>());
        inputArray.setSize(2);
        {
            seqwires::Track track;
            testUtils::addSimpleNotes(std::vector<seqwires::Pitch>{48, 50, 52, 53}, track);
            inputArray.getEntry(1).set(std::move(track));
        }
    }
    processor.process(testEnvironment.m_log);

    ASSERT_EQ(outputArray.getSize(), 2);
    EXPECT_EQ(outputArray.getEntry(0).get().getDuration(), 1);
    EXPECT_EQ(outputArray.getEntry(1).get().getDuration(), 1);
    testUtils::testSimpleNotes(std::vector<seqwires::Pitch>{61, 63, 65, 66}, outputArray.getEntry(0).get());
    testUtils::testSimpleNotes(std::vector<seqwires::Pitch>{49, 51, 53, 54}, outputArray.getEntry(1).get());
}
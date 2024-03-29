#include <gtest/gtest.h>

#include <SeqWiresLib/Functions/transposeFunction.hpp>
#include <SeqWiresLib/Processors/transposeProcessor.hpp>
#include <SeqWiresLib/Types/Track/TrackEvents/noteEvents.hpp>
#include <SeqWiresLib/Types/Track/trackFeature.hpp>

#include <BabelWiresLib/Types/Int/intFeature.hpp>
#include <BabelWiresLib/Features/arrayFeature.hpp>

#include <Tests/TestUtils/seqTestUtils.hpp>
#include <Tests/BabelWiresLib/TestUtils/testEnvironment.hpp>


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
    testUtils::addChords({
        {seqwires::PitchClass::PitchClass::Value::C, seqwires::ChordType::ChordType::Value::M},
        {seqwires::PitchClass::PitchClass::Value::E, seqwires::ChordType::ChordType::Value::m},
        {seqwires::PitchClass::PitchClass::Value::Gsh, seqwires::ChordType::ChordType::Value::dim},
        {seqwires::PitchClass::PitchClass::Value::A, seqwires::ChordType::ChordType::Value::aug},
        }, trackIn);

    auto trackOut = seqwires::transposeTrack(trackIn, 0);

    testUtils::testChords({
        {seqwires::PitchClass::PitchClass::Value::C, seqwires::ChordType::ChordType::Value::M},
        {seqwires::PitchClass::PitchClass::Value::E, seqwires::ChordType::ChordType::Value::m},
        {seqwires::PitchClass::PitchClass::Value::Gsh, seqwires::ChordType::ChordType::Value::dim},
        {seqwires::PitchClass::PitchClass::Value::A, seqwires::ChordType::ChordType::Value::aug},
        }, trackIn);
}

TEST(TransposeProcessorTest, funcSimpleChordsPositive) {
    seqwires::Track trackIn;

    // Some random chords.
    testUtils::addChords({
        {seqwires::PitchClass::PitchClass::Value::C, seqwires::ChordType::ChordType::Value::M},
        {seqwires::PitchClass::PitchClass::Value::E, seqwires::ChordType::ChordType::Value::m},
        {seqwires::PitchClass::PitchClass::Value::Gsh, seqwires::ChordType::ChordType::Value::dim},
        {seqwires::PitchClass::PitchClass::Value::B, seqwires::ChordType::ChordType::Value::aug},
        }, trackIn);

    auto trackOut = seqwires::transposeTrack(trackIn, 1);

    testUtils::testChords({
        {seqwires::PitchClass::PitchClass::Value::Csh, seqwires::ChordType::ChordType::Value::M},
        {seqwires::PitchClass::PitchClass::Value::F, seqwires::ChordType::ChordType::Value::m},
        {seqwires::PitchClass::PitchClass::Value::A, seqwires::ChordType::ChordType::Value::dim},
        {seqwires::PitchClass::PitchClass::Value::C, seqwires::ChordType::ChordType::Value::aug},
        }, trackOut);
}

TEST(TransposeProcessorTest, funcSimpleChordsNegative) {
    seqwires::Track trackIn;

    // Some random chords.
    testUtils::addChords({
        {seqwires::PitchClass::PitchClass::Value::C, seqwires::ChordType::ChordType::Value::M},
        {seqwires::PitchClass::PitchClass::Value::E, seqwires::ChordType::ChordType::Value::m},
        {seqwires::PitchClass::PitchClass::Value::Gsh, seqwires::ChordType::ChordType::Value::dim},
        {seqwires::PitchClass::PitchClass::Value::B, seqwires::ChordType::ChordType::Value::aug},
        }, trackIn);

    auto trackOut = seqwires::transposeTrack(trackIn, -1);

    testUtils::testChords({
        {seqwires::PitchClass::PitchClass::Value::B, seqwires::ChordType::ChordType::Value::M},
        {seqwires::PitchClass::PitchClass::Value::Dsh, seqwires::ChordType::ChordType::Value::m},
        {seqwires::PitchClass::PitchClass::Value::G, seqwires::ChordType::ChordType::Value::dim},
        {seqwires::PitchClass::PitchClass::Value::Ash, seqwires::ChordType::ChordType::Value::aug},
        }, trackOut);
}

TEST(TransposeProcessorTest, processor) {
    testUtils::TestEnvironment testEnvironment;
    testEnvironment.m_typeSystem.addEntry<seqwires::DefaultTrackType>();

    seqwires::TransposeProcessor processor(testEnvironment.m_projectContext);

    processor.getInputFeature()->setToDefault();
    processor.getOutputFeature()->setToDefault();

    auto* pitchOffsetFeature = processor.getInputFeature()->getChildFromStep(babelwires::PathStep("Offset")).as<babelwires::IntFeature>();
    auto* inputArray = processor.getInputFeature()->getChildFromStep(babelwires::PathStep("Tracks")).as<babelwires::ArrayFeature>();
    auto* outputArray = processor.getOutputFeature()->getChildFromStep(babelwires::PathStep("Tracks")).as<babelwires::ArrayFeature>();
    ASSERT_NE(pitchOffsetFeature, nullptr);
    ASSERT_NE(inputArray, nullptr);
    ASSERT_NE(outputArray, nullptr);
    EXPECT_EQ(pitchOffsetFeature->get(), 0);

    EXPECT_EQ(inputArray->getNumFeatures(), 1);
    EXPECT_EQ(outputArray->getNumFeatures(), 1);

    auto getInputTrack = [&inputArray](int i) { return inputArray->getChildFromStep(i).as<seqwires::TrackFeature>(); };
    auto getOutputTrack = [&outputArray](int i) { return outputArray->getChildFromStep(i).as<seqwires::TrackFeature>(); };

    ASSERT_NE(getInputTrack(0), nullptr);
    ASSERT_NE(getOutputTrack(0), nullptr);

    EXPECT_EQ(getInputTrack(0)->get().getDuration(), 0);
    EXPECT_EQ(getOutputTrack(0)->get().getDuration(), 0);

    {
        seqwires::Track track;
        testUtils::addSimpleNotes({60, 62, 64, 65}, track);
        getInputTrack(0)->set(std::move(track));
    }
    processor.process(testEnvironment.m_log);
    testUtils::testSimpleNotes({60, 62, 64, 65}, getOutputTrack(0)->get());

    pitchOffsetFeature->set(1);
    processor.process(testEnvironment.m_log);
    testUtils::testSimpleNotes({61, 63, 65, 66}, getOutputTrack(0)->get());

    // Confirm that adding a track entry does not cause existing entries to be changed.
    processor.getInputFeature()->clearChanges();
    processor.getOutputFeature()->clearChanges();
    inputArray->addEntry(0);
    {
        seqwires::Track track;
        testUtils::addSimpleNotes(std::vector<seqwires::Pitch>{48, 50, 52, 53}, track);
        getInputTrack(0)->set(std::move(track));
    }
    processor.process(testEnvironment.m_log);
    ASSERT_EQ(outputArray->getNumFeatures(), 2);
    ASSERT_NE(getOutputTrack(0), nullptr);
    ASSERT_NE(getOutputTrack(1), nullptr);
    EXPECT_FALSE(getOutputTrack(1)->isChanged(babelwires::Feature::Changes::SomethingChanged));
    EXPECT_EQ(getOutputTrack(0)->get().getDuration(), 1);
    testUtils::testSimpleNotes(std::vector<seqwires::Pitch>{49, 51, 53, 54}, getOutputTrack(0)->get());
    testUtils::testSimpleNotes(std::vector<seqwires::Pitch>{61, 63, 65, 66}, getOutputTrack(1)->get());
}
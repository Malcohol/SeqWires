#include <gtest/gtest.h>

#include <SeqWiresLib/Functions/percussionMapFunction.hpp>
#include <SeqWiresLib/Processors/percussionMapProcessor.hpp>
#include <SeqWiresLib/Tracks/track.hpp>
#include <SeqWiresLib/percussion.hpp>

#include <BabelWiresLib/Features/mapFeature.hpp>
#include <BabelWiresLib/Maps/MapEntries/allToSameFallbackMapEntryData.hpp>
#include <BabelWiresLib/Maps/MapEntries/oneToOneMapEntryData.hpp>
#include <BabelWiresLib/Maps/mapData.hpp>
#include <BabelWiresLib/TypeSystem/enumValue.hpp>
#include <BabelWiresLib/TypeSystem/typeSystem.hpp>

#include <Tests/BabelWiresLib/TestUtils/testEnvironment.hpp>
#include <Tests/TestUtils/seqTestUtils.hpp>
#include <Tests/TestUtils/testLog.hpp>

namespace {
    babelwires::MapData getTestPercussionMap(const babelwires::TypeSystem& typeSystem) {
        const seqwires::GM2StandardPercussionKit& percussionType =
            typeSystem.getRegisteredEntry(seqwires::GM2StandardPercussionKit::getThisIdentifier()).is<seqwires::GM2StandardPercussionKit>();

        babelwires::MapData percussionMap;
        percussionMap.setSourceTypeId(seqwires::GMPercussionKit::getThisIdentifier());
        percussionMap.setTargetTypeId(seqwires::GMPercussionKit::getThisIdentifier());

        babelwires::OneToOneMapEntryData maplet(typeSystem, seqwires::GM2StandardPercussionKit::getThisIdentifier(),
                                                seqwires::GM2StandardPercussionKit::getThisIdentifier());

        babelwires::EnumValue sourceValue;
        babelwires::EnumValue targetValue;

        sourceValue.set(percussionType.getIdentifierFromValue(seqwires::GM2StandardPercussionKit::Value::Clap));
        targetValue.set(percussionType.getIdentifierFromValue(seqwires::GM2StandardPercussionKit::Value::Cowbll));
        maplet.setSourceValue(sourceValue.clone());
        maplet.setTargetValue(targetValue.clone());
        percussionMap.emplaceBack(maplet.clone());

        sourceValue.set(percussionType.getIdentifierFromValue(seqwires::GM2StandardPercussionKit::Value::Crash1));
        targetValue.set(percussionType.getIdentifierFromValue(seqwires::GM2StandardPercussionKit::Value::Crash2));
        maplet.setSourceValue(sourceValue.clone());
        maplet.setTargetValue(targetValue.clone());
        percussionMap.emplaceBack(maplet.clone());

        percussionMap.emplaceBack(std::make_unique<babelwires::AllToSameFallbackMapEntryData>());
        return percussionMap;
    }

    seqwires::Track getTestInputTrack() {
        seqwires::Track track;

        testUtils::addNotes(
            {
                {seqwires::GM2StandardPercussionKit::getPitchFromValue(seqwires::GM2StandardPercussionKit::Value::AcBass), 0,
                 babelwires::Rational(1, 2)},
                {seqwires::GM2StandardPercussionKit::getPitchFromValue(seqwires::GM2StandardPercussionKit::Value::Clap), 0,
                 babelwires::Rational(1, 2)},
                {12, 0, babelwires::Rational(1, 2)},
                {seqwires::GM2StandardPercussionKit::getPitchFromValue(seqwires::GM2StandardPercussionKit::Value::Crash1), 0,
                 babelwires::Rational(1, 2)},
            },
            track);

        return track;
    }

    void testOutputTrack(const seqwires::Track& outputTrack) {
        testUtils::testNotes(
            {
                {seqwires::GM2StandardPercussionKit::getPitchFromValue(seqwires::GM2StandardPercussionKit::Value::AcBass), 0,
                 babelwires::Rational(1, 2)},
                {seqwires::GM2StandardPercussionKit::getPitchFromValue(seqwires::GM2StandardPercussionKit::Value::Cowbll), 0,
                 babelwires::Rational(1, 2)},
                // The out of range note gets dropped here.
                {seqwires::GM2StandardPercussionKit::getPitchFromValue(seqwires::GM2StandardPercussionKit::Value::Crash2),
                 babelwires::Rational(1, 2), babelwires::Rational(1, 2)},
            },
            outputTrack);
    }
} // namespace

TEST(PercussionMapProcessorTest, funcSimple) {
    testUtils::TestLog log;

    babelwires::TypeSystem typeSystem;
    const seqwires::GMPercussionKit *const gmPercussionKit = typeSystem.addEntry(std::make_unique<seqwires::GMPercussionKit>());
    const seqwires::GM2StandardPercussionKit *const gm2StandardPercussionKit = typeSystem.addEntry(std::make_unique<seqwires::GM2StandardPercussionKit>(*gmPercussionKit));

    babelwires::MapData mapData = getTestPercussionMap(typeSystem);

    seqwires::Track inputTrack = getTestInputTrack();

    seqwires::Track outputTrack = seqwires::mapPercussionFunction(typeSystem, inputTrack, mapData);

    testOutputTrack(outputTrack);
    EXPECT_EQ(inputTrack.getDuration(), outputTrack.getDuration());
}

TEST(PercussionMapProcessorTest, processor) {
    testUtils::TestEnvironment testEnvironment;
    const seqwires::GMPercussionKit *const gmPercussionKit = testEnvironment.m_typeSystem.addEntry(std::make_unique<seqwires::GMPercussionKit>());
    const seqwires::GM2StandardPercussionKit *const gm2StandardPercussionKit = testEnvironment.m_typeSystem.addEntry(std::make_unique<seqwires::GM2StandardPercussionKit>(*gmPercussionKit));

    const seqwires::GM2StandardPercussionKit& percussionType =
        testEnvironment.m_typeSystem.getRegisteredEntry(seqwires::GM2StandardPercussionKit::getThisIdentifier())
            .is<seqwires::GM2StandardPercussionKit>();

    seqwires::PercussionMapProcessor processor(testEnvironment.m_projectContext);

    processor.getInputFeature()->setToDefault();
    processor.getOutputFeature()->setToDefault();

    auto* percussionMapFeature =
        processor.getInputFeature()->getChildFromStep(babelwires::PathStep("Map")).as<babelwires::MapFeature>();
    auto* inputArray =
        processor.getInputFeature()->getChildFromStep(babelwires::PathStep("Tracks")).as<babelwires::ArrayFeature>();
    auto* outputArray =
        processor.getOutputFeature()->getChildFromStep(babelwires::PathStep("Tracks")).as<babelwires::ArrayFeature>();
    ASSERT_NE(percussionMapFeature, nullptr);
    ASSERT_NE(inputArray, nullptr);
    ASSERT_NE(outputArray, nullptr);

    EXPECT_EQ(inputArray->getNumFeatures(), 1);
    EXPECT_EQ(outputArray->getNumFeatures(), 1);

    auto getInputTrack = [&inputArray](int i) { return inputArray->getChildFromStep(i).as<seqwires::TrackFeature>(); };
    auto getOutputTrack = [&outputArray](int i) {
        return outputArray->getChildFromStep(i).as<seqwires::TrackFeature>();
    };

    ASSERT_NE(getInputTrack(0), nullptr);
    ASSERT_NE(getOutputTrack(0), nullptr);

    percussionMapFeature->set(getTestPercussionMap(testEnvironment.m_typeSystem));
    getInputTrack(0)->set(getTestInputTrack());

    processor.process(testEnvironment.m_log);

    testOutputTrack(getOutputTrack(0)->get());
}

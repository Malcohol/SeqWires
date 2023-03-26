#include <gtest/gtest.h>

#include <SeqWiresLib/Functions/percussionMapFunction.hpp>
#include <SeqWiresLib/Percussion/abstractPercussionSet.hpp>
#include <SeqWiresLib/Percussion/builtInPercussionInstruments.hpp>
#include <SeqWiresLib/Processors/percussionMapProcessor.hpp>
#include <SeqWiresLib/Tracks/percussionEvents.hpp>
#include <SeqWiresLib/Tracks/track.hpp>

#include <BabelWiresLib/Types/Enum/addBlankToEnum.hpp>
#include <BabelWiresLib/Features/mapFeature.hpp>
#include <BabelWiresLib/Maps/MapEntries/allToSameFallbackMapEntryData.hpp>
#include <BabelWiresLib/Maps/MapEntries/oneToOneMapEntryData.hpp>
#include <BabelWiresLib/Maps/mapData.hpp>
#include <BabelWiresLib/Types/Enum/enumValue.hpp>
#include <BabelWiresLib/TypeSystem/typeSystem.hpp>

#include <Tests/BabelWiresLib/TestUtils/testEnvironment.hpp>
#include <Tests/TestUtils/seqTestUtils.hpp>
#include <Tests/TestUtils/testLog.hpp>

namespace {
    babelwires::MapData getTestPercussionMap(const babelwires::TypeSystem& typeSystem) {
        const seqwires::BuiltInPercussionInstruments& builtInPercussion =
            typeSystem.getEntryByType<seqwires::BuiltInPercussionInstruments>();

        babelwires::TypeRef targetTypeRef(babelwires::AddBlankToEnum::getThisIdentifier(), {{seqwires::BuiltInPercussionInstruments::getThisIdentifier()}});

        babelwires::MapData percussionMap;
        percussionMap.setSourceTypeRef(seqwires::BuiltInPercussionInstruments::getThisIdentifier());
        percussionMap.setTargetTypeRef(targetTypeRef);

        babelwires::OneToOneMapEntryData maplet(typeSystem, seqwires::BuiltInPercussionInstruments::getThisIdentifier(),
                                                targetTypeRef);

        babelwires::EnumValue sourceValue;
        babelwires::EnumValue targetValue;

        sourceValue.set(builtInPercussion.getIdentifierFromValue(seqwires::BuiltInPercussionInstruments::Value::Clap));
        targetValue.set(
            builtInPercussion.getIdentifierFromValue(seqwires::BuiltInPercussionInstruments::Value::Cowbll));
        maplet.setSourceValue(sourceValue.clone());
        maplet.setTargetValue(targetValue.clone());
        percussionMap.emplaceBack(maplet.clone());

        sourceValue.set(
            builtInPercussion.getIdentifierFromValue(seqwires::BuiltInPercussionInstruments::Value::Crash1));
        targetValue.set(
            builtInPercussion.getIdentifierFromValue(seqwires::BuiltInPercussionInstruments::Value::Crash2));
        maplet.setSourceValue(sourceValue.clone());
        maplet.setTargetValue(targetValue.clone());
        percussionMap.emplaceBack(maplet.clone());

        sourceValue.set(
            builtInPercussion.getIdentifierFromValue(seqwires::BuiltInPercussionInstruments::Value::LFlTom));
        targetValue.set(babelwires::AddBlankToEnum::getBlankValue());
        maplet.setSourceValue(sourceValue.clone());
        maplet.setTargetValue(targetValue.clone());
        percussionMap.emplaceBack(maplet.clone());

        percussionMap.emplaceBack(std::make_unique<babelwires::AllToSameFallbackMapEntryData>());
        return percussionMap;
    }

    seqwires::Track getTestInputTrack() {
        seqwires::Track track;
        track.addEvent(seqwires::PercussionOnEvent{0, "AcBass", 64});
        track.addEvent(seqwires::PercussionOffEvent{babelwires::Rational(1, 2), "AcBass", 64});
        track.addEvent(seqwires::PercussionOnEvent{0, "Clap", 64});
        track.addEvent(seqwires::PercussionOffEvent{babelwires::Rational(1, 2), "Clap", 64});
        track.addEvent(seqwires::PercussionOnEvent{0, "LFlTom", 64});
        track.addEvent(seqwires::PercussionOffEvent{babelwires::Rational(1, 2), "LFlTom", 64});
        // An unrecognized instrument.
        track.addEvent(seqwires::PercussionOnEvent{0, "Dummy", 64});
        track.addEvent(seqwires::PercussionOffEvent{babelwires::Rational(1, 2), "Dummy", 64});
        track.addEvent(seqwires::PercussionOnEvent{0, "Crash1", 64});
        track.addEvent(seqwires::PercussionOffEvent{babelwires::Rational(1, 2), "Crash1", 64});
        return track;
    }

    seqwires::Track getTestOutputTrack() {
        seqwires::Track track;
        track.addEvent(seqwires::PercussionOnEvent{0, "AcBass", 64});
        track.addEvent(seqwires::PercussionOffEvent{babelwires::Rational(1, 2), "AcBass", 64});
        track.addEvent(seqwires::PercussionOnEvent{0, "Cowbll", 64});
        track.addEvent(seqwires::PercussionOffEvent{babelwires::Rational(1, 2), "Cowbll", 64});
        // LFlTom removed.
        // An unrecognized instrument.
        track.addEvent(seqwires::PercussionOnEvent{babelwires::Rational(1, 2), "Dummy", 64});
        track.addEvent(seqwires::PercussionOffEvent{babelwires::Rational(1, 2), "Dummy", 64});
        track.addEvent(seqwires::PercussionOnEvent{0, "Crash2", 64});
        track.addEvent(seqwires::PercussionOffEvent{babelwires::Rational(1, 2), "Crash2", 64});
        return track;
    }
} // namespace

TEST(PercussionMapProcessorTest, funcSimple) {
    testUtils::TestLog log;

    babelwires::TypeSystem typeSystem;
    const seqwires::BuiltInPercussionInstruments* const builtInPercussion =
        typeSystem.addEntry<seqwires::BuiltInPercussionInstruments>();
    typeSystem.addTypeConstructor<babelwires::AddBlankToEnum>();

    const babelwires::MapData mapData = getTestPercussionMap(typeSystem);
    const seqwires::Track inputTrack = getTestInputTrack();
    const seqwires::Track outputTrack = seqwires::mapPercussionFunction(typeSystem, inputTrack, mapData);
    const seqwires::Track expectedOutputTrack = getTestOutputTrack();

    EXPECT_EQ(outputTrack, expectedOutputTrack);
}

TEST(PercussionMapProcessorTest, processor) {
    testUtils::TestEnvironment testEnvironment;
    testEnvironment.m_typeSystem.addEntry<seqwires::AbstractPercussionSet>();
    testEnvironment.m_typeSystem.addEntry<seqwires::BuiltInPercussionInstruments>();
    testEnvironment.m_typeSystem.addRelatedTypes(seqwires::BuiltInPercussionInstruments::getThisIdentifier(),
                                                 {{}, {seqwires::AbstractPercussionSet::getThisIdentifier()}});

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

    const seqwires::Track outputTrack = getOutputTrack(0)->get();
    const seqwires::Track expectedOutputTrack = getTestOutputTrack();

    EXPECT_EQ(outputTrack, expectedOutputTrack);
}

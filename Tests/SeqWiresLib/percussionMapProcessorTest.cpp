#include <gtest/gtest.h>

#include <SeqWiresLib/Functions/percussionMapFunction.hpp>
#include <SeqWiresLib/Percussion/builtInPercussionInstruments.hpp>
#include <SeqWiresLib/Processors/percussionMapProcessor.hpp>
#include <SeqWiresLib/Types/Track/TrackEvents/percussionEvents.hpp>
#include <SeqWiresLib/Types/Track/track.hpp>
#include <SeqWiresLib/libRegistration.hpp>

#include <BabelWiresLib/TypeSystem/typeSystem.hpp>
#include <BabelWiresLib/Types/Enum/addBlankToEnum.hpp>
#include <BabelWiresLib/Types/Enum/enumValue.hpp>
#include <BabelWiresLib/Types/Map/MapEntries/allToSameFallbackMapEntryData.hpp>
#include <BabelWiresLib/Types/Map/MapEntries/oneToOneMapEntryData.hpp>
#include <BabelWiresLib/Types/Map/mapFeature.hpp>
#include <BabelWiresLib/Types/Map/mapValue.hpp>

#include <Tests/BabelWiresLib/TestUtils/testEnvironment.hpp>
#include <Tests/TestUtils/seqTestUtils.hpp>
#include <Tests/TestUtils/testLog.hpp>

namespace {
    babelwires::MapValue getTestPercussionMap(const babelwires::TypeSystem& typeSystem) {
        const seqwires::BuiltInPercussionInstruments& builtInPercussion =
            typeSystem.getEntryByType<seqwires::BuiltInPercussionInstruments>();

        babelwires::TypeRef targetTypeRef(babelwires::AddBlankToEnum::getThisIdentifier(),
                                          seqwires::BuiltInPercussionInstruments::getThisIdentifier());

        babelwires::MapValue percussionMap;
        percussionMap.setSourceTypeRef(seqwires::BuiltInPercussionInstruments::getThisIdentifier());
        percussionMap.setTargetTypeRef(targetTypeRef);

        babelwires::OneToOneMapEntryData maplet(typeSystem, seqwires::BuiltInPercussionInstruments::getThisIdentifier(),
                                                targetTypeRef);

        babelwires::EnumValue sourceValue;
        babelwires::EnumValue targetValue;

        sourceValue.set(builtInPercussion.getIdentifierFromValue(seqwires::BuiltInPercussionInstruments::Value::Clap));
        targetValue.set(
            builtInPercussion.getIdentifierFromValue(seqwires::BuiltInPercussionInstruments::Value::Cowbll));
        maplet.setSourceValue(sourceValue);
        maplet.setTargetValue(targetValue);
        percussionMap.emplaceBack(maplet.clone());

        sourceValue.set(
            builtInPercussion.getIdentifierFromValue(seqwires::BuiltInPercussionInstruments::Value::Crash1));
        targetValue.set(
            builtInPercussion.getIdentifierFromValue(seqwires::BuiltInPercussionInstruments::Value::Crash2));
        maplet.setSourceValue(sourceValue);
        maplet.setTargetValue(targetValue);
        percussionMap.emplaceBack(maplet.clone());

        sourceValue.set(
            builtInPercussion.getIdentifierFromValue(seqwires::BuiltInPercussionInstruments::Value::LFlTom));
        targetValue.set(babelwires::AddBlankToEnum::getBlankValue());
        maplet.setSourceValue(sourceValue);
        maplet.setTargetValue(targetValue);
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

    const babelwires::MapValue mapValue = getTestPercussionMap(typeSystem);
    const seqwires::Track inputTrack = getTestInputTrack();
    const seqwires::Track outputTrack = seqwires::mapPercussionFunction(typeSystem, inputTrack, mapValue);
    const seqwires::Track expectedOutputTrack = getTestOutputTrack();

    EXPECT_EQ(outputTrack, expectedOutputTrack);
}

TEST(PercussionMapProcessorTest, processor) {
    testUtils::TestEnvironment testEnvironment;
    seqwires::registerLib(testEnvironment.m_projectContext);

    seqwires::PercussionMapProcessor processor(testEnvironment.m_projectContext);

    processor.getInputFeature().setToDefault();
    processor.getOutputFeature().setToDefault();

    babelwires::ValueFeature& inputValueFeature = processor.getInputFeature();
    const babelwires::ValueFeature& outputValueFeature = processor.getOutputFeature();

    babelwires::ValueFeature& inputArrayFeature =
        inputValueFeature.getChildFromStep(babelwires::PathStep(seqwires::PercussionMapProcessor::getCommonArrayId()))
            .is<babelwires::ValueFeature>();
    const babelwires::ValueFeature& outputArrayFeature =
        outputValueFeature.getChildFromStep(babelwires::PathStep(seqwires::PercussionMapProcessor::getCommonArrayId()))
            .is<babelwires::ValueFeature>();

    babelwires::ArrayInstanceImpl<babelwires::ValueFeature, seqwires::TrackType> inputArray(inputArrayFeature);
    const babelwires::ArrayInstanceImpl<const babelwires::ValueFeature, seqwires::TrackType> outputArray(
        outputArrayFeature);

    seqwires::PercussionMapProcessorInput::Instance input(inputValueFeature);

    EXPECT_EQ(inputArray.getSize(), 1);
    EXPECT_EQ(outputArray.getSize(), 1);

    EXPECT_EQ(inputArray.getEntry(0).get().getDuration(), 0);
    EXPECT_EQ(outputArray.getEntry(0).get().getDuration(), 0);
    processor.getOutputFeature().setToDefault();

    input.getMap()->setValue(getTestPercussionMap(testEnvironment.m_typeSystem));
    inputArray.getEntry(0).set(getTestInputTrack());

    processor.process(testEnvironment.m_log);

    const seqwires::Track outputTrack = outputArray.getEntry(0).get();
    const seqwires::Track expectedOutputTrack = getTestOutputTrack();

    EXPECT_EQ(outputTrack, expectedOutputTrack);
}

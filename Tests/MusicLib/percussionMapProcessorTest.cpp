#include <gtest/gtest.h>

#include <MusicLib/Functions/percussionMapFunction.hpp>
#include <MusicLib/Percussion/builtInPercussionInstruments.hpp>
#include <MusicLib/Processors/percussionMapProcessor.hpp>
#include <MusicLib/Types/Track/TrackEvents/percussionEvents.hpp>
#include <MusicLib/Types/Track/track.hpp>
#include <MusicLib/libRegistration.hpp>

#include <BabelWiresLib/TypeSystem/typeSystem.hpp>
#include <BabelWiresLib/Types/Enum/enumAtomTypeConstructor.hpp>
#include <BabelWiresLib/Types/Enum/enumUnionTypeConstructor.hpp>
#include <BabelWiresLib/Types/Enum/enumValue.hpp>
#include <BabelWiresLib/Types/Map/MapEntries/allToSameFallbackMapEntryData.hpp>
#include <BabelWiresLib/Types/Map/MapEntries/oneToOneMapEntryData.hpp>
#include <BabelWiresLib/Types/Map/mapValue.hpp>
#include <BabelWiresLib/Types/Map/standardMapIdentifiers.hpp>
#include <BabelWiresLib/ValueTree/valueTreeRoot.hpp>

#include <Tests/BabelWiresLib/TestUtils/testEnvironment.hpp>
#include <Tests/TestUtils/seqTestUtils.hpp>
#include <Tests/TestUtils/testLog.hpp>

namespace {
    babelwires::MapValue getTestPercussionMap(const babelwires::TypeSystem& typeSystem) {
        const seqwires::BuiltInPercussionInstruments& builtInPercussion =
            typeSystem.getEntryByType<seqwires::BuiltInPercussionInstruments>();

        babelwires::TypeRef targetTypeRef =
            babelwires::EnumUnionTypeConstructor::makeTypeRef(seqwires::BuiltInPercussionInstruments::getThisType(),
            babelwires::EnumAtomTypeConstructor::makeTypeRef(babelwires::getBlankValueId()));

        babelwires::MapValue percussionMap;
        percussionMap.setSourceTypeRef(seqwires::BuiltInPercussionInstruments::getThisType());
        percussionMap.setTargetTypeRef(targetTypeRef);

        babelwires::OneToOneMapEntryData maplet(typeSystem, seqwires::BuiltInPercussionInstruments::getThisType(),
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
        targetValue.set(babelwires::getBlankValueId());
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
    typeSystem.addTypeConstructor<babelwires::EnumAtomTypeConstructor>();
    typeSystem.addTypeConstructor<babelwires::EnumUnionTypeConstructor>();

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

    processor.getInput().setToDefault();
    processor.getOutput().setToDefault();

    babelwires::ValueTreeNode& input = processor.getInput();
    const babelwires::ValueTreeNode& output = processor.getOutput();

    babelwires::ValueTreeNode& inputArray =
        input.getChildFromStep(seqwires::PercussionMapProcessor::getCommonArrayId()).is<babelwires::ValueTreeNode>();
    const babelwires::ValueTreeNode& outputArray =
        output.getChildFromStep(seqwires::PercussionMapProcessor::getCommonArrayId()).is<babelwires::ValueTreeNode>();

    babelwires::ArrayInstanceImpl<babelwires::ValueTreeNode, seqwires::TrackType> inArray(inputArray);
    const babelwires::ArrayInstanceImpl<const babelwires::ValueTreeNode, seqwires::TrackType> outArray(outputArray);

    seqwires::PercussionMapProcessorInput::Instance in(input);

    EXPECT_EQ(inArray.getSize(), 1);
    EXPECT_EQ(outArray.getSize(), 1);

    EXPECT_EQ(inArray.getEntry(0).get().getDuration(), 0);
    EXPECT_EQ(outArray.getEntry(0).get().getDuration(), 0);
    processor.getOutput().setToDefault();

    in.getMap()->setValue(getTestPercussionMap(testEnvironment.m_typeSystem));
    inArray.getEntry(0).set(getTestInputTrack());

    processor.process(testEnvironment.m_log);

    const seqwires::Track outputTrack = outArray.getEntry(0).get();
    const seqwires::Track expectedOutputTrack = getTestOutputTrack();

    EXPECT_EQ(outputTrack, expectedOutputTrack);
}

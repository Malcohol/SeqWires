#include <gtest/gtest.h>

#include <SeqWiresLib/Functions/mapChordsFunction.hpp>
#include <SeqWiresLib/Processors/chordMapProcessor.hpp>
#include <SeqWiresLib/Types/Track/TrackEvents/chordEvents.hpp>
#include <SeqWiresLib/Types/Track/track.hpp>
#include <SeqWiresLib/Types/Track/trackType.hpp>
#include <SeqWiresLib/chord.hpp>
#include <SeqWiresLib/libRegistration.hpp>

#include <BabelWiresLib/TypeSystem/typeSystem.hpp>
#include <BabelWiresLib/Types/Enum/enumValue.hpp>
#include <BabelWiresLib/Types/Map/MapEntries/allToOneFallbackMapEntryData.hpp>
#include <BabelWiresLib/Types/Map/MapEntries/oneToOneMapEntryData.hpp>
#include <BabelWiresLib/Types/Map/mapValue.hpp>
#include <BabelWiresLib/Types/Map/standardMapIdentifiers.hpp>
#include <BabelWiresLib/Types/Tuple/tupleValue.hpp>
#include <BabelWiresLib/ValueTree/valueTreeRoot.hpp>

#include <Tests/BabelWiresLib/TestUtils/testEnvironment.hpp>
#include <Tests/TestUtils/seqTestUtils.hpp>
#include <Tests/TestUtils/testLog.hpp>

namespace {
    enum class SourceMode { ChordOnly, NoChordToChord };
    enum class TargetMode { ChordOnly, ChordToNoChord };

    babelwires::MapValue getTestChordMap(const babelwires::TypeSystem& typeSystem, SourceMode sourceMode,
                                         TargetMode targetMode) {
        const seqwires::ChordType& chordTypeEnum = typeSystem.getEntryByType<seqwires::ChordType>();
        const seqwires::PitchClass& pitchClassEnum =
            typeSystem.getEntryByType<seqwires::PitchClass>().is<seqwires::PitchClass>();

        babelwires::MapValue chordMap;
        chordMap.setSourceTypeRef(seqwires::getMapChordFunctionSourceTypeRef());
        chordMap.setTargetTypeRef(seqwires::getMapChordFunctionTargetTypeRef());

        {
            babelwires::OneToOneMapEntryData chordMaplet(typeSystem, seqwires::getMapChordFunctionSourceTypeRef(),
                                                         seqwires::getMapChordFunctionTargetTypeRef());

            babelwires::EnumValue pitchClassSourceValue;
            pitchClassSourceValue.set(pitchClassEnum.getIdentifierFromValue(seqwires::PitchClass::Value::D));

            babelwires::EnumValue chordTypeSourceValue;
            chordTypeSourceValue.set(chordTypeEnum.getIdentifierFromValue(seqwires::ChordType::Value::m));

            babelwires::TupleValue sourceValue({pitchClassSourceValue, chordTypeSourceValue});

            babelwires::EnumValue pitchClassTargetValue;
            pitchClassTargetValue.set(pitchClassEnum.getIdentifierFromValue(seqwires::PitchClass::Value::A));

            babelwires::EnumValue chordTypeTargetValue;
            chordTypeTargetValue.set(chordTypeEnum.getIdentifierFromValue(seqwires::ChordType::Value::m7));

            babelwires::TupleValue targetValue({pitchClassTargetValue, chordTypeTargetValue});

            chordMaplet.setSourceValue(sourceValue);
            chordMaplet.setTargetValue(targetValue);

            chordMap.emplaceBack(chordMaplet.clone());
        }
        if (sourceMode == SourceMode::NoChordToChord) {
            babelwires::OneToOneMapEntryData chordMaplet(typeSystem, seqwires::getMapChordFunctionSourceTypeRef(),
                                                         seqwires::getMapChordFunctionTargetTypeRef());

            babelwires::EnumValue sourceValue;
            sourceValue.set(seqwires::NoChord::getNoChordValue());

            babelwires::EnumValue pitchClassTargetValue;
            pitchClassTargetValue.set(pitchClassEnum.getIdentifierFromValue(seqwires::PitchClass::Value::Fsh));

            babelwires::EnumValue chordTypeTargetValue;
            chordTypeTargetValue.set(chordTypeEnum.getIdentifierFromValue(seqwires::ChordType::Value::m7_11));

            babelwires::TupleValue targetValue({pitchClassTargetValue, chordTypeTargetValue});

            chordMaplet.setSourceValue(sourceValue);
            chordMaplet.setTargetValue(targetValue);

            chordMap.emplaceBack(chordMaplet.clone());
        }
        if (targetMode == TargetMode::ChordToNoChord) {
            babelwires::OneToOneMapEntryData chordMaplet(typeSystem, seqwires::getMapChordFunctionSourceTypeRef(),
                                                         seqwires::getMapChordFunctionTargetTypeRef());

            babelwires::EnumValue pitchClassSourceValue;
            pitchClassSourceValue.set(pitchClassEnum.getIdentifierFromValue(seqwires::PitchClass::Value::Gsh));

            babelwires::EnumValue chordTypeSourceValue;
            chordTypeSourceValue.set(chordTypeEnum.getIdentifierFromValue(seqwires::ChordType::Value::M6));

            babelwires::TupleValue sourceValue({pitchClassSourceValue, chordTypeSourceValue});

            babelwires::EnumValue targetValue;
            targetValue.set(seqwires::NoChord::getNoChordValue());

            chordMaplet.setSourceValue(sourceValue);
            chordMaplet.setTargetValue(targetValue);

            chordMap.emplaceBack(chordMaplet.clone());
        }
        {
            babelwires::AllToOneFallbackMapEntryData chordMaplet(typeSystem,
                                                                 seqwires::getMapChordFunctionTargetTypeRef());

            babelwires::EnumValue wildcardValue;
            wildcardValue.set(babelwires::getWildcardMatchId());

            babelwires::TupleValue targetValue({wildcardValue, wildcardValue});

            chordMaplet.setTargetValue(targetValue);

            chordMap.emplaceBack(chordMaplet.clone());
        }
        return chordMap;
    }

    seqwires::Track getTestInputTrack() {
        seqwires::Track track;

        testUtils::addChords({{seqwires::PitchClass::Value::C, seqwires::ChordType::ChordType::Value::M,
                               babelwires::Rational(1, 2), babelwires::Rational(1, 2)},
                              {seqwires::PitchClass::Value::D, seqwires::ChordType::ChordType::Value::m},
                              {seqwires::PitchClass::Value::Gsh, seqwires::ChordType::ChordType::Value::M6},
                              {seqwires::PitchClass::Value::C, seqwires::ChordType::ChordType::Value::m},
                              {seqwires::PitchClass::Value::D, seqwires::ChordType::ChordType::Value::m}},
                             track);

        track.setDuration(babelwires::Rational(7, 2));

        return track;
    }

    void testOutputTrack(const seqwires::Track& outputTrack, SourceMode sourceMode, TargetMode targetMode) {
        EXPECT_EQ(outputTrack.getDuration(), babelwires::Rational(7, 2));

        if ((sourceMode == SourceMode::ChordOnly) && (targetMode == TargetMode::ChordOnly)) {
            testUtils::testChords({{seqwires::PitchClass::Value::C, seqwires::ChordType::ChordType::Value::M,
                                    babelwires::Rational(1, 2), babelwires::Rational(1, 2)},
                                   {seqwires::PitchClass::Value::A, seqwires::ChordType::ChordType::Value::m7},
                                   {seqwires::PitchClass::Value::Gsh, seqwires::ChordType::ChordType::Value::M6},
                                   {seqwires::PitchClass::Value::C, seqwires::ChordType::ChordType::Value::m},
                                   {seqwires::PitchClass::Value::A, seqwires::ChordType::ChordType::Value::m7}},
                                  outputTrack);
        } else if ((sourceMode == SourceMode::ChordOnly) && (targetMode == TargetMode::ChordToNoChord)) {
            testUtils::testChords({{seqwires::PitchClass::Value::C, seqwires::ChordType::ChordType::Value::M,
                                    babelwires::Rational(1, 2), babelwires::Rational(1, 2)},
                                   {seqwires::PitchClass::Value::A, seqwires::ChordType::ChordType::Value::m7},
                                   {seqwires::PitchClass::Value::C, seqwires::ChordType::ChordType::Value::m,
                                    babelwires::Rational(1, 2), babelwires::Rational(1, 2)},
                                   {seqwires::PitchClass::Value::A, seqwires::ChordType::ChordType::Value::m7}},
                                  outputTrack);
        } else if ((sourceMode == SourceMode::NoChordToChord) && (targetMode == TargetMode::ChordOnly)) {
            testUtils::testChords({{seqwires::PitchClass::Value::Fsh, seqwires::ChordType::ChordType::Value::m7_11},
                                   {seqwires::PitchClass::Value::C, seqwires::ChordType::ChordType::Value::M},
                                   {seqwires::PitchClass::Value::A, seqwires::ChordType::ChordType::Value::m7},
                                   {seqwires::PitchClass::Value::Gsh, seqwires::ChordType::ChordType::Value::M6},
                                   {seqwires::PitchClass::Value::C, seqwires::ChordType::ChordType::Value::m},
                                   {seqwires::PitchClass::Value::A, seqwires::ChordType::ChordType::Value::m7},
                                   {seqwires::PitchClass::Value::Fsh, seqwires::ChordType::ChordType::Value::m7_11}},
                                  outputTrack);
        } else if ((sourceMode == SourceMode::NoChordToChord) && (targetMode == TargetMode::ChordToNoChord)) {
            testUtils::testChords({{seqwires::PitchClass::Value::Fsh, seqwires::ChordType::ChordType::Value::m7_11},
                                   {seqwires::PitchClass::Value::C, seqwires::ChordType::ChordType::Value::M},
                                   {seqwires::PitchClass::Value::A, seqwires::ChordType::ChordType::Value::m7},
                                   {seqwires::PitchClass::Value::C, seqwires::ChordType::ChordType::Value::m,
                                    babelwires::Rational(1, 2), babelwires::Rational(1, 2)},
                                   {seqwires::PitchClass::Value::A, seqwires::ChordType::ChordType::Value::m7},
                                   {seqwires::PitchClass::Value::Fsh, seqwires::ChordType::ChordType::Value::m7_11}},
                                  outputTrack);
        }
    }

    using TestData = std::tuple<SourceMode, TargetMode>;
} // namespace


class ChordMapProcessorTest : public testing::TestWithParam<TestData> {};

TEST_P(ChordMapProcessorTest, simpleFunction) {
    testUtils::TestEnvironment testEnvironment;
    seqwires::registerLib(testEnvironment.m_projectContext);

    const auto [sourceMode, targetMode] = GetParam();
    babelwires::MapValue chordMap = getTestChordMap(testEnvironment.m_typeSystem, sourceMode, targetMode);

    seqwires::Track inputTrack = getTestInputTrack();
    seqwires::Track outputTrack = seqwires::mapChordsFunction(testEnvironment.m_typeSystem, inputTrack, chordMap);
    testOutputTrack(outputTrack, sourceMode, targetMode);
}

INSTANTIATE_TEST_SUITE_P(
    ChordMapProcessorTest, ChordMapProcessorTest,
    testing::Values(TestData{SourceMode::ChordOnly, TargetMode::ChordOnly},
                    TestData{SourceMode::NoChordToChord, TargetMode::ChordOnly},
                    TestData{SourceMode::ChordOnly, TargetMode::ChordToNoChord},
                    TestData{SourceMode::NoChordToChord, TargetMode::ChordToNoChord}));

/*
TEST(ChordMapProcessorTest, processor) {
    testUtils::TestEnvironment testEnvironment;
    seqwires::registerLib(testEnvironment.m_projectContext);

    const seqwires::ChordType& chordTypeEnum =
        testEnvironment.m_typeSystem.getEntryByType<seqwires::ChordType>().is<seqwires::ChordType>();
    const seqwires::PitchClass& pitchClassEnum =
        testEnvironment.m_typeSystem.getEntryByType<seqwires::PitchClass>().is<seqwires::PitchClass>();

    seqwires::ChordMapProcessor processor(testEnvironment.m_projectContext);

    processor.getInput().setToDefault();
    processor.getOutput().setToDefault();

    babelwires::ValueTreeNode& input = processor.getInput();
    const babelwires::ValueTreeNode& output = processor.getOutput();

    babelwires::ValueTreeNode& inputArray =
        input.getChildFromStep(babelwires::PathStep(seqwires::ChordMapProcessor::getCommonArrayId()))
            .is<babelwires::ValueTreeNode>();
    const babelwires::ValueTreeNode& outputArray =
        output.getChildFromStep(babelwires::PathStep(seqwires::ChordMapProcessor::getCommonArrayId()))
            .is<babelwires::ValueTreeNode>();

    babelwires::ArrayInstanceImpl<babelwires::ValueTreeNode, seqwires::TrackType> inArray(inputArray);
    const babelwires::ArrayInstanceImpl<const babelwires::ValueTreeNode, seqwires::TrackType> outArray(outputArray);

    seqwires::ChordMapProcessorInput::Instance in(input);

    EXPECT_EQ(inArray.getSize(), 1);
    EXPECT_EQ(outArray.getSize(), 1);

    EXPECT_EQ(inArray.getEntry(0).get().getDuration(), 0);
    EXPECT_EQ(outArray.getEntry(0).get().getDuration(), 0);

    in.getTypMap()->setValue(getTestChordTypeMap(testEnvironment.m_typeSystem));
    in.getRtMap()->setValue(getTestPitchClassMap(testEnvironment.m_typeSystem));
    inArray.getEntry(0).set(getTestInputTrack());

    processor.process(testEnvironment.m_log);

    testOutputTrack(outArray.getEntry(0).get());
}

*/

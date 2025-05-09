#include <gtest/gtest.h>

#include <SeqWiresLib/Functions/mapChordsFunction.hpp>
#include <SeqWiresLib/Processors/chordMapProcessor.hpp>
#include <SeqWiresLib/Types/Track/TrackEvents/chordEvents.hpp>
#include <SeqWiresLib/Types/Track/track.hpp>
#include <SeqWiresLib/Types/Track/trackType.hpp>
#include <SeqWiresLib/chord.hpp>
#include <SeqWiresLib/libRegistration.hpp>
#include <SeqWiresLib/Functions/mergeFunction.hpp>

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
    enum class SourceMode { ChordToChord, SilenceToChord };
    enum class TargetMode { ChordToChord, ChordToSilence };
    enum class WildcardMode { NoWildcards, Wildcards };

    babelwires::MapValue getTestChordMap(const babelwires::TypeSystem& typeSystem, SourceMode sourceMode,
                                         TargetMode targetMode, WildcardMode wildcardMode) {
        const seqwires::ChordType& chordTypeEnum = typeSystem.getEntryByType<seqwires::ChordType>();
        const seqwires::PitchClass& pitchClassEnum =
            typeSystem.getEntryByType<seqwires::PitchClass>().is<seqwires::PitchClass>();

        babelwires::MapValue chordMap;
        chordMap.setSourceTypeRef(seqwires::getMapChordFunctionSourceTypeRef());
        chordMap.setTargetTypeRef(seqwires::getMapChordFunctionTargetTypeRef());

        // This object gets re-used.
        babelwires::OneToOneMapEntryData chordMaplet(typeSystem, seqwires::getMapChordFunctionSourceTypeRef(),
                                                     seqwires::getMapChordFunctionTargetTypeRef());
        {
            {
                babelwires::EnumValue pitchClassSourceValue(
                    pitchClassEnum.getIdentifierFromValue(seqwires::PitchClass::Value::D));
                babelwires::EnumValue chordTypeSourceValue(
                    chordTypeEnum.getIdentifierFromValue(seqwires::ChordType::Value::m));
                babelwires::TupleValue sourceValue({pitchClassSourceValue, chordTypeSourceValue});

                babelwires::EnumValue pitchClassTargetValue(
                    pitchClassEnum.getIdentifierFromValue(seqwires::PitchClass::Value::A));
                babelwires::EnumValue chordTypeTargetValue(
                    chordTypeEnum.getIdentifierFromValue(seqwires::ChordType::Value::m7));
                babelwires::TupleValue targetValue({pitchClassTargetValue, chordTypeTargetValue});

                chordMaplet.setSourceValue(sourceValue);
                chordMaplet.setTargetValue(targetValue);
                chordMap.emplaceBack(chordMaplet.clone());
            }
            if (wildcardMode == WildcardMode::Wildcards) {
                babelwires::EnumValue wildcardValue(babelwires::getWildcardId());
                babelwires::EnumValue chordTypeSourceValue(
                    chordTypeEnum.getIdentifierFromValue(seqwires::ChordType::Value::M7s11));
                babelwires::TupleValue sourceValue({wildcardValue, chordTypeSourceValue});

                babelwires::EnumValue wildcardMatchValue(babelwires::getWildcardMatchId());
                babelwires::EnumValue chordTypeTargetValue(
                    chordTypeEnum.getIdentifierFromValue(seqwires::ChordType::Value::m7));
                babelwires::TupleValue targetValue({wildcardMatchValue, chordTypeTargetValue});

                chordMaplet.setSourceValue(sourceValue);
                chordMaplet.setTargetValue(targetValue);
                chordMap.emplaceBack(chordMaplet.clone());
            }
        }
        if (sourceMode == SourceMode::SilenceToChord) {
            babelwires::EnumValue sourceValue(seqwires::NoChord::getNoChordValue());

            babelwires::EnumValue pitchClassTargetValue(
                pitchClassEnum.getIdentifierFromValue(seqwires::PitchClass::Value::Fsh));
            babelwires::EnumValue chordTypeTargetValue(
                chordTypeEnum.getIdentifierFromValue(seqwires::ChordType::Value::m7_11));
            babelwires::TupleValue targetValue({pitchClassTargetValue, chordTypeTargetValue});

            chordMaplet.setSourceValue(sourceValue);
            chordMaplet.setTargetValue(targetValue);
            chordMap.emplaceBack(chordMaplet.clone());
        }
        if (targetMode == TargetMode::ChordToSilence) {
            {
                babelwires::EnumValue pitchClassSourceValue(
                    pitchClassEnum.getIdentifierFromValue(seqwires::PitchClass::Value::Gsh));
                babelwires::EnumValue chordTypeSourceValue(
                    chordTypeEnum.getIdentifierFromValue(seqwires::ChordType::Value::M6));
                babelwires::TupleValue sourceValue({pitchClassSourceValue, chordTypeSourceValue});

                babelwires::EnumValue targetValue;
                targetValue.set(seqwires::NoChord::getNoChordValue());

                chordMaplet.setSourceValue(sourceValue);
                chordMaplet.setTargetValue(targetValue);
                chordMap.emplaceBack(chordMaplet.clone());
            }
            if (wildcardMode == WildcardMode::Wildcards) {
                babelwires::EnumValue pitchClassSourceValue(
                    pitchClassEnum.getIdentifierFromValue(seqwires::PitchClass::Value::Dsh));
                babelwires::EnumValue wildcardValue(babelwires::getWildcardId());
                babelwires::TupleValue sourceValue({pitchClassSourceValue, wildcardValue});

                babelwires::EnumValue targetValue;
                targetValue.set(seqwires::NoChord::getNoChordValue());

                chordMaplet.setSourceValue(sourceValue);
                chordMaplet.setTargetValue(targetValue);
                chordMap.emplaceBack(chordMaplet.clone());
            }
        }
        {
            babelwires::AllToOneFallbackMapEntryData chordMaplet(typeSystem,
                                                                 seqwires::getMapChordFunctionTargetTypeRef());

            babelwires::EnumValue wildcardValue(babelwires::getWildcardMatchId());
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
                              {seqwires::PitchClass::Value::Dsh, seqwires::ChordType::ChordType::Value::M},
                              {seqwires::PitchClass::Value::C, seqwires::ChordType::ChordType::Value::m},
                              {seqwires::PitchClass::Value::E, seqwires::ChordType::ChordType::Value::m,
                               babelwires::Rational(1, 2), babelwires::Rational(1, 2)},
                              {seqwires::PitchClass::Value::D, seqwires::ChordType::ChordType::Value::m},
                              {seqwires::PitchClass::Value::Csh, seqwires::ChordType::ChordType::Value::M7s11}},
                             track);

        track.setDuration(babelwires::Rational(11, 2));

        return track;
    }

    void testOutputTrack(const seqwires::Track& outputTrack, SourceMode sourceMode, TargetMode targetMode,
                         WildcardMode wildcardMode) {
        EXPECT_EQ(outputTrack.getDuration(), babelwires::Rational(11, 2));

        if (wildcardMode == WildcardMode::NoWildcards) {
            if ((sourceMode == SourceMode::ChordToChord) && (targetMode == TargetMode::ChordToChord)) {
                testUtils::testChords({{seqwires::PitchClass::Value::C, seqwires::ChordType::ChordType::Value::M,
                                        babelwires::Rational(1, 2), babelwires::Rational(1, 2)},
                                       {seqwires::PitchClass::Value::A, seqwires::ChordType::ChordType::Value::m7},
                                       {seqwires::PitchClass::Value::Gsh, seqwires::ChordType::ChordType::Value::M6},
                                       {seqwires::PitchClass::Value::Dsh, seqwires::ChordType::ChordType::Value::M},
                                       {seqwires::PitchClass::Value::C, seqwires::ChordType::ChordType::Value::m},
                                       {seqwires::PitchClass::Value::E, seqwires::ChordType::ChordType::Value::m,
                                        babelwires::Rational(1, 2), babelwires::Rational(1, 2)},
                                       {seqwires::PitchClass::Value::A, seqwires::ChordType::ChordType::Value::m7},
                                       {seqwires::PitchClass::Value::Csh, seqwires::ChordType::ChordType::Value::M7s11}},
                                      outputTrack);
            } else if ((sourceMode == SourceMode::ChordToChord) && (targetMode == TargetMode::ChordToSilence)) {
                testUtils::testChords({{seqwires::PitchClass::Value::C, seqwires::ChordType::ChordType::Value::M,
                                        babelwires::Rational(1, 2), babelwires::Rational(1, 2)},
                                       {seqwires::PitchClass::Value::A, seqwires::ChordType::ChordType::Value::m7},
                                       {seqwires::PitchClass::Value::Dsh, seqwires::ChordType::ChordType::Value::M,
                                        babelwires::Rational(1, 2), babelwires::Rational(1, 2)},
                                       {seqwires::PitchClass::Value::C, seqwires::ChordType::ChordType::Value::m},
                                       {seqwires::PitchClass::Value::E, seqwires::ChordType::ChordType::Value::m,
                                        babelwires::Rational(1, 2), babelwires::Rational(1, 2)},
                                       {seqwires::PitchClass::Value::A, seqwires::ChordType::ChordType::Value::m7},
                                       {seqwires::PitchClass::Value::Csh, seqwires::ChordType::ChordType::Value::M7s11}},
                                      outputTrack);
            } else if ((sourceMode == SourceMode::SilenceToChord) && (targetMode == TargetMode::ChordToChord)) {
                testUtils::testChords(
                    {{seqwires::PitchClass::Value::Fsh, seqwires::ChordType::ChordType::Value::m7_11},
                     {seqwires::PitchClass::Value::C, seqwires::ChordType::ChordType::Value::M},
                     {seqwires::PitchClass::Value::A, seqwires::ChordType::ChordType::Value::m7},
                     {seqwires::PitchClass::Value::Gsh, seqwires::ChordType::ChordType::Value::M6},
                     {seqwires::PitchClass::Value::Dsh, seqwires::ChordType::ChordType::Value::M},
                     {seqwires::PitchClass::Value::C, seqwires::ChordType::ChordType::Value::m},
                     {seqwires::PitchClass::Value::Fsh, seqwires::ChordType::ChordType::Value::m7_11},
                     {seqwires::PitchClass::Value::E, seqwires::ChordType::ChordType::Value::m},
                     {seqwires::PitchClass::Value::A, seqwires::ChordType::ChordType::Value::m7},
                     {seqwires::PitchClass::Value::Csh, seqwires::ChordType::ChordType::Value::M7s11},
                     {seqwires::PitchClass::Value::Fsh, seqwires::ChordType::ChordType::Value::m7_11}},
                    outputTrack);
            } else if ((sourceMode == SourceMode::SilenceToChord) && (targetMode == TargetMode::ChordToSilence)) {
                testUtils::testChords(
                    {{seqwires::PitchClass::Value::Fsh, seqwires::ChordType::ChordType::Value::m7_11},
                     {seqwires::PitchClass::Value::C, seqwires::ChordType::ChordType::Value::M},
                     {seqwires::PitchClass::Value::A, seqwires::ChordType::ChordType::Value::m7},
                     {seqwires::PitchClass::Value::Dsh, seqwires::ChordType::ChordType::Value::M,
                      babelwires::Rational(1, 2), babelwires::Rational(1, 2)},
                     {seqwires::PitchClass::Value::C, seqwires::ChordType::ChordType::Value::m},
                     {seqwires::PitchClass::Value::Fsh, seqwires::ChordType::ChordType::Value::m7_11},
                     {seqwires::PitchClass::Value::E, seqwires::ChordType::ChordType::Value::m},
                     {seqwires::PitchClass::Value::A, seqwires::ChordType::ChordType::Value::m7},
                     {seqwires::PitchClass::Value::Csh, seqwires::ChordType::ChordType::Value::M7s11},
                     {seqwires::PitchClass::Value::Fsh, seqwires::ChordType::ChordType::Value::m7_11}},
                    outputTrack);
            }
        } else {
            if ((sourceMode == SourceMode::ChordToChord) && (targetMode == TargetMode::ChordToChord)) {
                testUtils::testChords({{seqwires::PitchClass::Value::C, seqwires::ChordType::ChordType::Value::M,
                                        babelwires::Rational(1, 2), babelwires::Rational(1, 2)},
                                       {seqwires::PitchClass::Value::A, seqwires::ChordType::ChordType::Value::m7},
                                       {seqwires::PitchClass::Value::Gsh, seqwires::ChordType::ChordType::Value::M6},
                                       {seqwires::PitchClass::Value::Dsh, seqwires::ChordType::ChordType::Value::M},
                                       {seqwires::PitchClass::Value::C, seqwires::ChordType::ChordType::Value::m},
                                       {seqwires::PitchClass::Value::E, seqwires::ChordType::ChordType::Value::m,
                                        babelwires::Rational(1, 2), babelwires::Rational(1, 2)},
                                       {seqwires::PitchClass::Value::A, seqwires::ChordType::ChordType::Value::m7},
                                       {seqwires::PitchClass::Value::Csh, seqwires::ChordType::ChordType::Value::m7}},
                                      outputTrack);
            } else if ((sourceMode == SourceMode::ChordToChord) && (targetMode == TargetMode::ChordToSilence)) {
                testUtils::testChords({{seqwires::PitchClass::Value::C, seqwires::ChordType::ChordType::Value::M,
                                        babelwires::Rational(1, 2), babelwires::Rational(1, 2)},
                                       {seqwires::PitchClass::Value::A, seqwires::ChordType::ChordType::Value::m7},
                                       {seqwires::PitchClass::Value::C, seqwires::ChordType::ChordType::Value::m,
                                        babelwires::Rational(1, 2), babelwires::Rational(1)},
                                       {seqwires::PitchClass::Value::E, seqwires::ChordType::ChordType::Value::m,
                                        babelwires::Rational(1, 2), babelwires::Rational(1, 2)},
                                       {seqwires::PitchClass::Value::A, seqwires::ChordType::ChordType::Value::m7},
                                       {seqwires::PitchClass::Value::Csh, seqwires::ChordType::ChordType::Value::m7}},
                                      outputTrack);
            } else if ((sourceMode == SourceMode::SilenceToChord) && (targetMode == TargetMode::ChordToChord)) {
                testUtils::testChords(
                    {{seqwires::PitchClass::Value::Fsh, seqwires::ChordType::ChordType::Value::m7_11},
                     {seqwires::PitchClass::Value::C, seqwires::ChordType::ChordType::Value::M},
                     {seqwires::PitchClass::Value::A, seqwires::ChordType::ChordType::Value::m7},
                     {seqwires::PitchClass::Value::Gsh, seqwires::ChordType::ChordType::Value::M6},
                     {seqwires::PitchClass::Value::Dsh, seqwires::ChordType::ChordType::Value::M},
                     {seqwires::PitchClass::Value::C, seqwires::ChordType::ChordType::Value::m},
                     {seqwires::PitchClass::Value::Fsh, seqwires::ChordType::ChordType::Value::m7_11},
                     {seqwires::PitchClass::Value::E, seqwires::ChordType::ChordType::Value::m},
                     {seqwires::PitchClass::Value::A, seqwires::ChordType::ChordType::Value::m7},
                     {seqwires::PitchClass::Value::Csh, seqwires::ChordType::ChordType::Value::m7},
                     {seqwires::PitchClass::Value::Fsh, seqwires::ChordType::ChordType::Value::m7_11}},
                    outputTrack);
            } else if ((sourceMode == SourceMode::SilenceToChord) && (targetMode == TargetMode::ChordToSilence)) {
                testUtils::testChords(
                    {{seqwires::PitchClass::Value::Fsh, seqwires::ChordType::ChordType::Value::m7_11},
                     {seqwires::PitchClass::Value::C, seqwires::ChordType::ChordType::Value::M},
                     {seqwires::PitchClass::Value::A, seqwires::ChordType::ChordType::Value::m7},
                     {seqwires::PitchClass::Value::C, seqwires::ChordType::ChordType::Value::m,
                      babelwires::Rational(1, 2), babelwires::Rational(1)},
                     {seqwires::PitchClass::Value::Fsh, seqwires::ChordType::ChordType::Value::m7_11},
                     {seqwires::PitchClass::Value::E, seqwires::ChordType::ChordType::Value::m},
                     {seqwires::PitchClass::Value::A, seqwires::ChordType::ChordType::Value::m7},
                     {seqwires::PitchClass::Value::Csh, seqwires::ChordType::ChordType::Value::m7},
                     {seqwires::PitchClass::Value::Fsh, seqwires::ChordType::ChordType::Value::m7_11},
                     },
                    outputTrack);
            }
        }
    }

    using TestData = std::tuple<SourceMode, TargetMode, WildcardMode>;
} // namespace

class ChordMapProcessorTest : public testing::TestWithParam<TestData> {};

TEST_P(ChordMapProcessorTest, simpleFunction) {
    testUtils::TestEnvironment testEnvironment;
    seqwires::registerLib(testEnvironment.m_projectContext);

    const auto [sourceMode, targetMode, wildcardMode] = GetParam();
    babelwires::MapValue chordMap = getTestChordMap(testEnvironment.m_typeSystem, sourceMode, targetMode, wildcardMode);

    seqwires::Track inputTrack = getTestInputTrack();
    seqwires::Track outputTrack = seqwires::mapChordsFunction(testEnvironment.m_typeSystem, inputTrack, chordMap);
    testOutputTrack(outputTrack, sourceMode, targetMode, wildcardMode);
}

INSTANTIATE_TEST_SUITE_P(
    ChordMapProcessorTest, ChordMapProcessorTest,
    testing::Values(TestData{SourceMode::ChordToChord, TargetMode::ChordToChord, WildcardMode::NoWildcards},
                    TestData{SourceMode::ChordToChord, TargetMode::ChordToSilence, WildcardMode::NoWildcards},
                    TestData{SourceMode::SilenceToChord, TargetMode::ChordToChord, WildcardMode::NoWildcards},
                    TestData{SourceMode::SilenceToChord, TargetMode::ChordToSilence, WildcardMode::NoWildcards},
                    TestData{SourceMode::ChordToChord, TargetMode::ChordToChord, WildcardMode::Wildcards},
                    TestData{SourceMode::ChordToChord, TargetMode::ChordToSilence, WildcardMode::Wildcards},
                    TestData{SourceMode::SilenceToChord, TargetMode::ChordToChord, WildcardMode::Wildcards},
                    TestData{SourceMode::SilenceToChord, TargetMode::ChordToSilence, WildcardMode::Wildcards}));


TEST(ChordMapProcessorTest, chordsAndNotes) {
    testUtils::TestEnvironment testEnvironment;
    seqwires::registerLib(testEnvironment.m_projectContext);

    babelwires::MapValue chordMap = getTestChordMap(testEnvironment.m_typeSystem, SourceMode::SilenceToChord, TargetMode::ChordToSilence, WildcardMode::Wildcards);

    seqwires::Track chordTrack = getTestInputTrack();

    seqwires::Track noteTrack;

    testUtils::addNotes(
    {
        {60, babelwires::Rational(1, 4), babelwires::Rational(1, 4)},
        {62, babelwires::Rational(1, 4), babelwires::Rational(1, 4)},
        {64, babelwires::Rational(0, 4), babelwires::Rational(3, 4)},
        {65, babelwires::Rational(0, 4), babelwires::Rational(3, 4)},
        {67, babelwires::Rational(1, 4), babelwires::Rational(1, 4)},
        {69, babelwires::Rational(1, 4), babelwires::Rational(1, 4)},
        {71, babelwires::Rational(0, 4), babelwires::Rational(3, 4)},
        {72, babelwires::Rational(1, 4), babelwires::Rational(1, 4)},
    }, noteTrack);

    seqwires::Track preCombinedInput = seqwires::mergeTracks({&noteTrack, &chordTrack});
    seqwires::Track preCombinedOutput = seqwires::mapChordsFunction(testEnvironment.m_typeSystem, preCombinedInput, chordMap);

    seqwires::Track mappedChords = seqwires::mapChordsFunction(testEnvironment.m_typeSystem, chordTrack, chordMap);
    seqwires::Track postCombinedOutput = seqwires::mergeTracks({&noteTrack, &mappedChords});

    // This test exploits the fact that the the resulting events end up in the same relative order.
    // That's not guaranteed, and the test may have to sanitize both sides if that changes.
    EXPECT_EQ(preCombinedOutput, postCombinedOutput);
}

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
        input.getChildFromStep(seqwires::ChordMapProcessor::getCommonArrayId())
            .is<babelwires::ValueTreeNode>();
    const babelwires::ValueTreeNode& outputArray =
        output.getChildFromStep(seqwires::ChordMapProcessor::getCommonArrayId())
            .is<babelwires::ValueTreeNode>();

    babelwires::ArrayInstanceImpl<babelwires::ValueTreeNode, seqwires::TrackType> inArray(inputArray);
    const babelwires::ArrayInstanceImpl<const babelwires::ValueTreeNode, seqwires::TrackType> outArray(outputArray);

    seqwires::ChordMapProcessorInput::Instance in(input);

    EXPECT_EQ(inArray.getSize(), 1);
    EXPECT_EQ(outArray.getSize(), 1);

    EXPECT_EQ(inArray.getEntry(0).get().getDuration(), 0);
    EXPECT_EQ(outArray.getEntry(0).get().getDuration(), 0);

    in.getChrdMp()->setValue(getTestChordMap(testEnvironment.m_typeSystem, SourceMode::ChordToChord, TargetMode::ChordToChord, WildcardMode::NoWildcards));
    inArray.getEntry(0).set(getTestInputTrack());

    processor.process(testEnvironment.m_log);

    testOutputTrack(outArray.getEntry(0).get(), SourceMode::ChordToChord, TargetMode::ChordToChord, WildcardMode::NoWildcards);
}


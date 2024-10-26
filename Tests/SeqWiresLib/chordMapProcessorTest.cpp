#include <gtest/gtest.h>

#include <SeqWiresLib/Functions/mapChordsFunction.hpp>
#include <SeqWiresLib/Processors/chordMapProcessor.hpp>
#include <SeqWiresLib/Types/Track/TrackEvents/chordEvents.hpp>
#include <SeqWiresLib/Types/Track/track.hpp>
#include <SeqWiresLib/Types/Track/trackType.hpp>
#include <SeqWiresLib/chord.hpp>
#include <SeqWiresLib/libRegistration.hpp>

#include <BabelWiresLib/TypeSystem/typeSystem.hpp>
#include <BabelWiresLib/Types/Enum/addBlankToEnum.hpp>
#include <BabelWiresLib/Types/Enum/enumValue.hpp>
#include <BabelWiresLib/Types/Map/MapEntries/allToSameFallbackMapEntryData.hpp>
#include <BabelWiresLib/Types/Map/MapEntries/oneToOneMapEntryData.hpp>
#include <BabelWiresLib/Types/Map/mapValue.hpp>

#include <Tests/BabelWiresLib/TestUtils/testEnvironment.hpp>
#include <Tests/TestUtils/seqTestUtils.hpp>
#include <Tests/TestUtils/testLog.hpp>

namespace {
    enum class Mode { NoBlanks, SourceBlanks, TargetBlanks, SourceAndTargetBlanks };

    babelwires::MapValue getTestChordTypeMap(const babelwires::TypeSystem& typeSystem, Mode mode = Mode::NoBlanks) {
        const seqwires::ChordType& chordTypeEnum = typeSystem.getEntryByType<seqwires::ChordType>();

        babelwires::MapValue chordTypeMap;
        chordTypeMap.setSourceTypeRef(seqwires::getMapChordFunctionChordTypeRef());
        chordTypeMap.setTargetTypeRef(seqwires::getMapChordFunctionChordTypeRef());

        {
            babelwires::OneToOneMapEntryData chordTypeMaplet(typeSystem, seqwires::getMapChordFunctionChordTypeRef(),
                                                             seqwires::getMapChordFunctionChordTypeRef());

            babelwires::EnumValue chordTypeSourceValue;
            chordTypeSourceValue.set(chordTypeEnum.getIdentifierFromValue(seqwires::ChordType::Value::m));

            babelwires::EnumValue chordTypeTargetValue;
            chordTypeTargetValue.set(chordTypeEnum.getIdentifierFromValue(seqwires::ChordType::Value::m7));

            chordTypeMaplet.setSourceValue(chordTypeSourceValue);
            chordTypeMaplet.setTargetValue(chordTypeTargetValue);

            chordTypeMap.emplaceBack(chordTypeMaplet.clone());
        }
        if ((mode == Mode::TargetBlanks) || (mode == Mode::SourceAndTargetBlanks)) {
            babelwires::OneToOneMapEntryData chordTypeMaplet(typeSystem, seqwires::getMapChordFunctionChordTypeRef(),
                                                             seqwires::getMapChordFunctionChordTypeRef());

            babelwires::EnumValue chordTypeSourceValue;
            chordTypeSourceValue.set(chordTypeEnum.getIdentifierFromValue(seqwires::ChordType::Value::M6));

            babelwires::EnumValue chordTypeTargetValue;
            chordTypeTargetValue.set(babelwires::AddBlankToEnum::getBlankValue());

            chordTypeMaplet.setSourceValue(chordTypeSourceValue);
            chordTypeMaplet.setTargetValue(chordTypeTargetValue);

            chordTypeMap.emplaceBack(chordTypeMaplet.clone());
        }
        if ((mode == Mode::SourceBlanks) || (mode == Mode::SourceAndTargetBlanks)) {
            babelwires::OneToOneMapEntryData chordTypeMaplet(typeSystem, seqwires::getMapChordFunctionChordTypeRef(),
                                                             seqwires::getMapChordFunctionChordTypeRef());

            babelwires::EnumValue chordTypeSourceValue;
            chordTypeSourceValue.set(babelwires::AddBlankToEnum::getBlankValue());

            babelwires::EnumValue chordTypeTargetValue;
            chordTypeTargetValue.set(chordTypeEnum.getIdentifierFromValue(seqwires::ChordType::Value::m7_11));

            chordTypeMaplet.setSourceValue(chordTypeSourceValue);
            chordTypeMaplet.setTargetValue(chordTypeTargetValue);

            chordTypeMap.emplaceBack(chordTypeMaplet.clone());
        }
        chordTypeMap.emplaceBack(std::make_unique<babelwires::AllToSameFallbackMapEntryData>());
        return chordTypeMap;
    }

    babelwires::MapValue getTestPitchClassMap(const babelwires::TypeSystem& typeSystem, Mode mode = Mode::NoBlanks) {
        const seqwires::PitchClass& pitchClassEnum =
            typeSystem.getEntryByType<seqwires::PitchClass>().is<seqwires::PitchClass>();

        babelwires::MapValue pitchClassMap;
        pitchClassMap.setSourceTypeRef(seqwires::getMapChordFunctionPitchClassRef());
        pitchClassMap.setTargetTypeRef(seqwires::getMapChordFunctionPitchClassRef());

        {
            babelwires::OneToOneMapEntryData chordTypeMaplet(typeSystem, seqwires::getMapChordFunctionPitchClassRef(),
                                                             seqwires::getMapChordFunctionPitchClassRef());

            babelwires::EnumValue pitchClassSourceValue;
            pitchClassSourceValue.set(pitchClassEnum.getIdentifierFromValue(seqwires::PitchClass::Value::D));

            babelwires::EnumValue pitchClassTargetValue;
            pitchClassTargetValue.set(pitchClassEnum.getIdentifierFromValue(seqwires::PitchClass::Value::A));

            chordTypeMaplet.setSourceValue(pitchClassSourceValue);
            chordTypeMaplet.setTargetValue(pitchClassTargetValue);

            pitchClassMap.emplaceBack(chordTypeMaplet.clone());
        }
        if ((mode == Mode::TargetBlanks) || (mode == Mode::SourceAndTargetBlanks)) {
            babelwires::OneToOneMapEntryData chordTypeMaplet(typeSystem, seqwires::getMapChordFunctionPitchClassRef(),
                                                             seqwires::getMapChordFunctionPitchClassRef());

            babelwires::EnumValue pitchClassSourceValue;
            pitchClassSourceValue.set(pitchClassEnum.getIdentifierFromValue(seqwires::PitchClass::Value::Gsh));

            babelwires::EnumValue pitchClassTargetValue;
            pitchClassTargetValue.set(babelwires::AddBlankToEnum::getBlankValue());

            chordTypeMaplet.setSourceValue(pitchClassSourceValue);
            chordTypeMaplet.setTargetValue(pitchClassTargetValue);

            pitchClassMap.emplaceBack(chordTypeMaplet.clone());
        }
        if ((mode == Mode::SourceBlanks) || (mode == Mode::SourceAndTargetBlanks)) {
            babelwires::OneToOneMapEntryData chordTypeMaplet(typeSystem, seqwires::getMapChordFunctionPitchClassRef(),
                                                             seqwires::getMapChordFunctionPitchClassRef());

            babelwires::EnumValue pitchClassSourceValue;
            pitchClassSourceValue.set(babelwires::AddBlankToEnum::getBlankValue());

            babelwires::EnumValue pitchClassTargetValue;
            pitchClassTargetValue.set(pitchClassEnum.getIdentifierFromValue(seqwires::PitchClass::Value::Fsh));

            chordTypeMaplet.setSourceValue(pitchClassSourceValue);
            chordTypeMaplet.setTargetValue(pitchClassTargetValue);

            pitchClassMap.emplaceBack(chordTypeMaplet.clone());
        }

        pitchClassMap.emplaceBack(std::make_unique<babelwires::AllToSameFallbackMapEntryData>());
        return pitchClassMap;
    }

    seqwires::Track getTestInputTrack() {
        seqwires::Track track;

        testUtils::addChords({{seqwires::PitchClass::Value::C, seqwires::ChordType::ChordType::Value::M,
                               babelwires::Rational(1, 2), babelwires::Rational(1, 2)},
                              {seqwires::PitchClass::Value::D, seqwires::ChordType::ChordType::Value::M},
                              {seqwires::PitchClass::Value::C, seqwires::ChordType::ChordType::Value::M6},
                              {seqwires::PitchClass::Value::C, seqwires::ChordType::ChordType::Value::m},
                              {seqwires::PitchClass::Value::D, seqwires::ChordType::ChordType::Value::m}},
                             track);

        track.setDuration(babelwires::Rational(7, 2));

        return track;
    }

    void testOutputTrack(const seqwires::Track& outputTrack, Mode mode = Mode::NoBlanks) {
        EXPECT_EQ(outputTrack.getDuration(), babelwires::Rational(7, 2));

        if (mode == Mode::NoBlanks) {
            testUtils::testChords({{seqwires::PitchClass::Value::C, seqwires::ChordType::ChordType::Value::M,
                                    babelwires::Rational(1, 2), babelwires::Rational(1, 2)},
                                   {seqwires::PitchClass::Value::A, seqwires::ChordType::ChordType::Value::M},
                                   {seqwires::PitchClass::Value::C, seqwires::ChordType::ChordType::Value::M6},
                                   {seqwires::PitchClass::Value::C, seqwires::ChordType::ChordType::Value::m7},
                                   {seqwires::PitchClass::Value::A, seqwires::ChordType::ChordType::Value::m7}},
                                  outputTrack);
        } else if (mode == Mode::TargetBlanks) {
            testUtils::testChords({{seqwires::PitchClass::Value::C, seqwires::ChordType::ChordType::Value::M,
                                    babelwires::Rational(1, 2), babelwires::Rational(1, 2)},
                                   {seqwires::PitchClass::Value::A, seqwires::ChordType::ChordType::Value::M},
                                   {seqwires::PitchClass::Value::C, seqwires::ChordType::ChordType::Value::m7,
                                    babelwires::Rational(1, 2), babelwires::Rational(1, 2)},
                                   {seqwires::PitchClass::Value::A, seqwires::ChordType::ChordType::Value::m7}},
                                  outputTrack);
        } else if (mode == Mode::SourceBlanks) {
            testUtils::testChords({{seqwires::PitchClass::Value::Fsh, seqwires::ChordType::ChordType::Value::m7_11},
                                   {seqwires::PitchClass::Value::C, seqwires::ChordType::ChordType::Value::M},
                                   {seqwires::PitchClass::Value::A, seqwires::ChordType::ChordType::Value::M},
                                   {seqwires::PitchClass::Value::C, seqwires::ChordType::ChordType::Value::M6},
                                   {seqwires::PitchClass::Value::C, seqwires::ChordType::ChordType::Value::m7},
                                   {seqwires::PitchClass::Value::A, seqwires::ChordType::ChordType::Value::m7},
                                   {seqwires::PitchClass::Value::Fsh, seqwires::ChordType::ChordType::Value::m7_11}},
                                  outputTrack);
        } else if (mode == Mode::SourceAndTargetBlanks) {
            testUtils::testChords({{seqwires::PitchClass::Value::Fsh, seqwires::ChordType::ChordType::Value::m7_11},
                                   {seqwires::PitchClass::Value::C, seqwires::ChordType::ChordType::Value::M},
                                   {seqwires::PitchClass::Value::A, seqwires::ChordType::ChordType::Value::M},
                                   {seqwires::PitchClass::Value::Fsh, seqwires::ChordType::ChordType::Value::m7_11},
                                   {seqwires::PitchClass::Value::C, seqwires::ChordType::ChordType::Value::m7},
                                   {seqwires::PitchClass::Value::A, seqwires::ChordType::ChordType::Value::m7},
                                   {seqwires::PitchClass::Value::Fsh, seqwires::ChordType::ChordType::Value::m7_11}},
                                  outputTrack);
        }
    }
} // namespace

TEST(ChordMapProcessorTest, simpleFunction) {
    testUtils::TestLog log;

    babelwires::TypeSystem typeSystem;
    typeSystem.addEntry<seqwires::ChordType>();
    typeSystem.addEntry<seqwires::PitchClass>();
    typeSystem.addTypeConstructor<babelwires::AddBlankToEnum>();

    for (auto mode :
         std::array<Mode, 4>{Mode::NoBlanks, Mode::TargetBlanks, Mode::SourceBlanks, Mode::SourceAndTargetBlanks}) {
        babelwires::MapValue chordTypeMap = getTestChordTypeMap(typeSystem, mode);
        babelwires::MapValue pitchClassMap = getTestPitchClassMap(typeSystem, mode);

        seqwires::Track inputTrack = getTestInputTrack();
        seqwires::Track outputTrack = seqwires::mapChordsFunction(typeSystem, inputTrack, chordTypeMap, pitchClassMap);
        testOutputTrack(outputTrack, mode);
    }
}

TEST(ChordMapProcessorTest, processor) {
    testUtils::TestEnvironment testEnvironment;
    seqwires::registerLib(testEnvironment.m_projectContext);

    const seqwires::ChordType& chordTypeEnum =
        testEnvironment.m_typeSystem.getEntryByType<seqwires::ChordType>().is<seqwires::ChordType>();
    const seqwires::PitchClass& pitchClassEnum =
        testEnvironment.m_typeSystem.getEntryByType<seqwires::PitchClass>().is<seqwires::PitchClass>();

    seqwires::ChordMapProcessor processor(testEnvironment.m_projectContext);

    processor.getInputFeature().setToDefault();
    processor.getOutputFeature().setToDefault();

    babelwires::ValueTreeNode& inputValueFeature = processor.getInputFeature();
    const babelwires::ValueTreeNode& outputValueFeature = processor.getOutputFeature();

    babelwires::ValueTreeNode& inputArrayFeature =
        inputValueFeature.getChildFromStep(babelwires::PathStep(seqwires::ChordMapProcessor::getCommonArrayId()))
            .is<babelwires::ValueTreeNode>();
    const babelwires::ValueTreeNode& outputArrayFeature =
        outputValueFeature.getChildFromStep(babelwires::PathStep(seqwires::ChordMapProcessor::getCommonArrayId()))
            .is<babelwires::ValueTreeNode>();

    babelwires::ArrayInstanceImpl<babelwires::ValueTreeNode, seqwires::TrackType> inputArray(inputArrayFeature);
    const babelwires::ArrayInstanceImpl<const babelwires::ValueTreeNode, seqwires::TrackType> outputArray(
        outputArrayFeature);

    seqwires::ChordMapProcessorInput::Instance input(inputValueFeature);

    EXPECT_EQ(inputArray.getSize(), 1);
    EXPECT_EQ(outputArray.getSize(), 1);

    EXPECT_EQ(inputArray.getEntry(0).get().getDuration(), 0);
    EXPECT_EQ(outputArray.getEntry(0).get().getDuration(), 0);

    input.getTypMap()->setValue(getTestChordTypeMap(testEnvironment.m_typeSystem));
    input.getRtMap()->setValue(getTestPitchClassMap(testEnvironment.m_typeSystem));
    inputArray.getEntry(0).set(getTestInputTrack());

    processor.process(testEnvironment.m_log);

    testOutputTrack(outputArray.getEntry(0).get());
}

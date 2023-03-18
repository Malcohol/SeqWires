#include <gtest/gtest.h>

#include <SeqWiresLib/Functions/mapChordsFunction.hpp>
#include <SeqWiresLib/Processors/chordMapProcessor.hpp>
#include <SeqWiresLib/Tracks/chordEvents.hpp>
#include <SeqWiresLib/Tracks/track.hpp>
#include <SeqWiresLib/chord.hpp>

#include <BabelWiresLib/Enums/addBlankToEnum.hpp>
#include <BabelWiresLib/Features/mapFeature.hpp>
#include <BabelWiresLib/Maps/MapEntries/allToSameFallbackMapEntryData.hpp>
#include <BabelWiresLib/Maps/MapEntries/oneToOneMapEntryData.hpp>
#include <BabelWiresLib/Maps/mapData.hpp>
#include <BabelWiresLib/Enums/enumValue.hpp>
#include <BabelWiresLib/TypeSystem/typeSystem.hpp>

#include <Tests/BabelWiresLib/TestUtils/testEnvironment.hpp>
#include <Tests/TestUtils/seqTestUtils.hpp>
#include <Tests/TestUtils/testLog.hpp>

namespace {
    enum class Mode { NoBlanks, SourceBlanks, TargetBlanks, SourceAndTargetBlanks };

    babelwires::MapData getTestChordTypeMap(const babelwires::TypeSystem& typeSystem, Mode mode = Mode::NoBlanks) {
        const seqwires::ChordType& chordTypeEnum = typeSystem.getEntryByType<seqwires::ChordType>();

        babelwires::MapData chordTypeMap;
        chordTypeMap.setSourceTypeRef(seqwires::getMapChordFunctionChordTypeRef());
        chordTypeMap.setTargetTypeRef(seqwires::getMapChordFunctionChordTypeRef());

        {
            babelwires::OneToOneMapEntryData chordTypeMaplet(typeSystem, seqwires::getMapChordFunctionChordTypeRef(),
                                                             seqwires::getMapChordFunctionChordTypeRef());

            babelwires::EnumValue chordTypeSourceValue;
            chordTypeSourceValue.set(chordTypeEnum.getIdentifierFromValue(seqwires::ChordType::Value::m));

            babelwires::EnumValue chordTypeTargetValue;
            chordTypeTargetValue.set(chordTypeEnum.getIdentifierFromValue(seqwires::ChordType::Value::m7));

            chordTypeMaplet.setSourceValue(chordTypeSourceValue.clone());
            chordTypeMaplet.setTargetValue(chordTypeTargetValue.clone());

            chordTypeMap.emplaceBack(chordTypeMaplet.clone());
        }
        if ((mode == Mode::TargetBlanks) || (mode == Mode::SourceAndTargetBlanks)) {
            babelwires::OneToOneMapEntryData chordTypeMaplet(typeSystem, seqwires::getMapChordFunctionChordTypeRef(),
                                                             seqwires::getMapChordFunctionChordTypeRef());

            babelwires::EnumValue chordTypeSourceValue;
            chordTypeSourceValue.set(chordTypeEnum.getIdentifierFromValue(seqwires::ChordType::Value::M6));

            babelwires::EnumValue chordTypeTargetValue;
            chordTypeTargetValue.set(babelwires::AddBlankToEnum::getBlankValue());

            chordTypeMaplet.setSourceValue(chordTypeSourceValue.clone());
            chordTypeMaplet.setTargetValue(chordTypeTargetValue.clone());

            chordTypeMap.emplaceBack(chordTypeMaplet.clone());
        }
        if ((mode == Mode::SourceBlanks) || (mode == Mode::SourceAndTargetBlanks)) {
            babelwires::OneToOneMapEntryData chordTypeMaplet(typeSystem, seqwires::getMapChordFunctionChordTypeRef(),
                                                             seqwires::getMapChordFunctionChordTypeRef());

            babelwires::EnumValue chordTypeSourceValue;
            chordTypeSourceValue.set(babelwires::AddBlankToEnum::getBlankValue());

            babelwires::EnumValue chordTypeTargetValue;
            chordTypeTargetValue.set(chordTypeEnum.getIdentifierFromValue(seqwires::ChordType::Value::m7_11));

            chordTypeMaplet.setSourceValue(chordTypeSourceValue.clone());
            chordTypeMaplet.setTargetValue(chordTypeTargetValue.clone());

            chordTypeMap.emplaceBack(chordTypeMaplet.clone());
        }
        chordTypeMap.emplaceBack(std::make_unique<babelwires::AllToSameFallbackMapEntryData>());
        return chordTypeMap;
    }

    babelwires::MapData getTestPitchClassMap(const babelwires::TypeSystem& typeSystem, Mode mode = Mode::NoBlanks) {
        const seqwires::PitchClass& pitchClassEnum =
            typeSystem.getEntryByType<seqwires::PitchClass>().is<seqwires::PitchClass>();

        babelwires::MapData pitchClassMap;
        pitchClassMap.setSourceTypeRef(seqwires::getMapChordFunctionPitchClassRef());
        pitchClassMap.setTargetTypeRef(seqwires::getMapChordFunctionPitchClassRef());

        {
            babelwires::OneToOneMapEntryData chordTypeMaplet(typeSystem, seqwires::getMapChordFunctionPitchClassRef(),
                                                             seqwires::getMapChordFunctionPitchClassRef());

            babelwires::EnumValue pitchClassSourceValue;
            pitchClassSourceValue.set(pitchClassEnum.getIdentifierFromValue(seqwires::PitchClass::Value::D));

            babelwires::EnumValue pitchClassTargetValue;
            pitchClassTargetValue.set(pitchClassEnum.getIdentifierFromValue(seqwires::PitchClass::Value::A));

            chordTypeMaplet.setSourceValue(pitchClassSourceValue.clone());
            chordTypeMaplet.setTargetValue(pitchClassTargetValue.clone());

            pitchClassMap.emplaceBack(chordTypeMaplet.clone());
        }
        if ((mode == Mode::TargetBlanks) || (mode == Mode::SourceAndTargetBlanks)) {
            babelwires::OneToOneMapEntryData chordTypeMaplet(typeSystem, seqwires::getMapChordFunctionPitchClassRef(),
                                                             seqwires::getMapChordFunctionPitchClassRef());

            babelwires::EnumValue pitchClassSourceValue;
            pitchClassSourceValue.set(pitchClassEnum.getIdentifierFromValue(seqwires::PitchClass::Value::GSharp));

            babelwires::EnumValue pitchClassTargetValue;
            pitchClassTargetValue.set(babelwires::AddBlankToEnum::getBlankValue());

            chordTypeMaplet.setSourceValue(pitchClassSourceValue.clone());
            chordTypeMaplet.setTargetValue(pitchClassTargetValue.clone());

            pitchClassMap.emplaceBack(chordTypeMaplet.clone());
        }
        if ((mode == Mode::SourceBlanks) || (mode == Mode::SourceAndTargetBlanks)) {
            babelwires::OneToOneMapEntryData chordTypeMaplet(typeSystem, seqwires::getMapChordFunctionPitchClassRef(),
                                                             seqwires::getMapChordFunctionPitchClassRef());

            babelwires::EnumValue pitchClassSourceValue;
            pitchClassSourceValue.set(babelwires::AddBlankToEnum::getBlankValue());

            babelwires::EnumValue pitchClassTargetValue;
            pitchClassTargetValue.set(pitchClassEnum.getIdentifierFromValue(seqwires::PitchClass::Value::FSharp));

            chordTypeMaplet.setSourceValue(pitchClassSourceValue.clone());
            chordTypeMaplet.setTargetValue(pitchClassTargetValue.clone());

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
            testUtils::testChords({{seqwires::PitchClass::Value::FSharp, seqwires::ChordType::ChordType::Value::m7_11},
                                   {seqwires::PitchClass::Value::C, seqwires::ChordType::ChordType::Value::M},
                                   {seqwires::PitchClass::Value::A, seqwires::ChordType::ChordType::Value::M},
                                   {seqwires::PitchClass::Value::C, seqwires::ChordType::ChordType::Value::M6},
                                   {seqwires::PitchClass::Value::C, seqwires::ChordType::ChordType::Value::m7},
                                   {seqwires::PitchClass::Value::A, seqwires::ChordType::ChordType::Value::m7},
                                   {seqwires::PitchClass::Value::FSharp, seqwires::ChordType::ChordType::Value::m7_11}},
                                  outputTrack);
        } else if (mode == Mode::SourceAndTargetBlanks) {
            testUtils::testChords({{seqwires::PitchClass::Value::FSharp, seqwires::ChordType::ChordType::Value::m7_11},
                                   {seqwires::PitchClass::Value::C, seqwires::ChordType::ChordType::Value::M},
                                   {seqwires::PitchClass::Value::A, seqwires::ChordType::ChordType::Value::M},
                                   {seqwires::PitchClass::Value::FSharp, seqwires::ChordType::ChordType::Value::m7_11},
                                   {seqwires::PitchClass::Value::C, seqwires::ChordType::ChordType::Value::m7},
                                   {seqwires::PitchClass::Value::A, seqwires::ChordType::ChordType::Value::m7},
                                   {seqwires::PitchClass::Value::FSharp, seqwires::ChordType::ChordType::Value::m7_11}},
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
        babelwires::MapData chordTypeMap = getTestChordTypeMap(typeSystem, mode);
        babelwires::MapData pitchClassMap = getTestPitchClassMap(typeSystem, mode);

        seqwires::Track inputTrack = getTestInputTrack();
        seqwires::Track outputTrack = seqwires::mapChordsFunction(typeSystem, inputTrack, chordTypeMap, pitchClassMap);
        testOutputTrack(outputTrack, mode);
    }
}

TEST(ChordMapProcessorTest, processor) {
    testUtils::TestEnvironment testEnvironment;
    testEnvironment.m_typeSystem.addEntry<seqwires::ChordType>();
    testEnvironment.m_typeSystem.addEntry<seqwires::PitchClass>();
    testEnvironment.m_typeSystem.addTypeConstructor<babelwires::AddBlankToEnum>();

    const seqwires::ChordType& chordTypeEnum =
        testEnvironment.m_typeSystem.getEntryByType<seqwires::ChordType>().is<seqwires::ChordType>();
    const seqwires::PitchClass& pitchClassEnum =
        testEnvironment.m_typeSystem.getEntryByType<seqwires::PitchClass>().is<seqwires::PitchClass>();

    seqwires::ChordMapProcessor processor(testEnvironment.m_projectContext);

    processor.getInputFeature()->setToDefault();
    processor.getOutputFeature()->setToDefault();

    auto* chordTypeMapFeature =
        processor.getInputFeature()->getChildFromStep(babelwires::PathStep("TypMap")).as<babelwires::MapFeature>();
    auto* pitchClassMapFeature =
        processor.getInputFeature()->getChildFromStep(babelwires::PathStep("RtMap")).as<babelwires::MapFeature>();
    auto* inputArray =
        processor.getInputFeature()->getChildFromStep(babelwires::PathStep("Tracks")).as<babelwires::ArrayFeature>();
    auto* outputArray =
        processor.getOutputFeature()->getChildFromStep(babelwires::PathStep("Tracks")).as<babelwires::ArrayFeature>();
    ASSERT_NE(chordTypeMapFeature, nullptr);
    ASSERT_NE(pitchClassMapFeature, nullptr);
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

    chordTypeMapFeature->set(getTestChordTypeMap(testEnvironment.m_typeSystem));
    pitchClassMapFeature->set(getTestPitchClassMap(testEnvironment.m_typeSystem));
    getInputTrack(0)->set(getTestInputTrack());

    processor.process(testEnvironment.m_log);

    testOutputTrack(getOutputTrack(0)->get());
}

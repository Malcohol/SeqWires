#include <gtest/gtest.h>

#include <SeqWiresLib/Functions/mapChordsFunction.hpp>
#include <SeqWiresLib/Processors/chordMapProcessor.hpp>
#include <SeqWiresLib/Tracks/chordEvents.hpp>
#include <SeqWiresLib/Tracks/track.hpp>
#include <SeqWiresLib/chord.hpp>

#include <BabelWiresLib/Enums/addBlank.hpp>
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
    babelwires::MapData getTestChordTypeMap(const babelwires::TypeSystem& typeSystem) {
        const seqwires::ChordType& chordTypeEnum = typeSystem.getEntryByType<seqwires::ChordType>();

        babelwires::MapData chordTypeMap;
        chordTypeMap.setSourceTypeId(seqwires::getMapChordFunctionTypeRef());
        chordTypeMap.setTargetTypeId(seqwires::getMapChordFunctionTypeRef());

        {
            babelwires::OneToOneMapEntryData chordTypeMaplet(typeSystem, seqwires::getMapChordFunctionTypeRef(),
                                                             seqwires::getMapChordFunctionTypeRef());

            babelwires::EnumValue chordTypeSourceValue;
            chordTypeSourceValue.set(chordTypeEnum.getIdentifierFromValue(seqwires::ChordType::Value::m));

            babelwires::EnumValue chordTypeTargetValue;
            chordTypeTargetValue.set(chordTypeEnum.getIdentifierFromValue(seqwires::ChordType::Value::m7));

            chordTypeMaplet.setSourceValue(chordTypeSourceValue.clone());
            chordTypeMaplet.setTargetValue(chordTypeTargetValue.clone());

            chordTypeMap.emplaceBack(chordTypeMaplet.clone());
        }
        {
            babelwires::OneToOneMapEntryData chordTypeMaplet(typeSystem, seqwires::getMapChordFunctionTypeRef(),
                                                             seqwires::getMapChordFunctionTypeRef());

            babelwires::EnumValue chordTypeSourceValue;
            chordTypeSourceValue.set(chordTypeEnum.getIdentifierFromValue(seqwires::ChordType::Value::M6));

            babelwires::EnumValue chordTypeTargetValue;
            chordTypeTargetValue.set(babelwires::AddBlank::getBlankValue());

            chordTypeMaplet.setSourceValue(chordTypeSourceValue.clone());
            chordTypeMaplet.setTargetValue(chordTypeTargetValue.clone());

            chordTypeMap.emplaceBack(chordTypeMaplet.clone());
        }
        chordTypeMap.emplaceBack(std::make_unique<babelwires::AllToSameFallbackMapEntryData>());
        return chordTypeMap;
    }

    babelwires::MapData getTestPitchClassMap(const babelwires::TypeSystem& typeSystem) {
        const seqwires::PitchClass& pitchClassEnum =
            typeSystem.getEntryByType<seqwires::PitchClass>().is<seqwires::PitchClass>();

        babelwires::MapData pitchClassMap;
        pitchClassMap.setSourceTypeId(seqwires::PitchClass::getThisIdentifier());
        pitchClassMap.setTargetTypeId(seqwires::PitchClass::getThisIdentifier());

        babelwires::OneToOneMapEntryData chordTypeMaplet(typeSystem, seqwires::PitchClass::getThisIdentifier(),
                                                         seqwires::PitchClass::getThisIdentifier());

        babelwires::EnumValue pitchClassSourceValue;
        pitchClassSourceValue.set(pitchClassEnum.getIdentifierFromValue(seqwires::PitchClass::Value::D));

        babelwires::EnumValue pitchClassTargetValue;
        pitchClassTargetValue.set(pitchClassEnum.getIdentifierFromValue(seqwires::PitchClass::Value::A));

        chordTypeMaplet.setSourceValue(pitchClassSourceValue.clone());
        chordTypeMaplet.setTargetValue(pitchClassTargetValue.clone());

        pitchClassMap.emplaceBack(chordTypeMaplet.clone());
        pitchClassMap.emplaceBack(std::make_unique<babelwires::AllToSameFallbackMapEntryData>());
        return pitchClassMap;
    }

    seqwires::Track getTestInputTrack() {
        seqwires::Track track;

        testUtils::addChords({{seqwires::PitchClass::Value::C, seqwires::ChordType::ChordType::Value::M},
                              {seqwires::PitchClass::Value::D, seqwires::ChordType::ChordType::Value::M},
                              {seqwires::PitchClass::Value::C, seqwires::ChordType::ChordType::Value::M6},
                              {seqwires::PitchClass::Value::C, seqwires::ChordType::ChordType::Value::m},
                              {seqwires::PitchClass::Value::D, seqwires::ChordType::ChordType::Value::m}},
                             track);

        return track;
    }

    void testOutputTrack(const seqwires::Track& outputTrack) {
        testUtils::testChords({{seqwires::PitchClass::Value::C, seqwires::ChordType::ChordType::Value::M},
                               {seqwires::PitchClass::Value::A, seqwires::ChordType::ChordType::Value::M},
                               {seqwires::PitchClass::Value::C, seqwires::ChordType::ChordType::Value::m7, babelwires::Rational(1, 2), babelwires::Rational(1, 2)},
                               {seqwires::PitchClass::Value::A, seqwires::ChordType::ChordType::Value::m7}},
                              outputTrack);
    }
} // namespace

TEST(ChordMapProcessorTest, simpleFunction) {
    testUtils::TestLog log;

    babelwires::TypeSystem typeSystem;
    typeSystem.addEntry<seqwires::ChordType>();
    typeSystem.addEntry<seqwires::PitchClass>();
    typeSystem.addTypeConstructor<babelwires::AddBlank>();

    babelwires::MapData chordTypeMap = getTestChordTypeMap(typeSystem);
    babelwires::MapData pitchClassMap = getTestPitchClassMap(typeSystem);

    seqwires::Track inputTrack = getTestInputTrack();

    seqwires::Track outputTrack = seqwires::mapChordsFunction(typeSystem, inputTrack, chordTypeMap, pitchClassMap);

    testOutputTrack(outputTrack);
    EXPECT_EQ(inputTrack.getDuration(), outputTrack.getDuration());
}

TEST(ChordMapProcessorTest, processor) {
    testUtils::TestEnvironment testEnvironment;
    testEnvironment.m_typeSystem.addEntry<seqwires::ChordType>();
    testEnvironment.m_typeSystem.addEntry<seqwires::PitchClass>();
    testEnvironment.m_typeSystem.addTypeConstructor<babelwires::AddBlank>();

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

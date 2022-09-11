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
        const seqwires::GM2Percussion& percussionType =
            typeSystem.getRegisteredEntry(seqwires::GM2Percussion::getThisIdentifier()).is<seqwires::GM2Percussion>();

        babelwires::MapData percussionMap;
        percussionMap.setSourceTypeId(seqwires::GMPercussion::getThisIdentifier());
        percussionMap.setTargetTypeId(seqwires::GMPercussion::getThisIdentifier());

        babelwires::OneToOneMapEntryData maplet(typeSystem, seqwires::GM2Percussion::getThisIdentifier(),
                                                seqwires::GM2Percussion::getThisIdentifier());

        babelwires::EnumValue sourceValue;
        babelwires::EnumValue targetValue;

        sourceValue.set(percussionType.getIdentifierFromValue(seqwires::GM2Percussion::Value::Clap));
        targetValue.set(percussionType.getIdentifierFromValue(seqwires::GM2Percussion::Value::Cowbll));
        maplet.setSourceValue(sourceValue.clone());
        maplet.setTargetValue(targetValue.clone());
        percussionMap.emplaceBack(maplet.clone());

        sourceValue.set(percussionType.getIdentifierFromValue(seqwires::GM2Percussion::Value::Crash1));
        targetValue.set(percussionType.getIdentifierFromValue(seqwires::GM2Percussion::Value::Crash2));
        maplet.setSourceValue(sourceValue.clone());
        maplet.setTargetValue(targetValue.clone());
        percussionMap.emplaceBack(maplet.clone());

        percussionMap.emplaceBack(std::make_unique<babelwires::AllToSameFallbackMapEntryData>());
        return percussionMap;
    }
} // namespace

TEST(PercussionMapProcessorTest, funcSimple) {
    testUtils::TestLog log;

    babelwires::TypeSystem typeSystem;
    typeSystem.addEntry(std::make_unique<seqwires::GM2Percussion>());
    typeSystem.addEntry(std::make_unique<seqwires::GMPercussion>());

    babelwires::MapData mapData = getTestPercussionMap(typeSystem);

    seqwires::Track inputTrack;

    testUtils::addNotes(
        {
            {seqwires::GM2Percussion::getPitchFromValue(seqwires::GM2Percussion::Value::AcBass), 0,
             babelwires::Rational(1, 2)},
            {seqwires::GM2Percussion::getPitchFromValue(seqwires::GM2Percussion::Value::Clap), 0,
             babelwires::Rational(1, 2)},
            {12, 0, babelwires::Rational(1, 2)},
            {seqwires::GM2Percussion::getPitchFromValue(seqwires::GM2Percussion::Value::Crash1), 0,
             babelwires::Rational(1, 2)},
        },
        inputTrack);

    seqwires::Track outputTrack = seqwires::mapPercussionFunction(typeSystem, inputTrack, mapData);

    EXPECT_EQ(inputTrack.getDuration(), outputTrack.getDuration());

    testUtils::testNotes(
        {
            {seqwires::GM2Percussion::getPitchFromValue(seqwires::GM2Percussion::Value::AcBass), 0,
             babelwires::Rational(1, 2)},
            {seqwires::GM2Percussion::getPitchFromValue(seqwires::GM2Percussion::Value::Cowbll), 0,
             babelwires::Rational(1, 2)},
            // The out of range note gets dropped here.
            {seqwires::GM2Percussion::getPitchFromValue(seqwires::GM2Percussion::Value::Crash2),
             babelwires::Rational(1, 2), babelwires::Rational(1, 2)},
        },
        outputTrack);
}

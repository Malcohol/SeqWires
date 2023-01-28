#include <gtest/gtest.h>

#include <SeqWiresLib/Percussion/builtInPercussionInstruments.hpp>
#include <SeqWiresLib/Percussion/percussionSet.hpp>

#include <BabelWiresLib/TypeSystem/typeSystem.hpp>

#include <Tests/BabelWiresLib/TestUtils/testEnvironment.hpp>
#include <Tests/TestUtils/equalSets.hpp>
#include <Tests/TestUtils/testIdentifiers.hpp>

TEST(PercussionSetTest, oneInstrumentBlockAllBuiltIn) {
    testUtils::TestEnvironment testEnvironment;
    const seqwires::BuiltInPercussionInstruments* const builtIns =
        testEnvironment.m_typeSystem.addEntry(std::make_unique<seqwires::BuiltInPercussionInstruments>());

    seqwires::PercussionSet::InstrumentBlock block = {{seqwires::BuiltInPercussionInstruments::Value::HBongo,
                                                       seqwires::BuiltInPercussionInstruments::Value::Claves,
                                                       seqwires::BuiltInPercussionInstruments::Value::Bass1},
                                                      40,
                                                      builtIns};

    seqwires::PercussionSet percussionSet(testUtils::getTestRegisteredLongIdentifier("Foo"), 12, block, 41);

    EXPECT_EQ(percussionSet.getIdentifier(), "Foo");
    EXPECT_EQ(percussionSet.getVersion(), 12);
    EXPECT_TRUE(testUtils::areEqualSets(percussionSet.getEnumValues(), {"HBongo", "Claves", "Bass1"}));
    EXPECT_EQ(percussionSet.getEnumValues().size(), 3);
    EXPECT_EQ(percussionSet.getEnumValues()[0], "HBongo");
    EXPECT_EQ(percussionSet.getEnumValues()[1], "Claves");
    EXPECT_EQ(percussionSet.getEnumValues()[2], "Bass1");
    EXPECT_EQ(percussionSet.getIndexOfDefaultValue(), 1);
    EXPECT_EQ(percussionSet.tryGetIndexFromIdentifier("HBongo"), 0);
    EXPECT_EQ(percussionSet.tryGetIndexFromIdentifier("Claves"), 1);
    EXPECT_EQ(percussionSet.tryGetIndexFromIdentifier("Bass1"), 2);
    EXPECT_EQ(percussionSet.tryGetPitchFromInstrument("HBongo"), 40);
    EXPECT_EQ(percussionSet.tryGetPitchFromInstrument("Claves"), 41);
    EXPECT_EQ(percussionSet.tryGetPitchFromInstrument("Bass1"), 42);
    EXPECT_FALSE(percussionSet.tryGetPitchFromInstrument("Flerm"));
    EXPECT_EQ(percussionSet.tryGetInstrumentFromPitch(40), "HBongo");
    EXPECT_EQ(percussionSet.tryGetInstrumentFromPitch(41), "Claves");
    EXPECT_EQ(percussionSet.tryGetInstrumentFromPitch(42), "Bass1");
    EXPECT_FALSE(percussionSet.tryGetInstrumentFromPitch(39));
    EXPECT_FALSE(percussionSet.tryGetInstrumentFromPitch(43));
}

#include <gtest/gtest.h>

#include <SeqWiresLib/Tracks/noteEvents.hpp>
#include <SeqWiresLib/Tracks/track.hpp>
#include <SeqWiresLib/Tracks/trackType.hpp>
#include <SeqWiresLib/Tracks/trackTypeConstructor.hpp>

#include <BabelWiresLib/Types/Rational/rationalValue.hpp>

#include <Tests/TestUtils/seqTestUtils.hpp>
#include <Tests/BabelWiresLib/TestUtils/testEnvironment.hpp>

TEST(TrackType, defaultTrackTypeCreateValue) {
    seqwires::DefaultTrackType trackType;

    babelwires::ValueHolder newValue = trackType.createValue();
    EXPECT_TRUE(newValue);

    const auto* const newTrackValue = newValue->as<seqwires::Track>();
    EXPECT_NE(newTrackValue, nullptr);
    EXPECT_EQ(newTrackValue->getDuration(), 0);
}

TEST(TrackType, defaultTrackTypeIsValidValue) {
    seqwires::DefaultTrackType trackType;

    seqwires::Track track(8);

    EXPECT_TRUE(trackType.isValidValue(track));
    EXPECT_FALSE(trackType.isValidValue(babelwires::RationalValue(3)));
}

TEST(TrackType, defaultIntTypeGetKind) {
    seqwires::DefaultTrackType trackType;

    EXPECT_FALSE(trackType.getKind().empty());
}

TEST(TrackType, constructedTrackTypeCreateValue) {
    testUtils::TestEnvironment testEnvironment;
    testEnvironment.m_typeSystem.addTypeConstructor<seqwires::TrackTypeConstructor>();

    babelwires::TypeRef trackTypeRef(seqwires::TrackTypeConstructor::getThisIdentifier(), babelwires::RationalValue(8));

    const babelwires::Type* const type = trackTypeRef.tryResolve(testEnvironment.m_typeSystem);

    babelwires::ValueHolder newValue = type->createValue();
    EXPECT_TRUE(newValue);

    const auto* const newTrack = newValue->as<seqwires::Track>();
    EXPECT_NE(newTrack, nullptr);
    EXPECT_EQ(newTrack->getDuration(), 8);
}
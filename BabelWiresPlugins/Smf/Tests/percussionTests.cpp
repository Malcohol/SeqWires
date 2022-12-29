#include <gtest/gtest.h>

#include <BabelWiresPlugins/Smf/Plugin/Percussion/gm2StandardPercussionSet.hpp>
#include <BabelWiresPlugins/Smf/Plugin/libRegistration.hpp>
#include <BabelWiresPlugins/Smf/Plugin/smfParser.hpp>
#include <BabelWiresPlugins/Smf/Plugin/smfSourceModel.hpp>
#include <BabelWiresPlugins/Smf/Plugin/smfTargetModel.hpp>
#include <BabelWiresPlugins/Smf/Plugin/smfWriter.hpp>

#include <SeqWiresLib/Features/trackFeature.hpp>
#include <SeqWiresLib/Tracks/noteEvents.hpp>
#include <SeqWiresLib/Tracks/percussionEvents.hpp>
#include <SeqWiresLib/Utilities/filteredTrackIterator.hpp>
#include <SeqWiresLib/libRegistration.hpp>

#include <Common/IO/fileDataSource.hpp>

#include <Tests/TestUtils/seqTestUtils.hpp>

#include <Tests/BabelWiresLib/TestUtils/testEnvironment.hpp>

#include <Tests/TestUtils/tempFilePath.hpp>

class SmfStandardPercussionTest : public testing::TestWithParam<const char*> {};

TEST_P(SmfStandardPercussionTest, saveLoad) {
    testUtils::TestEnvironment testEnvironment;
    seqwires::registerLib(testEnvironment.m_projectContext);
    smf::registerLib(testEnvironment.m_projectContext);

    const char* specification = GetParam();

    testUtils::TempFilePath tempFile("standardPercussion.mid", specification);

    {
        smf::target::SmfFeature smfFeature(testEnvironment.m_projectContext);
        smfFeature.setToDefault();

        auto* smfFormatFeature =
            smfFeature.getChildFromStep(babelwires::PathStep("Format")).as<smf::target::SmfFormatFeature>();
        ASSERT_NE(smfFormatFeature, nullptr);

        smfFormatFeature->getMidiMetadata().getSpecFeature()->set(babelwires::Identifier(specification));

        auto* tracks =
            smfFormatFeature->getChildFromStep(babelwires::PathStep("tracks")).as<babelwires::ArrayFeature>();

        EXPECT_EQ(tracks->getNumFeatures(), 1);

        auto* channelTrack = tracks->getFeature(0)->as<babelwires::RecordFeature>();
        ASSERT_NE(channelTrack, nullptr);
        EXPECT_EQ(channelTrack->getNumFeatures(), 2);

        auto* channelFeature =
            channelTrack->getChildFromStep(babelwires::PathStep("Chan")).as<babelwires::IntFeature>();
        channelFeature->set(9);

        ASSERT_NE(tracks, nullptr);

        auto* trackFeature = channelTrack->getChildFromStep(babelwires::PathStep("Track")).as<seqwires::TrackFeature>();

        auto track = std::make_unique<seqwires::Track>();

        track->addEvent(seqwires::PercussionOnEvent{0, "HTom"});
        track->addEvent(seqwires::PercussionOffEvent{babelwires::Rational(1, 4), "HTom"});
        track->addEvent(seqwires::PercussionOnEvent{0, "LMTom"});
        track->addEvent(seqwires::PercussionOffEvent{babelwires::Rational(1, 4), "LMTom"});
        track->addEvent(seqwires::PercussionOnEvent{0, "LwTom"});
        track->addEvent(seqwires::PercussionOffEvent{babelwires::Rational(1, 4), "LwTom"});

        trackFeature->set(std::move(track));

        std::ofstream os(tempFile);
        smf::writeToSmf(testEnvironment.m_projectContext, testEnvironment.m_log, smfFeature.getFormatFeature(), os);
    }

    {
        babelwires::FileDataSource midiFile(tempFile);

        const auto feature = smf::parseSmfSequence(midiFile, testEnvironment.m_projectContext, testEnvironment.m_log);
        ASSERT_NE(feature, nullptr);
        auto smfFeature = feature.get()->as<const smf::source::Format0SmfFeature>();
        ASSERT_NE(smfFeature, nullptr);

        EXPECT_EQ(smfFeature->getMidiMetadata().getSpecFeature()->get(), babelwires::Identifier(specification));

        EXPECT_EQ(smfFeature->getNumMidiTracks(), 1);
        const auto& channelGroup = dynamic_cast<const smf::source::RecordChannelGroup&>(smfFeature->getMidiTrack(0));
        EXPECT_EQ(channelGroup.getNumFeatures(), 1);
        const auto* trackFeature = channelGroup.getFeature(0)->as<const seqwires::TrackFeature>();
        ASSERT_NE(trackFeature, nullptr);
        ASSERT_EQ(channelGroup.getStepToChild(trackFeature), babelwires::PathStep(babelwires::Identifier("ch9")));

        const auto& track = trackFeature->get();

        auto categoryMap = track.getNumEventGroupsByCategory();
        EXPECT_EQ(categoryMap.find(seqwires::NoteEvent::s_noteEventCategory), categoryMap.end());
        EXPECT_NE(categoryMap.find(seqwires::PercussionEvent::s_percussionEventCategory), categoryMap.end());

        auto span = seqwires::iterateOver<seqwires::PercussionEvent>(track);

        auto it = span.begin();
        ASSERT_NE(it, span.end());
        EXPECT_EQ(it->as<seqwires::PercussionOnEvent>()->getInstrument(), "HTom");
        ++it;
        ASSERT_NE(it, span.end());
        EXPECT_EQ(it->as<seqwires::PercussionOffEvent>()->getInstrument(), "HTom");
        ++it;
        ASSERT_NE(it, span.end());
        EXPECT_EQ(it->as<seqwires::PercussionOnEvent>()->getInstrument(), "LMTom");
        ++it;
        ASSERT_NE(it, span.end());
        EXPECT_EQ(it->as<seqwires::PercussionOffEvent>()->getInstrument(), "LMTom");
        ++it;
        ASSERT_NE(it, span.end());
        EXPECT_EQ(it->as<seqwires::PercussionOnEvent>()->getInstrument(), "LwTom");
        ++it;
        ASSERT_NE(it, span.end());
        EXPECT_EQ(it->as<seqwires::PercussionOffEvent>()->getInstrument(), "LwTom");
        ++it;
        EXPECT_EQ(it, span.end());
    }
}

INSTANTIATE_TEST_SUITE_P(PercussionTest,
                         SmfStandardPercussionTest,
                         testing::Values("GM", /*"GS", "XG",*/ "GM2"));

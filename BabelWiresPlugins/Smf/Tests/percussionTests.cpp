#include <gtest/gtest.h>

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

namespace {
    struct PercussionTestData {
        const char* m_specificationId;
        babelwires::Identifier m_instrumentId0;
        babelwires::Identifier m_instrumentId1;
        babelwires::Identifier m_instrumentId2;
    };
} // namespace

class SmfStandardPercussionTest : public testing::TestWithParam<PercussionTestData> {};

TEST_P(SmfStandardPercussionTest, saveLoad) {
    testUtils::TestEnvironment testEnvironment;
    seqwires::registerLib(testEnvironment.m_projectContext);
    smf::registerLib(testEnvironment.m_projectContext);

    const PercussionTestData& testData = GetParam();

    testUtils::TempFilePath tempFile("standardPercussion.mid", testData.m_specificationId);

    {
        smf::target::SmfFeature smfFeature(testEnvironment.m_projectContext);
        smfFeature.setToDefault();

        auto* smfFormatFeature =
            smfFeature.getChildFromStep(babelwires::PathStep("Format")).as<smf::target::SmfFormatFeature>();
        ASSERT_NE(smfFormatFeature, nullptr);

        smfFormatFeature->getMidiMetadata().getSpecFeature()->set(babelwires::Identifier(testData.m_specificationId));

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

        track->addEvent(seqwires::PercussionOnEvent{0, testData.m_instrumentId0});
        track->addEvent(seqwires::PercussionOffEvent{babelwires::Rational(1, 4), testData.m_instrumentId0});
        track->addEvent(seqwires::PercussionOnEvent{0, testData.m_instrumentId1});
        track->addEvent(seqwires::PercussionOffEvent{babelwires::Rational(1, 4), testData.m_instrumentId1});
        track->addEvent(seqwires::PercussionOnEvent{0, testData.m_instrumentId2});
        track->addEvent(seqwires::PercussionOffEvent{babelwires::Rational(1, 4), testData.m_instrumentId2});

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

        EXPECT_EQ(smfFeature->getMidiMetadata().getSpecFeature()->get(),
                  babelwires::Identifier(testData.m_specificationId));

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
        EXPECT_EQ(it->as<seqwires::PercussionOnEvent>()->getInstrument(), testData.m_instrumentId0);
        ++it;
        ASSERT_NE(it, span.end());
        EXPECT_EQ(it->as<seqwires::PercussionOffEvent>()->getInstrument(), testData.m_instrumentId0);
        ++it;
        ASSERT_NE(it, span.end());
        EXPECT_EQ(it->as<seqwires::PercussionOnEvent>()->getInstrument(), testData.m_instrumentId1);
        ++it;
        ASSERT_NE(it, span.end());
        EXPECT_EQ(it->as<seqwires::PercussionOffEvent>()->getInstrument(), testData.m_instrumentId1);
        ++it;
        ASSERT_NE(it, span.end());
        EXPECT_EQ(it->as<seqwires::PercussionOnEvent>()->getInstrument(), testData.m_instrumentId2);
        ++it;
        ASSERT_NE(it, span.end());
        EXPECT_EQ(it->as<seqwires::PercussionOffEvent>()->getInstrument(), testData.m_instrumentId2);
        ++it;
        EXPECT_EQ(it, span.end());
    }
}

// The set should be automatically selected based on the standard and instruments.
INSTANTIATE_TEST_SUITE_P(PercussionTest, SmfStandardPercussionTest,
                         testing::Values(PercussionTestData{"GM", "AcBass", "HMTom", "OTrian"},   // GM Percussion
                                         PercussionTestData{"GM2", "HighQ", "HMTom", "OSurdo"},   // Standard percussion
                                         PercussionTestData{"GM2", "AcBass", "RLwTm2", "RHiTm1"}, // Room set
                                         PercussionTestData{"GM2", "PKick", "PLwTm2", "PHiTm1"},  // Power set
                                         PercussionTestData{"GM2", "ElBass", "ElSnr2", "RevCym"}, // Electronic set
                                         PercussionTestData{"GM2", "ARmSht", "Tamb", "AClavs"},   // Analog set
                                         PercussionTestData{"GM2", "JKick2", "HMTom", "OTrian"},  // Jazz set
                                         PercussionTestData{"GM2", "BrTap", "BrSlap", "BrSwrL"},  // Brush set
                                         PercussionTestData{"GM2", "TimpF", "Timpc", "Aplaus"},   // Orchestra set
                                         PercussionTestData{"GM2", "GFret", "Bubble", "Dog"},     // SFX set
                                         PercussionTestData{"GS", "SnrRll", "FngSnp", "AcBass"},  // GS Standard 1 set
                                         PercussionTestData{"GS", "SnrRll", "RLwTm2", "RHiTm1"},  // GS Room set
                                         PercussionTestData{"GS", "SnrRll", "PLwTm2", "PHiTm1"},  // GS Power set
                                         PercussionTestData{"GS", "SnrRll", "ELwTm2", "RevCym"},  // GS Electronic set
                                         PercussionTestData{"GS", "Bs909", "ELwTm2", "Bs808"},    // GS 808/909 set
                                         PercussionTestData{"GS", "SnrRll", "JKick1", "JKick2"},  // GS Jazz set
                                         PercussionTestData{"XG", "SnrRll", "RimSht", "BeepLo"},  // XG Standard 1 set
                                         PercussionTestData{"XG", "SnrRll", "RLwTm2", "BeepLo"},  // XG Room set
                                         PercussionTestData{"XG", "PKick", "PLwTm1", "BeepLo"},   // XG Rock set
                                         PercussionTestData{"XG", "PKick", "ELwTm1", "RevCym"},   // XG Electro set
                                         PercussionTestData{"XG", "AnBass", "AMdTm2", "RevCym"},  // XG Analog set
                                         PercussionTestData{"XG", "JKick1", "ClHHat", "OpHHat"},  // XG Jazz set
                                         PercussionTestData{"XG", "JKick2", "BrTap", "BrSlap"},   // XG Brush set
                                         PercussionTestData{"XG", "CnBD1", "CnCym2", "CnBD2"},    // XG Classic set
                                         PercussionTestData{"XG", "PckScr", "Thnder", "XgMaou"},  // XG SFX1 set
                                         PercussionTestData{"XG", "DoorCr", "Aplaus", "XgCstr"}   // XG SFX2 set
                                         ));

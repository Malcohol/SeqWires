#include <gtest/gtest.h>

#include <Plugins/Smf/Plugin/gmSpec.hpp>
#include <Plugins/Smf/Plugin/libRegistration.hpp>
#include <Plugins/Smf/Plugin/smfParser.hpp>
#include <Plugins/Smf/Plugin/smfSourceModel.hpp>
#include <Plugins/Smf/Plugin/smfTargetModel.hpp>
#include <Plugins/Smf/Plugin/smfWriter.hpp>

#include <SeqWiresLib/Types/Track/TrackEvents/noteEvents.hpp>
#include <SeqWiresLib/Types/Track/TrackEvents/percussionEvents.hpp>
#include <SeqWiresLib/Types/Track/trackFeature.hpp>
#include <SeqWiresLib/Utilities/filteredTrackIterator.hpp>
#include <SeqWiresLib/libRegistration.hpp>

#include <Common/IO/fileDataSource.hpp>

#include <Tests/TestUtils/seqTestUtils.hpp>

#include <Tests/BabelWiresLib/TestUtils/testEnvironment.hpp>

#include <Tests/TestUtils/equalSets.hpp>
#include <Tests/TestUtils/tempFilePath.hpp>

namespace {
    struct PercussionTestData {
        smf::GMSpecType::Value m_specificationId;
        babelwires::ShortId m_instrumentId0;
        babelwires::ShortId m_instrumentId1;
        babelwires::ShortId m_instrumentId2;
    };
} // namespace

class SmfStandardPercussionTest : public testing::TestWithParam<PercussionTestData> {};

TEST_P(SmfStandardPercussionTest, saveLoad) {
    testUtils::TestEnvironment testEnvironment;
    seqwires::registerLib(testEnvironment.m_projectContext);
    smf::registerLib(testEnvironment.m_projectContext);

    const PercussionTestData& testData = GetParam();

    testUtils::TempFilePath tempFile("standardPercussion.mid",
                                     smf::GMSpecType::getIdentifierFromValue(testData.m_specificationId).toString());

    {
        smf::target::SmfFeature smfFeature(testEnvironment.m_projectContext);
        smfFeature.setToDefault();

        auto* smfFormatFeature =
            smfFeature.getChildFromStep(babelwires::PathStep("Format")).as<smf::target::SmfFormatFeature>();
        ASSERT_NE(smfFormatFeature, nullptr);

        smf::MidiMetadata::setSpecValue(smfFormatFeature->getMidiMetadata(), testData.m_specificationId);

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

        seqwires::Track track;

        track.addEvent(seqwires::PercussionOnEvent{0, testData.m_instrumentId0});
        track.addEvent(seqwires::PercussionOffEvent{babelwires::Rational(1, 4), testData.m_instrumentId0});
        track.addEvent(seqwires::PercussionOnEvent{0, testData.m_instrumentId1});
        track.addEvent(seqwires::PercussionOffEvent{babelwires::Rational(1, 4), testData.m_instrumentId1});
        track.addEvent(seqwires::PercussionOnEvent{0, testData.m_instrumentId2});
        track.addEvent(seqwires::PercussionOffEvent{babelwires::Rational(1, 4), testData.m_instrumentId2});

        trackFeature->set(std::move(track));

        std::ofstream os = tempFile.openForWriting(std::ios_base::binary);
        smf::writeToSmf(testEnvironment.m_projectContext, testEnvironment.m_log, smfFeature.getFormatFeature(), os);
    }

    {
        babelwires::FileDataSource midiFile(tempFile);

        const auto feature = smf::parseSmfSequence(midiFile, testEnvironment.m_projectContext, testEnvironment.m_log);
        ASSERT_NE(feature, nullptr);
        auto smfFeature = feature.get()->as<const smf::source::Format0SmfFeature>();
        ASSERT_NE(smfFeature, nullptr);

        EXPECT_EQ(smf::MidiMetadata::getSpecValue(smfFeature->getMidiMetadata()), testData.m_specificationId);

        EXPECT_EQ(smfFeature->getNumMidiTracks(), 1);
        const auto& channelGroup = dynamic_cast<const smf::source::RecordChannelGroup&>(smfFeature->getMidiTrack(0));
        EXPECT_EQ(channelGroup.getNumFeatures(), 1);
        const auto* trackFeature = channelGroup.getFeature(0)->as<const seqwires::TrackFeature>();
        ASSERT_NE(trackFeature, nullptr);
        ASSERT_EQ(channelGroup.getStepToChild(trackFeature), babelwires::PathStep(babelwires::ShortId("ch9")));

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
INSTANTIATE_TEST_SUITE_P(
    PercussionTest, SmfStandardPercussionTest,
    testing::Values(PercussionTestData{smf::GMSpecType::Value::GM, "AcBass", "HMTom", "OTrian"}, // GM Percussion
                    PercussionTestData{smf::GMSpecType::Value::GM2, "HighQ", "HMTom", "OSurdo"}, // Standard percussion
                    PercussionTestData{smf::GMSpecType::Value::GM2, "AcBass", "RLwTm2", "RHiTm1"}, // Room set
                    PercussionTestData{smf::GMSpecType::Value::GM2, "PKick", "PLwTm2", "PHiTm1"},  // Power set
                    PercussionTestData{smf::GMSpecType::Value::GM2, "ElBass", "ElSnr2", "RevCym"}, // Electronic set
                    PercussionTestData{smf::GMSpecType::Value::GM2, "ARmSht", "Tamb", "AClavs"},   // Analog set
                    PercussionTestData{smf::GMSpecType::Value::GM2, "JKick2", "HMTom", "OTrian"},  // Jazz set
                    PercussionTestData{smf::GMSpecType::Value::GM2, "BrTap", "BrSlap", "BrSwrL"},  // Brush set
                    PercussionTestData{smf::GMSpecType::Value::GM2, "TimpF", "Timpc", "Aplaus"},   // Orchestra set
                    PercussionTestData{smf::GMSpecType::Value::GM2, "GFret", "Bubble", "Dog"},     // SFX set
                    PercussionTestData{smf::GMSpecType::Value::GS, "SnrRll", "FngSnp", "AcBass"},  // GS Standard 1 set
                    PercussionTestData{smf::GMSpecType::Value::GS, "SnrRll", "RLwTm2", "RHiTm1"},  // GS Room set
                    PercussionTestData{smf::GMSpecType::Value::GS, "SnrRll", "PLwTm2", "PHiTm1"},  // GS Power set
                    PercussionTestData{smf::GMSpecType::Value::GS, "SnrRll", "ELwTm2", "RevCym"},  // GS Electronic set
                    PercussionTestData{smf::GMSpecType::Value::GS, "Bs909", "ELwTm2", "Bs808"},    // GS 808/909 set
                    PercussionTestData{smf::GMSpecType::Value::GS, "SnrRll", "JKick1", "JKick2"},  // GS Jazz set
                    PercussionTestData{smf::GMSpecType::Value::GS, "SnrRll", "JKick2", "BrTap"},   // GS Jazz set
                    PercussionTestData{smf::GMSpecType::Value::GS, "TimpF", "Timpc", "Aplaus"},    // GS Orchestra set
                    PercussionTestData{smf::GMSpecType::Value::GS, "Laugh", "Jetpln", "Aplaus"},   // GS SFX set
                    PercussionTestData{smf::GMSpecType::Value::XG, "SnrRll", "RimSht", "BeepLo"},  // XG Standard 1 set
                    PercussionTestData{smf::GMSpecType::Value::XG, "SnrRll", "RLwTm2", "BeepLo"},  // XG Room set
                    PercussionTestData{smf::GMSpecType::Value::XG, "PKick", "PLwTm1", "BeepLo"},   // XG Rock set
                    PercussionTestData{smf::GMSpecType::Value::XG, "PKick", "ELwTm1", "RevCym"},   // XG Electro set
                    PercussionTestData{smf::GMSpecType::Value::XG, "AnBass", "AMdTm2", "RevCym"},  // XG Analog set
                    PercussionTestData{smf::GMSpecType::Value::XG, "JKick1", "ClHHat", "OpHHat"},  // XG Jazz set
                    PercussionTestData{smf::GMSpecType::Value::XG, "JKick2", "BrTap", "BrSlap"},   // XG Brush set
                    PercussionTestData{smf::GMSpecType::Value::XG, "CnBD1", "CnCym2", "CnBD2"},    // XG Classic set
                    PercussionTestData{smf::GMSpecType::Value::XG, "PckScr", "Thnder", "XgMaou"},  // XG SFX1 set
                    PercussionTestData{smf::GMSpecType::Value::XG, "DoorCr", "Aplaus", "XgCstr"}   // XG SFX2 set
                    ));

namespace {
    struct TrackAllocationTestData {
        smf::GMSpecType::Value m_specificationId;
        std::vector<babelwires::ShortId> m_instrumentsInChannel[3];
        bool m_hasNotes[3];
        // The events expected to appear in channels 8, 9 and 10.
        std::vector<babelwires::ShortId> m_expectedInChannel[3];
    };
} // namespace

class SmfTrackAllocationPercussionTest : public testing::TestWithParam<TrackAllocationTestData> {};

TEST_P(SmfTrackAllocationPercussionTest, trackAllocation) {
    testUtils::TestEnvironment testEnvironment;
    seqwires::registerLib(testEnvironment.m_projectContext);
    smf::registerLib(testEnvironment.m_projectContext);

    const TrackAllocationTestData& testData = GetParam();

    testUtils::TempFilePath tempFile("smfPercussionTrackAllocation.mid",
                                     smf::GMSpecType::getIdentifierFromValue(testData.m_specificationId).toString());

    {
        smf::target::SmfFeature smfFeature(testEnvironment.m_projectContext);
        smfFeature.setToDefault();

        auto* smfFormatFeature =
            smfFeature.getChildFromStep(babelwires::PathStep("Format")).as<smf::target::SmfFormatFeature>();
        ASSERT_NE(smfFormatFeature, nullptr);

        smf::MidiMetadata::setSpecValue(smfFormatFeature->getMidiMetadata(), testData.m_specificationId);

        auto* tracks =
            smfFormatFeature->getChildFromStep(babelwires::PathStep("tracks")).as<babelwires::ArrayFeature>();

        tracks->setSize(3);

        for (int i = 0; i < 3; ++i) {
            auto* channelTrack = tracks->getFeature(i)->as<babelwires::RecordFeature>();
            ASSERT_NE(channelTrack, nullptr);
            EXPECT_EQ(channelTrack->getNumFeatures(), 2);

            auto* channelFeature =
                channelTrack->getChildFromStep(babelwires::PathStep("Chan")).as<babelwires::IntFeature>();
            channelFeature->set(8 + i);

            auto* trackFeature =
                channelTrack->getChildFromStep(babelwires::PathStep("Track")).as<seqwires::TrackFeature>();

            seqwires::Track track;

            track.addEvent(seqwires::NoteOnEvent{0, 65});
            track.addEvent(seqwires::NoteOffEvent{babelwires::Rational(1, 4), 65});

            for (auto instrument : testData.m_instrumentsInChannel[i]) {
                track.addEvent(seqwires::PercussionOnEvent{0, instrument});
                track.addEvent(seqwires::PercussionOffEvent{babelwires::Rational(1, 4), instrument});
            }

            trackFeature->set(std::move(track));
        }

        std::ofstream os = tempFile.openForWriting(std::ios_base::binary);
        smf::writeToSmf(testEnvironment.m_projectContext, testEnvironment.m_log, smfFeature.getFormatFeature(), os);
    }

    {
        babelwires::FileDataSource midiFile(tempFile);

        const auto feature = smf::parseSmfSequence(midiFile, testEnvironment.m_projectContext, testEnvironment.m_log);
        ASSERT_NE(feature, nullptr);
        auto smfFeature = feature.get()->as<const smf::source::Format0SmfFeature>();
        ASSERT_NE(smfFeature, nullptr);

        EXPECT_EQ(smf::MidiMetadata::getSpecValue(smfFeature->getMidiMetadata()), testData.m_specificationId);

        EXPECT_EQ(smfFeature->getNumMidiTracks(), 1);
        const auto& channelGroup = dynamic_cast<const smf::source::RecordChannelGroup&>(smfFeature->getMidiTrack(0));
        EXPECT_EQ(channelGroup.getNumFeatures(), 3);

        for (int i = 0; i < 3; ++i) {
            const auto* const trackFeature = channelGroup.getFeature(i)->as<const seqwires::TrackFeature>();
            ASSERT_NE(trackFeature, nullptr);
            // ASSERT_EQ(channelGroup.getStepToChild(trackFeature),
            // babelwires::PathStep(babelwires::ShortId("ch9")));

            const auto& track = trackFeature->get();
            auto categoryMap = track.getNumEventGroupsByCategory();

            if (testData.m_hasNotes[i]) {
                ASSERT_NE(categoryMap.find(seqwires::NoteEvent::s_noteEventCategory), categoryMap.end());
                EXPECT_EQ(categoryMap.find(seqwires::NoteEvent::s_noteEventCategory)->second, 1);
                EXPECT_EQ(categoryMap.find(seqwires::PercussionEvent::s_percussionEventCategory), categoryMap.end());
            } else {
                EXPECT_EQ(categoryMap.find(seqwires::NoteEvent::s_noteEventCategory), categoryMap.end());
                ASSERT_NE(categoryMap.find(seqwires::PercussionEvent::s_percussionEventCategory), categoryMap.end());

                std::vector<babelwires::ShortId> instrumentsInTrack;
                for (auto event : seqwires::iterateOver<seqwires::PercussionOnEvent>(track)) {
                    instrumentsInTrack.emplace_back(event.getInstrument());
                }

                EXPECT_TRUE(testUtils::areEqualSets(instrumentsInTrack, testData.m_expectedInChannel[i]));
            }
        }
    }
}

// Test how tracks get assigned in the various standards (the GS implementation in SeqWires is not as flexible as the
// full standard)
INSTANTIATE_TEST_SUITE_P(
    PercussionTest, SmfTrackAllocationPercussionTest,
    testing::Values(TrackAllocationTestData{smf::GMSpecType::Value::GM,
                                            {{"AcBass", "HMTom", "OTrian"},
                                             {"AcBass", "HMTom", "OTrian"},
                                             {"AcBass", "HMTom", "OTrian"}},
                                            {true, false, true},
                                            {{}, {"AcBass", "HMTom", "OTrian"}, {}}},
                    TrackAllocationTestData{smf::GMSpecType::Value::GM2,
                        {{"AcBass", "HMTom", "OTrian"}, {"AcBass", "HMTom", "OTrian"}, {"AcBass", "HMTom", "OTrian"}},
                        {true, false, false},
                        {{}, {"AcBass", "HMTom", "OTrian"}, {"AcBass", "HMTom", "OTrian"}}},
                    TrackAllocationTestData{smf::GMSpecType::Value::GM2,
                                            {{"AcBass", "HMTom", "OTrian"}, {"AcBass", "HMTom", "OTrian"}, {}},
                                            {true, false, true},
                                            {{}, {"AcBass", "HMTom", "OTrian"}, {}}},
                    TrackAllocationTestData{smf::GMSpecType::Value::GS,
                        {{"AcBass", "HMTom", "OTrian"}, {"AcBass", "HMTom", "OTrian"}, {"AcBass", "HMTom", "OTrian"}},
                        {true, false, false},
                        {{}, {"AcBass", "HMTom", "OTrian"}, {"AcBass", "HMTom", "OTrian"}}},
                    TrackAllocationTestData{smf::GMSpecType::Value::GS,
                                            {{"AcBass", "HMTom", "OTrian"}, {"AcBass", "HMTom", "OTrian"}, {}},
                                            {true, false, true},
                                            {{}, {"AcBass", "HMTom", "OTrian"}, {}}},
                    TrackAllocationTestData{smf::GMSpecType::Value::XG,
                        {{"AcBass", "HMTom", "OTrian"}, {"AcBass", "HMTom", "OTrian"}, {"AcBass", "HMTom", "OTrian"}},
                        {false, false, false},
                        {{"AcBass", "HMTom", "OTrian"}, {"AcBass", "HMTom", "OTrian"}, {"AcBass", "HMTom", "OTrian"}}},
                    TrackAllocationTestData{smf::GMSpecType::Value::XG,
                                            {{"AcBass", "HMTom", "OTrian"}, {"AcBass", "HMTom", "OTrian"}, {}},
                                            {false, false, true},
                                            {{"AcBass", "HMTom", "OTrian"}, {"AcBass", "HMTom", "OTrian"}, {}}}));
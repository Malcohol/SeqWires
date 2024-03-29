#include <gtest/gtest.h>

#include <Plugins/Smf/Plugin/libRegistration.hpp>
#include <Plugins/Smf/Plugin/smfParser.hpp>
#include <Plugins/Smf/Plugin/smfSourceModel.hpp>
#include <Plugins/Smf/Plugin/smfTargetModel.hpp>
#include <Plugins/Smf/Plugin/smfWriter.hpp>

#include <SeqWiresLib/Types/Track/trackFeature.hpp>
#include <SeqWiresLib/Types/Track/TrackEvents/noteEvents.hpp>
#include <SeqWiresLib/libRegistration.hpp>

#include <BabelWiresLib/libRegistration.hpp>

#include <Common/IO/fileDataSource.hpp>

#include <Tests/TestUtils/seqTestUtils.hpp>

#include <Tests/BabelWiresLib/TestUtils/testEnvironment.hpp>

#include <Tests/TestUtils/tempFilePath.hpp>

TEST(SmfSaveLoadTest, cMajorScale) {
    testUtils::TestEnvironment testEnvironment;
    seqwires::registerLib(testEnvironment.m_projectContext);
    smf::registerLib(testEnvironment.m_projectContext);

    testUtils::TempFilePath tempFile("cMajor.mid");

    const std::vector<seqwires::Pitch> pitches{60, 62, 64, 65, 67, 69, 71, 72};
    {
        smf::target::SmfFeature smfFeature(testEnvironment.m_projectContext);
        smfFeature.setToDefault();

        auto* tracks = babelwires::FeaturePath::deserializeFromString("Format/tracks")
                           .follow(smfFeature)
                           .as<babelwires::ArrayFeature>();
        ASSERT_NE(tracks, nullptr);
        EXPECT_EQ(tracks->getNumFeatures(), 1);

        auto* channelTrack = tracks->getFeature(0)->as<babelwires::RecordFeature>();
        ASSERT_NE(channelTrack, nullptr);
        EXPECT_EQ(channelTrack->getNumFeatures(), 2);

        auto* channelFeature =
            channelTrack->getChildFromStep(babelwires::PathStep("Chan")).as<babelwires::IntFeature>();
        channelFeature->set(2);

        auto* trackFeature = channelTrack->getChildFromStep(babelwires::PathStep("Track")).as<seqwires::TrackFeature>();

        seqwires::Track track;

        testUtils::addSimpleNotes(pitches, track);

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

        EXPECT_EQ(smfFeature->getNumMidiTracks(), 1);
        const auto& channelGroup = dynamic_cast<const smf::source::RecordChannelGroup&>(smfFeature->getMidiTrack(0));
        EXPECT_EQ(channelGroup.getNumFeatures(), 1);
        const auto* trackFeature = channelGroup.getFeature(0)->as<const seqwires::TrackFeature>();
        ASSERT_NE(trackFeature, nullptr);
        ASSERT_EQ(channelGroup.getStepToChild(trackFeature), babelwires::PathStep(babelwires::ShortId("ch2")));

        testUtils::testSimpleNotes(pitches, trackFeature->get());
    }
}

namespace {

    enum MetadataFlags { HAS_SEQUENCE_NAME = 0b001, HAS_COPYRIGHT = 0b010, HAS_TEMPO = 0b100 };

    void addMetadata(smf::target::SmfFormatFeature& smfFeature, std::uint8_t flags) {
        auto* metadata = smfFeature.getChildFromStep(babelwires::PathStep("Meta")).as<smf::MidiMetadata>();
        if (flags & HAS_SEQUENCE_NAME) {
            metadata->getActivatedSequenceName().set("Test Sequence Name");
        }
        if (flags & HAS_COPYRIGHT) {
            metadata->getActivatedCopyright().set("(C)2021 Test Copyright");
        }
        if (flags & HAS_TEMPO) {
            metadata->getActivatedTempoFeature().set(100);
        }
    }

    void checkMetadata(const smf::source::SmfFeature& smfFeature, std::uint8_t flags) {
        const auto* metadata = smfFeature.getChildFromStep(babelwires::PathStep("Meta")).as<smf::MidiMetadata>();
        if (flags & HAS_SEQUENCE_NAME) {
            ASSERT_NE(metadata->getSequenceName(), nullptr);
            EXPECT_EQ(metadata->getSequenceName()->get(), "Test Sequence Name");
        }
        if (flags & HAS_COPYRIGHT) {
            ASSERT_NE(metadata->getCopyright(), nullptr);
            EXPECT_EQ(metadata->getCopyright()->get(), "(C)2021 Test Copyright");
        }
        if (flags & HAS_TEMPO) {
            ASSERT_NE(metadata->getTempoFeature(), nullptr);
            EXPECT_EQ(metadata->getTempoFeature()->get(), 100);
        }
    }
} // namespace

TEST(SmfSaveLoadTest, cMajorScaleWithMetadata) {
    testUtils::TestEnvironment testEnvironment;
    seqwires::registerLib(testEnvironment.m_projectContext);
    smf::registerLib(testEnvironment.m_projectContext);

    const std::vector<seqwires::Pitch> pitches{60, 62, 64, 65, 67, 69, 71, 72};

    for (std::uint8_t metadata = 0; metadata < 8; ++metadata) {
        testUtils::TempFilePath tempFile("cMajorWithMetadata.mid", metadata);
        {
            smf::target::SmfFeature smfFeature(testEnvironment.m_projectContext);
            smfFeature.setToDefault();

            auto* smfFormatFeature =
                smfFeature.getChildFromStep(babelwires::PathStep("Format")).as<smf::target::SmfFormatFeature>();
            ASSERT_NE(smfFormatFeature, nullptr);

            addMetadata(*smfFormatFeature, metadata);

            auto* tracks =
                smfFormatFeature->getChildFromStep(babelwires::PathStep("tracks")).as<babelwires::ArrayFeature>();
            ASSERT_NE(tracks, nullptr);
            EXPECT_EQ(tracks->getNumFeatures(), 1);

            auto* channelTrack = tracks->getFeature(0)->as<babelwires::RecordFeature>();
            ASSERT_NE(channelTrack, nullptr);
            EXPECT_EQ(channelTrack->getNumFeatures(), 2);

            auto* channelFeature =
                channelTrack->getChildFromStep(babelwires::PathStep("Chan")).as<babelwires::IntFeature>();
            channelFeature->set(2);

            auto* trackFeature =
                channelTrack->getChildFromStep(babelwires::PathStep("Track")).as<seqwires::TrackFeature>();

            seqwires::Track track;

            testUtils::addSimpleNotes(pitches, track);

            trackFeature->set(std::move(track));

            std::ofstream os = tempFile.openForWriting(std::ios_base::binary);
            smf::writeToSmf(testEnvironment.m_projectContext, testEnvironment.m_log, smfFeature.getFormatFeature(), os);
        }

        babelwires::FileDataSource midiFile(tempFile);

        const auto feature = smf::parseSmfSequence(midiFile, testEnvironment.m_projectContext, testEnvironment.m_log);
        ASSERT_NE(feature, nullptr);
        auto smfFeature = feature.get()->as<const smf::source::Format0SmfFeature>();
        ASSERT_NE(smfFeature, nullptr);

        checkMetadata(*smfFeature, metadata);

        EXPECT_EQ(smfFeature->getNumMidiTracks(), 1);
        const auto& channelGroup = dynamic_cast<const smf::source::RecordChannelGroup&>(smfFeature->getMidiTrack(0));
        EXPECT_EQ(channelGroup.getNumFeatures(), 1);
        const auto* trackFeature = channelGroup.getFeature(0)->as<const seqwires::TrackFeature>();
        ASSERT_NE(trackFeature, nullptr);
        ASSERT_EQ(channelGroup.getStepToChild(trackFeature), babelwires::PathStep(babelwires::ShortId("ch2")));

        testUtils::testSimpleNotes(pitches, trackFeature->get());
    }
}

namespace {
    const std::vector<seqwires::Pitch> chordPitches[3] = {
        {60, 62, 64, 65, 67, 69, 71, 72}, {64, 65, 67, 69, 71, 72, 74, 76}, {67, 69, 71, 72, 74, 76, 77, 79}};
}

TEST(SmfSaveLoadTest, format0Chords) {
    testUtils::TestEnvironment testEnvironment;
    seqwires::registerLib(testEnvironment.m_projectContext);
    smf::registerLib(testEnvironment.m_projectContext);
    testUtils::TempFilePath tempFile("format0Chords.mid");

    const char* trackName[3] = {"ch0", "ch1", "ch2"};

    {
        smf::target::SmfFeature smfFeature(testEnvironment.m_projectContext);
        smfFeature.setToDefault();

        auto* tracks = babelwires::FeaturePath::deserializeFromString("Format/tracks")
                           .follow(smfFeature)
                           .as<babelwires::ArrayFeature>();
        ASSERT_NE(tracks, nullptr);
        tracks->addEntry();
        tracks->addEntry();
        EXPECT_EQ(tracks->getNumFeatures(), 3);

        for (int i = 0; i < 3; ++i) {
            auto* channelTrack = tracks->getFeature(i)->as<babelwires::RecordFeature>();
            ASSERT_NE(channelTrack, nullptr);
            EXPECT_EQ(channelTrack->getNumFeatures(), 2);
            auto* channelFeature =
                channelTrack->getChildFromStep(babelwires::PathStep("Chan")).as<babelwires::IntFeature>();
            channelFeature->set(i);
            auto* trackFeature =
                channelTrack->getChildFromStep(babelwires::PathStep("Track")).as<seqwires::TrackFeature>();
            seqwires::Track track;
            testUtils::addSimpleNotes(chordPitches[i], track);
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

        EXPECT_EQ(smfFeature->getNumMidiTracks(), 1);
        const auto& channelGroup = dynamic_cast<const smf::source::RecordChannelGroup&>(smfFeature->getMidiTrack(0));
        EXPECT_EQ(channelGroup.getNumFeatures(), 3);

        for (int i = 0; i < 3; ++i) {
            const auto* trackFeature = channelGroup.getFeature(i)->as<const seqwires::TrackFeature>();
            ASSERT_NE(trackFeature, nullptr);
            ASSERT_EQ(channelGroup.getStepToChild(trackFeature),
                      babelwires::PathStep(babelwires::ShortId(trackName[i])));
            testUtils::testSimpleNotes(chordPitches[i], trackFeature->get());
        }
    }
}

TEST(SmfSaveLoadTest, format1Chords) {
    testUtils::TestEnvironment testEnvironment;
    seqwires::registerLib(testEnvironment.m_projectContext);
    smf::registerLib(testEnvironment.m_projectContext);
    testUtils::TempFilePath tempFile("format1Chords.mid");

    const char* trackName[3] = {"ch0", "ch1", "ch2"};

    {
        smf::target::SmfFeature smfFeature(testEnvironment.m_projectContext);
        smfFeature.setToDefault();

        auto* smfFormatFeature =
            smfFeature.getChildFromStep(babelwires::PathStep("Format")).as<smf::target::SmfFormatFeature>();
        ASSERT_NE(smfFormatFeature, nullptr);

        smfFormatFeature->selectTag("SMF1");

        auto* tracks =
            smfFormatFeature->getChildFromStep(babelwires::PathStep("tracks")).as<babelwires::ArrayFeature>();

        ASSERT_NE(tracks, nullptr);
        tracks->addEntry();
        tracks->addEntry();
        EXPECT_EQ(tracks->getNumFeatures(), 3);

        for (int i = 0; i < 3; ++i) {
            auto* channelTrack = tracks->getFeature(i)->as<babelwires::RecordFeature>();
            ASSERT_NE(channelTrack, nullptr);
            EXPECT_EQ(channelTrack->getNumFeatures(), 2);
            auto* channelFeature =
                channelTrack->getChildFromStep(babelwires::PathStep("Chan")).as<babelwires::IntFeature>();
            channelFeature->set(i);
            auto* trackFeature =
                channelTrack->getChildFromStep(babelwires::PathStep("Track")).as<seqwires::TrackFeature>();
            seqwires::Track track;
            testUtils::addSimpleNotes(chordPitches[i], track);
            trackFeature->set(std::move(track));
        }

        std::ofstream os = tempFile.openForWriting(std::ios_base::binary);
        smf::writeToSmf(testEnvironment.m_projectContext, testEnvironment.m_log, smfFeature.getFormatFeature(), os);
    }

    {
        babelwires::FileDataSource midiFile(tempFile);

        const auto feature = smf::parseSmfSequence(midiFile, testEnvironment.m_projectContext, testEnvironment.m_log);
        ASSERT_NE(feature, nullptr);
        auto smfFeature = feature.get()->as<const smf::source::Format1SmfFeature>();
        ASSERT_NE(smfFeature, nullptr);

        EXPECT_EQ(smfFeature->getNumMidiTracks(), 3);
        for (int i = 0; i < 3; ++i) {
            const auto& channelGroup =
                dynamic_cast<const smf::source::ExtensibleChannelGroup&>(smfFeature->getMidiTrack(i));
            EXPECT_EQ(channelGroup.getNumFeatures(), 2);
            const auto* channelFeature =
                channelGroup.getChildFromStep(babelwires::PathStep("ChanNo")).as<const babelwires::IntFeature>();
            ASSERT_NE(channelFeature, nullptr);
            EXPECT_EQ(channelFeature->get(), i);
            const auto* trackFeature =
                channelGroup.getChildFromStep(babelwires::PathStep("Track")).as<const seqwires::TrackFeature>();
            ASSERT_NE(trackFeature, nullptr);
            testUtils::testSimpleNotes(chordPitches[i], trackFeature->get());
        }
    }
}

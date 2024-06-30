#include <gtest/gtest.h>

#include <Plugins/Smf/Plugin/libRegistration.hpp>
#include <Plugins/Smf/Plugin/midiTrackAndChannel.hpp>
#include <Plugins/Smf/Plugin/midiTrackAndChannelArray.hpp>
#include <Plugins/Smf/Plugin/smfParser.hpp>
#include <Plugins/Smf/Plugin/smfSourceModel.hpp>
#include <Plugins/Smf/Plugin/smfTargetModel.hpp>
#include <Plugins/Smf/Plugin/smfWriter.hpp>

#include <SeqWiresLib/Types/Track/TrackEvents/noteEvents.hpp>
#include <SeqWiresLib/Types/Track/trackFeature.hpp>
#include <SeqWiresLib/libRegistration.hpp>

#include <BabelWiresLib/libRegistration.hpp>
#include <BabelWiresLib/InstanceOf/instanceOfArrayType.hpp>

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

        auto smfType = smfFeature.getSmfTypeFeature();
        auto tracks = smfType.getTracks();
        auto trackAndChan = tracks.getEntry(0);
        trackAndChan.getChan().set(2);

        seqwires::Track track;
        testUtils::addSimpleNotes(pitches, track);
        trackAndChan.getTrack().set(std::move(track));

        std::ofstream os = tempFile.openForWriting(std::ios_base::binary);
        smf::writeToSmf(testEnvironment.m_projectContext, testEnvironment.m_log, smfFeature, os);
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

    void addMetadata(smf::target::SmfFeature& smfFeature, std::uint8_t flags) {
        auto metadata = smfFeature.getSmfTypeFeature().getMeta();

        if (flags & HAS_SEQUENCE_NAME) {
            metadata.activateAndGetName().set("Test Sequence Name");
        }
        if (flags & HAS_COPYRIGHT) {
            metadata.activateAndGetCopyR().set("(C)2021 Test Copyright");
        }
        if (flags & HAS_TEMPO) {
            metadata.activateAndGetTempo().set(100);
        }
    }

    void checkMetadata(const smf::source::SmfFeature& smfFeature, std::uint8_t flags) {
        const auto& metadata = smf::MidiMetadata::Instance<const babelwires::ValueFeature>(&smfFeature.getMidiMetadata());
        
        if (flags & HAS_SEQUENCE_NAME) {
            ASSERT_TRUE(metadata.tryGetName());
            EXPECT_EQ(metadata.tryGetName()->get(), "Test Sequence Name");
        }
        if (flags & HAS_COPYRIGHT) {
            ASSERT_TRUE(metadata.tryGetCopyR());
            EXPECT_EQ(metadata.tryGetCopyR()->get(), "(C)2021 Test Copyright");
        }
        if (flags & HAS_TEMPO) {
            ASSERT_TRUE(metadata.tryGetTempo());
            EXPECT_EQ(metadata.tryGetTempo()->get(), 100);
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

            addMetadata(smfFeature, metadata);

            auto smfType = smfFeature.getSmfTypeFeature();
            auto tracks = smfType.getTracks();
            auto trackAndChan = tracks.getEntry(0);
            trackAndChan.getChan().set(2);

            seqwires::Track track;
            testUtils::addSimpleNotes(pitches, track);
            trackAndChan.getTrack().set(std::move(track));

            std::ofstream os = tempFile.openForWriting(std::ios_base::binary);
            smf::writeToSmf(testEnvironment.m_projectContext, testEnvironment.m_log, smfFeature, os);
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

        auto smfType = smfFeature.getSmfTypeFeature();
        auto tracks = smfType.getTracks();
        tracks.setSize(3);

        for (int i = 0; i < 3; ++i) {
            auto trackAndChan = tracks.getEntry(i);
            trackAndChan.getChan().set(i);
            seqwires::Track track;
            testUtils::addSimpleNotes(chordPitches[i], track);
            trackAndChan.getTrack().set(std::move(track));
        }

        std::ofstream os = tempFile.openForWriting(std::ios_base::binary);
        smf::writeToSmf(testEnvironment.m_projectContext, testEnvironment.m_log, smfFeature, os);
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

        auto smfType = smfFeature.getSmfTypeFeature();
        smfType.selectTag("SMF1");
        auto tracks = smfType.getTracks();
        tracks.setSize(3);

        for (int i = 0; i < 3; ++i) {
            auto trackAndChan = tracks.getEntry(i);
            trackAndChan.getChan().set(i);
            seqwires::Track track;
            testUtils::addSimpleNotes(chordPitches[i], track);
            trackAndChan.getTrack().set(std::move(track));
        }

        std::ofstream os = tempFile.openForWriting(std::ios_base::binary);
        smf::writeToSmf(testEnvironment.m_projectContext, testEnvironment.m_log, smfFeature, os);
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

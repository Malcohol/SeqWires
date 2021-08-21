#include <gtest/gtest.h>

#include <BabelWiresPlugins/Smf/Plugin/smfParser.hpp>
#include <BabelWiresPlugins/Smf/Plugin/smfSourceModel.hpp>
#include <BabelWiresPlugins/Smf/Plugin/smfWriter.hpp>
#include <BabelWiresPlugins/Smf/Plugin/smfTargetModel.hpp>

#include <SeqWiresLib/Features/trackFeature.hpp>
#include <SeqWiresLib/Tracks/noteEvents.hpp>

#include <BabelWires/Common/IO/fileDataSource.hpp>

#include <Tests/TestUtils/seqTestUtils.hpp>

#include <Tests/TestUtils/tempFilePath.hpp>

#include <Tests/TestUtils/testLog.hpp>

TEST(SmfSaveLoadTest, cMajorScale) {
    testUtils::TestLog log;
    testUtils::TempFilePath tempFile("cMajor.mid");

    const std::vector<seqwires::Pitch> pitches{60, 62, 64, 65, 67, 69, 71, 72};
    {
        smf::target::Format0SmfFeature smfFeature;
        smfFeature.setToDefault();

        auto* tracks = smfFeature.getChildFromStep(babelwires::PathStep("tracks")).asA<babelwires::ArrayFeature>();
        ASSERT_NE(tracks, nullptr);
        EXPECT_EQ(tracks->getNumFeatures(), 1);

        auto* channelTrack = tracks->getFeature(0)->asA<babelwires::RecordFeature>();
        ASSERT_NE(channelTrack, nullptr);
        EXPECT_EQ(channelTrack->getNumFeatures(), 2);

        auto* channelFeature = channelTrack->getChildFromStep(babelwires::PathStep("Chan")).asA<babelwires::IntFeature>();
        channelFeature->set(2);

        auto* trackFeature = channelTrack->getChildFromStep(babelwires::PathStep("Track")).asA<seqwires::TrackFeature>();

        auto track = std::make_unique<seqwires::Track>();

        testUtils::addSimpleNotes(pitches, *track);

        trackFeature->set(std::move(track));

        std::ofstream os(tempFile);
        smf::writeToSmfFormat0(os, smfFeature);
    }

    {
        babelwires::FileDataSource midiFile(tempFile);

        const auto feature = smf::parseSmfSequence(midiFile, log);
        ASSERT_NE(feature, nullptr);
        auto smfFeature = feature.get()->asA<const smf::source::Format0SmfFeature>();
        ASSERT_NE(smfFeature, nullptr);

        EXPECT_EQ(smfFeature->getNumMidiTracks(), 1);
        const auto& channelGroup = dynamic_cast<const smf::source::RecordChannelGroup&>(smfFeature->getMidiTrack(0));
        EXPECT_EQ(channelGroup.getNumFeatures(), 1);
        const auto* trackFeature = channelGroup.getFeature(0)->asA<const seqwires::TrackFeature>();
        ASSERT_NE(trackFeature, nullptr);
        ASSERT_EQ(channelGroup.getStepToChild(trackFeature), babelwires::PathStep(babelwires::FieldIdentifier("ch2")));

        testUtils::testSimpleNotes(pitches, trackFeature->get());
    }
}

namespace {

    enum MetadataFlags {
        HAS_SEQUENCE_NAME = 0b001,
        HAS_COPYRIGHT     = 0b010,
        HAS_TEMPO         = 0b100
    };

    void addMetadata(smf::target::SmfFeature& smfFeature, std::uint8_t flags) {
        auto* metadata = dynamic_cast<smf::MidiMetadata*>(&smfFeature.getChildFromStep(babelwires::PathStep("Meta")));
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
        const auto* metadata = dynamic_cast<const smf::MidiMetadata*>(&smfFeature.getChildFromStep(babelwires::PathStep("Meta")));
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
}

TEST(SmfSaveLoadTest, cMajorScaleWithMetadata) {
    testUtils::TestLog log;

    const std::vector<seqwires::Pitch> pitches{60, 62, 64, 65, 67, 69, 71, 72};

    for (std::uint8_t metadata = 0; metadata < 8; ++metadata) {
        testUtils::TempFilePath tempFile("cMajorWithMetadata.mid", metadata);
        {
            smf::target::Format0SmfFeature smfFeature;
            smfFeature.setToDefault();

            addMetadata(smfFeature, metadata);

            auto* tracks = smfFeature.getChildFromStep(babelwires::PathStep("tracks")).asA<babelwires::ArrayFeature>();
            ASSERT_NE(tracks, nullptr);
            EXPECT_EQ(tracks->getNumFeatures(), 1);

            auto* channelTrack = tracks->getFeature(0)->asA<babelwires::RecordFeature>();
            ASSERT_NE(channelTrack, nullptr);
            EXPECT_EQ(channelTrack->getNumFeatures(), 2);

            auto* channelFeature = channelTrack->getChildFromStep(babelwires::PathStep("Chan")).asA<babelwires::IntFeature>();
            channelFeature->set(2);

            auto* trackFeature = channelTrack->getChildFromStep(babelwires::PathStep("Track")).asA<seqwires::TrackFeature>();

            auto track = std::make_unique<seqwires::Track>();

            testUtils::addSimpleNotes(pitches, *track);

            trackFeature->set(std::move(track));

            std::ofstream os(tempFile);
            smf::writeToSmfFormat0(os, smfFeature);
        }

        babelwires::FileDataSource midiFile(tempFile);

        const auto feature = smf::parseSmfSequence(midiFile, log);
        ASSERT_NE(feature, nullptr);
        auto smfFeature = feature.get()->asA<const smf::source::Format0SmfFeature>();
        ASSERT_NE(smfFeature, nullptr);

        checkMetadata(*smfFeature, metadata);

        EXPECT_EQ(smfFeature->getNumMidiTracks(), 1);
        const auto& channelGroup = dynamic_cast<const smf::source::RecordChannelGroup&>(smfFeature->getMidiTrack(0));
        EXPECT_EQ(channelGroup.getNumFeatures(), 1);
        const auto* trackFeature = channelGroup.getFeature(0)->asA<const seqwires::TrackFeature>();
        ASSERT_NE(trackFeature, nullptr);
        ASSERT_EQ(channelGroup.getStepToChild(trackFeature), babelwires::PathStep(babelwires::FieldIdentifier("ch2")));

        testUtils::testSimpleNotes(pitches, trackFeature->get());
    }
}

namespace {
    const std::vector<seqwires::Pitch> chordPitches[3] = { {60, 62, 64, 65, 67, 69, 71, 72}, {64, 65, 67, 69, 71, 72, 74, 76}, {67, 69, 71, 72, 74, 76, 77, 79} };
}

TEST(SmfSaveLoadTest, format0Chords) {
    testUtils::TestLog log;
    testUtils::TempFilePath tempFile("format0Chords.mid");

    const char* trackName[3] = { "ch0", "ch1", "ch2" };

    {
        smf::target::Format0SmfFeature smfFeature;
        smfFeature.setToDefault();

        auto* tracks = smfFeature.getChildFromStep(babelwires::PathStep("tracks")).asA<babelwires::ArrayFeature>();
        ASSERT_NE(tracks, nullptr);
        tracks->addEntry();
        tracks->addEntry();
        EXPECT_EQ(tracks->getNumFeatures(), 3);

        for (int i = 0; i < 3; ++i) {
            auto* channelTrack = tracks->getFeature(i)->asA<babelwires::RecordFeature>();
            ASSERT_NE(channelTrack, nullptr);
            EXPECT_EQ(channelTrack->getNumFeatures(), 2);
            auto* channelFeature = channelTrack->getChildFromStep(babelwires::PathStep("Chan")).asA<babelwires::IntFeature>();
            channelFeature->set(i);
            auto* trackFeature = channelTrack->getChildFromStep(babelwires::PathStep("Track")).asA<seqwires::TrackFeature>();
            auto track = std::make_unique<seqwires::Track>();
            testUtils::addSimpleNotes(chordPitches[i], *track);
            trackFeature->set(std::move(track));
        }

        std::ofstream os(tempFile);
        smf::writeToSmfFormat0(os, smfFeature);
    }

    {
        babelwires::FileDataSource midiFile(tempFile);

        const auto feature = smf::parseSmfSequence(midiFile, log);
        ASSERT_NE(feature, nullptr);
        auto smfFeature = feature.get()->asA<const smf::source::Format0SmfFeature>();
        ASSERT_NE(smfFeature, nullptr);

        EXPECT_EQ(smfFeature->getNumMidiTracks(), 1);
        const auto& channelGroup = dynamic_cast<const smf::source::RecordChannelGroup&>(smfFeature->getMidiTrack(0));
        EXPECT_EQ(channelGroup.getNumFeatures(), 3);

        for (int i = 0; i < 3; ++i) {
            const auto* trackFeature = channelGroup.getFeature(i)->asA<const seqwires::TrackFeature>();
            ASSERT_NE(trackFeature, nullptr);
            ASSERT_EQ(channelGroup.getStepToChild(trackFeature), babelwires::PathStep(babelwires::FieldIdentifier(trackName[i])));
            testUtils::testSimpleNotes(chordPitches[i], trackFeature->get());
        }
    }
}

TEST(SmfSaveLoadTest, format1Chords) {
    testUtils::TestLog log;
    testUtils::TempFilePath tempFile("format1Chords.mid");

    const char* trackName[3] = { "ch0", "ch1", "ch2" };

    {
        smf::target::Format1SmfFeature smfFeature;
        smfFeature.setToDefault();

        auto* tracks = smfFeature.getChildFromStep(babelwires::PathStep("tracks")).asA<babelwires::ArrayFeature>();
        ASSERT_NE(tracks, nullptr);
        tracks->addEntry();
        tracks->addEntry();
        EXPECT_EQ(tracks->getNumFeatures(), 3);

        for (int i = 0; i < 3; ++i) {
            auto* channelTrack = tracks->getFeature(i)->asA<babelwires::RecordFeature>();
            ASSERT_NE(channelTrack, nullptr);
            EXPECT_EQ(channelTrack->getNumFeatures(), 2);
            auto* channelFeature = channelTrack->getChildFromStep(babelwires::PathStep("Chan")).asA<babelwires::IntFeature>();
            channelFeature->set(i);
            auto* trackFeature = channelTrack->getChildFromStep(babelwires::PathStep("Track")).asA<seqwires::TrackFeature>();
            auto track = std::make_unique<seqwires::Track>();
            testUtils::addSimpleNotes(chordPitches[i], *track);
            trackFeature->set(std::move(track));
        }

        std::ofstream os(tempFile);
        smf::writeToSmfFormat1(os, smfFeature);
    }

    {
        babelwires::FileDataSource midiFile(tempFile);

        const auto feature = smf::parseSmfSequence(midiFile, log);
        ASSERT_NE(feature, nullptr);
        auto smfFeature = feature.get()->asA<const smf::source::Format1SmfFeature>();
        ASSERT_NE(smfFeature, nullptr);

        EXPECT_EQ(smfFeature->getNumMidiTracks(), 3);
        for (int i = 0; i < 3; ++i) {
            const auto& channelGroup = dynamic_cast<const smf::source::ExtensibleChannelGroup&>(smfFeature->getMidiTrack(i));
            EXPECT_EQ(channelGroup.getNumFeatures(), 2);
            const auto* channelFeature = channelGroup.getChildFromStep(babelwires::PathStep("ChanNo")).asA<const babelwires::IntFeature>();
            ASSERT_NE(channelFeature, nullptr);
            EXPECT_EQ(channelFeature->get(), i);
            const auto* trackFeature = channelGroup.getChildFromStep(babelwires::PathStep("Track")).asA<const seqwires::TrackFeature>();
            ASSERT_NE(trackFeature, nullptr);
            testUtils::testSimpleNotes(chordPitches[i], trackFeature->get());
        }
    }
}

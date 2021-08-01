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

        auto* tracks = dynamic_cast<babelwires::ArrayFeature*>(&smfFeature.getChildFromStep(babelwires::PathStep("tracks")));
        ASSERT_NE(tracks, nullptr);
        EXPECT_EQ(tracks->getNumFeatures(), 1);

        auto* channelTrack = dynamic_cast<babelwires::RecordFeature*>(tracks->getFeature(0));
        ASSERT_NE(channelTrack, nullptr);
        EXPECT_EQ(channelTrack->getNumFeatures(), 2);

        auto* channelFeature = dynamic_cast<babelwires::IntFeature*>(&channelTrack->getChildFromStep(babelwires::PathStep("Chan")));
        channelFeature->set(2);

        auto* trackFeature = dynamic_cast<seqwires::TrackFeature*>(&channelTrack->getChildFromStep(babelwires::PathStep("Track")));

        auto track = std::make_unique<seqwires::Track>();

        testUtils::addSimpleNotes(pitches, *track);

        trackFeature->set(std::move(track));

        std::ofstream os(tempFile);
        smf::writeToSmfFormat0(os, smfFeature);
    }

    {
        babelwires::FileDataSource midiFile(tempFile);

        const auto feature = smf::parseSmfSequence(midiFile);
        ASSERT_NE(feature, nullptr);
        auto smfFeature = dynamic_cast<const smf::source::Format0SmfFeature*>(feature.get());
        ASSERT_NE(smfFeature, nullptr);

        EXPECT_EQ(smfFeature->getNumMidiTracks(), 1);
        const auto& channelGroup = dynamic_cast<const smf::source::RecordChannelGroup&>(smfFeature->getMidiTrack(0));
        EXPECT_EQ(channelGroup.getNumFeatures(), 1);
        const auto* trackFeature = dynamic_cast<const seqwires::TrackFeature*>(channelGroup.getFeature(0));
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

            auto* tracks = dynamic_cast<babelwires::ArrayFeature*>(&smfFeature.getChildFromStep(babelwires::PathStep("tracks")));
            ASSERT_NE(tracks, nullptr);
            EXPECT_EQ(tracks->getNumFeatures(), 1);

            auto* channelTrack = dynamic_cast<babelwires::RecordFeature*>(tracks->getFeature(0));
            ASSERT_NE(channelTrack, nullptr);
            EXPECT_EQ(channelTrack->getNumFeatures(), 2);

            auto* channelFeature = dynamic_cast<babelwires::IntFeature*>(&channelTrack->getChildFromStep(babelwires::PathStep("Chan")));
            channelFeature->set(2);

            auto* trackFeature = dynamic_cast<seqwires::TrackFeature*>(&channelTrack->getChildFromStep(babelwires::PathStep("Track")));

            auto track = std::make_unique<seqwires::Track>();

            testUtils::addSimpleNotes(pitches, *track);

            trackFeature->set(std::move(track));

            std::ofstream os(tempFile);
            smf::writeToSmfFormat0(os, smfFeature);
        }

        babelwires::FileDataSource midiFile(tempFile);

        const auto feature = smf::parseSmfSequence(midiFile);
        ASSERT_NE(feature, nullptr);
        auto smfFeature = dynamic_cast<const smf::source::Format0SmfFeature*>(feature.get());
        ASSERT_NE(smfFeature, nullptr);

        checkMetadata(*smfFeature, metadata);

        EXPECT_EQ(smfFeature->getNumMidiTracks(), 1);
        const auto& channelGroup = dynamic_cast<const smf::source::RecordChannelGroup&>(smfFeature->getMidiTrack(0));
        EXPECT_EQ(channelGroup.getNumFeatures(), 1);
        const auto* trackFeature = dynamic_cast<const seqwires::TrackFeature*>(channelGroup.getFeature(0));
        ASSERT_NE(trackFeature, nullptr);
        ASSERT_EQ(channelGroup.getStepToChild(trackFeature), babelwires::PathStep(babelwires::FieldIdentifier("ch2")));

        testUtils::testSimpleNotes(pitches, trackFeature->get());
    }
}

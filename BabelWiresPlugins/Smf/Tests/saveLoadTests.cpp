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

        const auto& metadata = smfFeature->getMidiMetadata();
        //ASSERT_NE(metadata.getSequenceName(), nullptr);
        //ASSERT_NE(metadata.getCopyright(), nullptr);
        //EXPECT_EQ(metadata.getSequenceName()->get(), "C Major Scale Test");
        //EXPECT_EQ(metadata.getCopyright()->get(), "https://jazz-soft.net");

        EXPECT_EQ(smfFeature->getNumMidiTracks(), 1);
        const auto& channelGroup = dynamic_cast<const smf::source::RecordChannelGroup&>(smfFeature->getMidiTrack(0));
        EXPECT_EQ(channelGroup.getNumFeatures(), 1);
        const auto* trackFeature = dynamic_cast<const seqwires::TrackFeature*>(channelGroup.getFeature(0));
        ASSERT_NE(trackFeature, nullptr);
        ASSERT_EQ(channelGroup.getStepToChild(trackFeature), babelwires::PathStep(babelwires::FieldIdentifier("ch2")));

        const seqwires::Track& track = trackFeature->get();
        const auto& categoryMap = track.getNumEventGroupsByCategory();
        EXPECT_NE(categoryMap.find(seqwires::NoteEvent::s_noteEventCategory), categoryMap.end());
        EXPECT_EQ(categoryMap.find(seqwires::NoteEvent::s_noteEventCategory)->second, 8);
        EXPECT_EQ(track.getDuration(), babelwires::Rational(1, 4) * 8);

        testUtils::testSimpleNotes(pitches, track);
    }
}

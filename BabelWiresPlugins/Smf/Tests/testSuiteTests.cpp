#include <gtest/gtest.h>

#include <BabelWiresPlugins/Smf/Plugin/smfParser.hpp>
#include <BabelWiresPlugins/Smf/Plugin/smfSourceModel.hpp>

#include <SeqWiresLib/Features/trackFeature.hpp>
#include <SeqWiresLib/Tracks/noteEvents.hpp>

#include <BabelWires/Common/IO/fileDataSource.hpp>

#include <Tests/TestUtils/testLog.hpp>

TEST(SmfTest, loadAllTestFilesWithoutCrashing) {
    testUtils::TestLog log;
    int numFilesTested = 0;

    for (auto& p: std::filesystem::directory_iterator(std::filesystem::current_path())) {
        if (p.path().extension() == ".mid") {
            ++numFilesTested;
            try {
                babelwires::FileDataSource midiFile(p.path());
                smf::parseSmfSequence(midiFile);
            } catch(const babelwires::ParseException&) {
                // Allowed.
            } catch(...) {
                EXPECT_TRUE(false);
            }
        }
    }

    EXPECT_GT(numFilesTested, 0);
}

namespace {
    const char* channelNames[] = { "ch0", "ch1", "ch2" };

    void testSimpleNotes(const std::vector<seqwires::Pitch>& expectedPitches, seqwires::Track::const_iterator noteIterator, const seqwires::Track::const_iterator& endIterator) {
        for (auto pitch : expectedPitches)
        {
            EXPECT_NE(noteIterator, endIterator);
            auto noteOn = noteIterator->asA<const seqwires::NoteOnEvent>();
            ASSERT_NE(noteOn, nullptr);
            EXPECT_EQ(noteOn->getTimeSinceLastEvent(), 0);
            EXPECT_EQ(noteOn->m_pitch, pitch);
            EXPECT_EQ(noteOn->m_velocity, 127);
            ++noteIterator;

            EXPECT_NE(noteIterator, endIterator);
            auto noteOff = noteIterator->asA<seqwires::NoteOffEvent>();
            ASSERT_NE(noteOff, nullptr);
            EXPECT_EQ(noteOff->getTimeSinceLastEvent(), babelwires::Rational(1, 4));
            EXPECT_EQ(noteOff->m_pitch, pitch);
            EXPECT_EQ(noteOff->m_velocity, 64);
            ++noteIterator;
        }
        EXPECT_EQ(noteIterator, endIterator);
    }
}

TEST(SmfTest, cMajorScale) {
    testUtils::TestLog log;

    babelwires::FileDataSource midiFile("test-c-major-scale.mid");

    const auto feature = smf::parseSmfSequence(midiFile);
    ASSERT_NE(feature, nullptr);
    auto smfFeature = dynamic_cast<const smf::source::Format0SmfFeature*>(feature.get());
    ASSERT_NE(smfFeature, nullptr);

    const auto& metadata = smfFeature->getMidiMetadata();
    ASSERT_NE(metadata.getSequenceName(), nullptr);
    ASSERT_NE(metadata.getCopyright(), nullptr);
    EXPECT_EQ(metadata.getSequenceName()->get(), "C Major Scale Test");
    EXPECT_EQ(metadata.getCopyright()->get(), "https://jazz-soft.net");

    EXPECT_EQ(smfFeature->getNumMidiTracks(), 1);
    const auto& channelGroup = dynamic_cast<const smf::source::RecordChannelGroup&>(smfFeature->getMidiTrack(0));
    EXPECT_EQ(channelGroup.getNumFeatures(), 1);
    const auto* trackFeature = dynamic_cast<const seqwires::TrackFeature*>(channelGroup.getFeature(0));
    ASSERT_NE(trackFeature, nullptr);
    ASSERT_EQ(channelGroup.getStepToChild(trackFeature), babelwires::PathStep(babelwires::FieldIdentifier("ch0")));

    const seqwires::Track& track = trackFeature->get();
    const auto& categoryMap = track.getNumEventGroupsByCategory();
    EXPECT_NE(categoryMap.find(seqwires::NoteEvent::s_noteEventCategory), categoryMap.end());
    EXPECT_EQ(categoryMap.find(seqwires::NoteEvent::s_noteEventCategory)->second, 8);
    EXPECT_EQ(track.getDuration(), babelwires::Rational(1, 4) * 8);

    // Note: right now, we only parse notes. As we add parsing of other event types, this test will fail.
    // (A temporarily fix would be to use a FilteredTrackIterator.)
    auto noteIterator = track.begin();
    const auto endIterator = track.end();

    testSimpleNotes(std::vector<seqwires::Pitch>{60, 62, 64, 65, 67, 69, 71, 72}, track.begin(), track.end());
}

TEST(SmfTest, multichannelChords0) {
    testUtils::TestLog log;

    babelwires::FileDataSource midiFile("test-multichannel-chords-0.mid");

    const auto feature = smf::parseSmfSequence(midiFile);    
    ASSERT_NE(feature, nullptr);
    auto smfFeature = dynamic_cast<const smf::source::Format0SmfFeature*>(feature.get());
    ASSERT_NE(smfFeature, nullptr);

    const auto& metadata = smfFeature->getMidiMetadata();
    ASSERT_NE(metadata.getSequenceName(), nullptr);
    EXPECT_EQ(metadata.getSequenceName()->get(), "Multi-channel chords Test 0");

    EXPECT_EQ(smfFeature->getNumMidiTracks(), 1);
    const auto& channelGroup = dynamic_cast<const smf::source::RecordChannelGroup&>(smfFeature->getMidiTrack(0));
    EXPECT_EQ(channelGroup.getNumFeatures(), 3);

    const seqwires::Track* tracks[3] = {};

    for (int i = 0; i < 3; ++i) {
        const auto* trackFeature = dynamic_cast<const seqwires::TrackFeature*>(channelGroup.getFeature(i));
        ASSERT_NE(trackFeature, nullptr);
        ASSERT_EQ(channelGroup.getStepToChild(trackFeature), babelwires::PathStep(babelwires::FieldIdentifier(channelNames[i])));
        tracks[i] = &trackFeature->get();
    }

    testSimpleNotes(std::vector<seqwires::Pitch>{60, 62, 64, 65, 67, 69, 71, 72}, tracks[0]->begin(), tracks[0]->end());
    testSimpleNotes(std::vector<seqwires::Pitch>{64, 65, 67, 69, 71, 72, 74, 76}, tracks[1]->begin(), tracks[1]->end());
    testSimpleNotes(std::vector<seqwires::Pitch>{67, 69, 71, 72, 74, 76, 77, 79}, tracks[2]->begin(), tracks[2]->end());
}

TEST(SmfTest, multichannelChords1) {
    testUtils::TestLog log;

    babelwires::FileDataSource midiFile("test-multichannel-chords-1.mid");

    const auto feature = smf::parseSmfSequence(midiFile);    
    ASSERT_NE(feature, nullptr);
    auto smfFeature = dynamic_cast<const smf::source::Format1SmfFeature*>(feature.get());
    ASSERT_NE(smfFeature, nullptr);

    const auto& metadata = smfFeature->getMidiMetadata();
    ASSERT_NE(metadata.getSequenceName(), nullptr);
    EXPECT_EQ(metadata.getSequenceName()->get(), "Multi-channel chords Test 1");

    EXPECT_EQ(smfFeature->getNumMidiTracks(), 3);

    const seqwires::Track* tracks[3] = {};

    for (int i = 0; i < 3; ++i) {
        const auto& channelGroup = dynamic_cast<const smf::source::ExtensibleChannelGroup&>(smfFeature->getMidiTrack(i));
        EXPECT_EQ(channelGroup.getNumFeatures(), 2);

        const auto* channelNumFeature = dynamic_cast<const babelwires::IntFeature*>(channelGroup.getFeature(0));
        ASSERT_NE(channelNumFeature, nullptr);
        ASSERT_EQ(channelGroup.getStepToChild(channelNumFeature), babelwires::PathStep(babelwires::FieldIdentifier("ChanNo")));
        EXPECT_EQ(channelNumFeature->get(), i);

        const auto* trackFeature = dynamic_cast<const seqwires::TrackFeature*>(channelGroup.getFeature(1));
        ASSERT_NE(trackFeature, nullptr);
        ASSERT_EQ(channelGroup.getStepToChild(trackFeature), babelwires::PathStep(babelwires::FieldIdentifier("Track")));
        tracks[i] = &trackFeature->get();
    }

    testSimpleNotes(std::vector<seqwires::Pitch>{60, 62, 64, 65, 67, 69, 71, 72}, tracks[0]->begin(), tracks[0]->end());
    testSimpleNotes(std::vector<seqwires::Pitch>{64, 65, 67, 69, 71, 72, 74, 76}, tracks[1]->begin(), tracks[1]->end());
    testSimpleNotes(std::vector<seqwires::Pitch>{67, 69, 71, 72, 74, 76, 77, 79}, tracks[2]->begin(), tracks[2]->end());
}

TEST(SmfTest, multichannelChords2) {
    testUtils::TestLog log;

    babelwires::FileDataSource midiFile("test-multichannel-chords-2.mid");

    const auto feature = smf::parseSmfSequence(midiFile);    
    ASSERT_NE(feature, nullptr);
    auto smfFeature = dynamic_cast<const smf::source::Format1SmfFeature*>(feature.get());
    ASSERT_NE(smfFeature, nullptr);

    const auto& metadata = smfFeature->getMidiMetadata();
    ASSERT_NE(metadata.getSequenceName(), nullptr);
    EXPECT_EQ(metadata.getSequenceName()->get(), "Multi-channel chords Test 2");

    EXPECT_EQ(smfFeature->getNumMidiTracks(), 2);

    const seqwires::Track* tracks[3] = {};

    const auto& channelGroup0 = dynamic_cast<const smf::source::ExtensibleChannelGroup&>(smfFeature->getMidiTrack(0));
    EXPECT_EQ(channelGroup0.getNumFeatures(), 3);

    const auto* channelNumFeature = dynamic_cast<const babelwires::IntFeature*>(channelGroup0.getFeature(0));
    ASSERT_NE(channelNumFeature, nullptr);
    ASSERT_EQ(channelGroup0.getStepToChild(channelNumFeature), babelwires::PathStep(babelwires::FieldIdentifier("ChanNo")));
    EXPECT_EQ(channelNumFeature->get(), 0);

    const auto* trackFeature0 = dynamic_cast<const seqwires::TrackFeature*>(channelGroup0.getFeature(1));
    ASSERT_NE(trackFeature0, nullptr);
    ASSERT_EQ(channelGroup0.getStepToChild(trackFeature0), babelwires::PathStep(babelwires::FieldIdentifier("Track")));
    tracks[0] = &trackFeature0->get();

    const auto* trackFeature1 = dynamic_cast<const seqwires::TrackFeature*>(channelGroup0.getFeature(2));
    ASSERT_NE(trackFeature1, nullptr);
    ASSERT_EQ(channelGroup0.getStepToChild(trackFeature1), babelwires::PathStep(babelwires::FieldIdentifier("ex1")));
    tracks[1] = &trackFeature1->get();

    const auto& channelGroup1 = dynamic_cast<const smf::source::ExtensibleChannelGroup&>(smfFeature->getMidiTrack(1));
    EXPECT_EQ(channelGroup1.getNumFeatures(), 2);

    const auto* channelNumFeature2 = dynamic_cast<const babelwires::IntFeature*>(channelGroup1.getFeature(0));
    ASSERT_NE(channelNumFeature2, nullptr);
    ASSERT_EQ(channelGroup1.getStepToChild(channelNumFeature2), babelwires::PathStep(babelwires::FieldIdentifier("ChanNo")));
    EXPECT_EQ(channelNumFeature2->get(), 2);

    const auto* trackFeature2 = dynamic_cast<const seqwires::TrackFeature*>(channelGroup1.getFeature(1));
    ASSERT_NE(trackFeature2, nullptr);
    ASSERT_EQ(channelGroup1.getStepToChild(trackFeature2), babelwires::PathStep(babelwires::FieldIdentifier("Track")));
    tracks[2] = &trackFeature2->get();

    testSimpleNotes(std::vector<seqwires::Pitch>{60, 62, 64, 65, 67, 69, 71, 72}, tracks[0]->begin(), tracks[0]->end());
    testSimpleNotes(std::vector<seqwires::Pitch>{64, 65, 67, 69, 71, 72, 74, 76}, tracks[1]->begin(), tracks[1]->end());
    testSimpleNotes(std::vector<seqwires::Pitch>{67, 69, 71, 72, 74, 76, 77, 79}, tracks[2]->begin(), tracks[2]->end());
}

TEST(SmfTest, multichannelChords3) {
    testUtils::TestLog log;

    babelwires::FileDataSource midiFile("test-multichannel-chords-3.mid");

    const auto feature = smf::parseSmfSequence(midiFile);    
    ASSERT_NE(feature, nullptr);
    auto smfFeature = dynamic_cast<const smf::source::Format1SmfFeature*>(feature.get());
    ASSERT_NE(smfFeature, nullptr);

    const auto& metadata = smfFeature->getMidiMetadata();
    ASSERT_NE(metadata.getSequenceName(), nullptr);
    EXPECT_EQ(metadata.getSequenceName()->get(), "Multi-channel chords Test 3");

    EXPECT_EQ(smfFeature->getNumMidiTracks(), 3);

    const seqwires::Track* tracks[3] = {};

    const int expectedChannelMapping[] = { 0, 1, 0};

    for (int i = 0; i < 3; ++i) {
        const auto& channelGroup = dynamic_cast<const smf::source::ExtensibleChannelGroup&>(smfFeature->getMidiTrack(i));
        EXPECT_EQ(channelGroup.getNumFeatures(), 2);

        const auto* channelNumFeature = dynamic_cast<const babelwires::IntFeature*>(channelGroup.getFeature(0));
        ASSERT_NE(channelNumFeature, nullptr);
        ASSERT_EQ(channelGroup.getStepToChild(channelNumFeature), babelwires::PathStep(babelwires::FieldIdentifier("ChanNo")));
        EXPECT_EQ(channelNumFeature->get(), expectedChannelMapping[i]);

        const auto* trackFeature = dynamic_cast<const seqwires::TrackFeature*>(channelGroup.getFeature(1));
        ASSERT_NE(trackFeature, nullptr);
        ASSERT_EQ(channelGroup.getStepToChild(trackFeature), babelwires::PathStep(babelwires::FieldIdentifier("Track")));
        tracks[i] = &trackFeature->get();
    }

    testSimpleNotes(std::vector<seqwires::Pitch>{60, 62, 64, 65, 67, 69, 71, 72}, tracks[0]->begin(), tracks[0]->end());
    testSimpleNotes(std::vector<seqwires::Pitch>{64, 65, 67, 69, 71, 72, 74, 76}, tracks[1]->begin(), tracks[1]->end());
    testSimpleNotes(std::vector<seqwires::Pitch>{67, 69, 71, 72, 74, 76, 77, 79}, tracks[2]->begin(), tracks[2]->end());
}

TEST(SmfTest, trackLength) {
    testUtils::TestLog log;

    babelwires::FileDataSource midiFile("test-track-length.mid");

    const auto feature = smf::parseSmfSequence(midiFile);
    ASSERT_NE(feature, nullptr);
    auto smfFeature = dynamic_cast<const smf::source::Format0SmfFeature*>(feature.get());
    ASSERT_NE(smfFeature, nullptr);

    EXPECT_EQ(smfFeature->getNumMidiTracks(), 1);
    const auto& channelGroup = dynamic_cast<const smf::source::RecordChannelGroup&>(smfFeature->getMidiTrack(0));
    EXPECT_EQ(channelGroup.getNumFeatures(), 1);
    const auto* trackFeature = dynamic_cast<const seqwires::TrackFeature*>(channelGroup.getFeature(0));
    ASSERT_NE(trackFeature, nullptr);

    const seqwires::Track& track = trackFeature->get();
    EXPECT_EQ(track.getDuration(), babelwires::Rational(3, 4));
}

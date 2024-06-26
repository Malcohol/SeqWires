#include <gtest/gtest.h>

#include <Plugins/Smf/Plugin/Percussion/gm2StandardPercussionSet.hpp>
#include <Plugins/Smf/Plugin/libRegistration.hpp>
#include <Plugins/Smf/Plugin/smfParser.hpp>
#include <Plugins/Smf/Plugin/smfSourceModel.hpp>

#include <SeqWiresLib/Types/Track/trackFeature.hpp>
#include <SeqWiresLib/Types/Track/TrackEvents/noteEvents.hpp>
#include <SeqWiresLib/Types/Track/TrackEvents/percussionEvents.hpp>
#include <SeqWiresLib/Utilities/filteredTrackIterator.hpp>
#include <SeqWiresLib/libRegistration.hpp>

#include <Common/IO/fileDataSource.hpp>

#include <Tests/BabelWiresLib/TestUtils/testEnvironment.hpp>

#include <Tests/TestUtils/seqTestUtils.hpp>

TEST(SmfTestSuiteTest, loadAllTestFilesWithoutCrashing) {
    testUtils::TestEnvironment testEnvironment;
    seqwires::registerLib(testEnvironment.m_projectContext);
    smf::registerLib(testEnvironment.m_projectContext);

    int numFilesTested = 0;

    for (auto& p : std::filesystem::directory_iterator(std::filesystem::current_path())) {
        if (p.path().extension() == ".mid") {
            ++numFilesTested;
            try {
                babelwires::FileDataSource midiFile(p.path());
                smf::parseSmfSequence(midiFile, testEnvironment.m_projectContext, testEnvironment.m_log);
            } catch (const babelwires::ParseException&) {
                // Allowed.
            } catch (...) {
                EXPECT_TRUE(false);
            }
        }
    }

    EXPECT_GT(numFilesTested, 0);
}

namespace {
    const char* channelNames[] = {"ch0", "ch1", "ch2"};
} // namespace

TEST(SmfTestSuiteTest, cMajorScale) {
    testUtils::TestEnvironment testEnvironment;
    seqwires::registerLib(testEnvironment.m_projectContext);
    smf::registerLib(testEnvironment.m_projectContext);

    babelwires::FileDataSource midiFile("test-c-major-scale.mid");

    const auto feature = smf::parseSmfSequence(midiFile, testEnvironment.m_projectContext, testEnvironment.m_log);
    ASSERT_NE(feature, nullptr);
    auto smfFeature = feature.get()->as<const smf::source::Format0SmfFeature>();
    ASSERT_NE(smfFeature, nullptr);

    const auto& metadata = smf::MidiMetadata::Instance<const babelwires::ValueFeature>(smfFeature->getMidiMetadata());
    ASSERT_TRUE(metadata.tryGetName().has_value());
    EXPECT_EQ(metadata.tryGetName()->get(), "C Major Scale Test");
    ASSERT_TRUE(metadata.tryGetCopyR().has_value());
    EXPECT_EQ(metadata.tryGetCopyR()->get(), "https://jazz-soft.net");

    EXPECT_EQ(smfFeature->getNumMidiTracks(), 1);
    const auto& channelGroup = dynamic_cast<const smf::source::RecordChannelGroup&>(smfFeature->getMidiTrack(0));
    EXPECT_EQ(channelGroup.getNumFeatures(), 1);
    const auto* trackFeature = channelGroup.getFeature(0)->as<const seqwires::TrackFeature>();
    ASSERT_NE(trackFeature, nullptr);
    ASSERT_EQ(channelGroup.getStepToChild(trackFeature), babelwires::PathStep(babelwires::ShortId("ch0")));

    const seqwires::Track& track = trackFeature->get();
    const auto& categoryMap = track.getNumEventGroupsByCategory();
    EXPECT_NE(categoryMap.find(seqwires::NoteEvent::s_noteEventCategory), categoryMap.end());
    EXPECT_EQ(categoryMap.find(seqwires::NoteEvent::s_noteEventCategory)->second, 8);
    EXPECT_EQ(track.getDuration(), babelwires::Rational(1, 4) * 8);

    testUtils::testSimpleNotes(std::vector<seqwires::Pitch>{60, 62, 64, 65, 67, 69, 71, 72}, track);
}

TEST(SmfTestSuiteTest, multichannelChords0) {
    testUtils::TestEnvironment testEnvironment;
    seqwires::registerLib(testEnvironment.m_projectContext);
    smf::registerLib(testEnvironment.m_projectContext);

    babelwires::FileDataSource midiFile("test-multichannel-chords-0.mid");

    const auto feature = smf::parseSmfSequence(midiFile, testEnvironment.m_projectContext, testEnvironment.m_log);
    ASSERT_NE(feature, nullptr);
    auto smfFeature = feature.get()->as<const smf::source::Format0SmfFeature>();
    ASSERT_NE(smfFeature, nullptr);

    const auto& metadata = smf::MidiMetadata::Instance<const babelwires::ValueFeature>(smfFeature->getMidiMetadata());
    ASSERT_TRUE(metadata.tryGetName().has_value());
    EXPECT_EQ(metadata.tryGetName()->get(), "Multi-channel chords Test 0");

    EXPECT_EQ(smfFeature->getNumMidiTracks(), 1);
    const auto& channelGroup = dynamic_cast<const smf::source::RecordChannelGroup&>(smfFeature->getMidiTrack(0));
    EXPECT_EQ(channelGroup.getNumFeatures(), 3);

    const seqwires::Track* tracks[3] = {};

    for (int i = 0; i < 3; ++i) {
        const auto* trackFeature = channelGroup.getFeature(i)->as<const seqwires::TrackFeature>();
        ASSERT_NE(trackFeature, nullptr);
        ASSERT_EQ(channelGroup.getStepToChild(trackFeature),
                  babelwires::PathStep(babelwires::ShortId(channelNames[i])));
        tracks[i] = &trackFeature->get();
    }

    testUtils::testSimpleNotes(std::vector<seqwires::Pitch>{60, 62, 64, 65, 67, 69, 71, 72}, *tracks[0]);
    testUtils::testSimpleNotes(std::vector<seqwires::Pitch>{64, 65, 67, 69, 71, 72, 74, 76}, *tracks[1]);
    testUtils::testSimpleNotes(std::vector<seqwires::Pitch>{67, 69, 71, 72, 74, 76, 77, 79}, *tracks[2]);
}

TEST(SmfTestSuiteTest, multichannelChords1) {
    testUtils::TestEnvironment testEnvironment;
    seqwires::registerLib(testEnvironment.m_projectContext);
    smf::registerLib(testEnvironment.m_projectContext);

    babelwires::FileDataSource midiFile("test-multichannel-chords-1.mid");

    const auto feature = smf::parseSmfSequence(midiFile, testEnvironment.m_projectContext, testEnvironment.m_log);
    ASSERT_NE(feature, nullptr);
    auto smfFeature = feature.get()->as<const smf::source::Format1SmfFeature>();
    ASSERT_NE(smfFeature, nullptr);

    const auto& metadata = smf::MidiMetadata::Instance<const babelwires::ValueFeature>(smfFeature->getMidiMetadata());
    ASSERT_TRUE(metadata.tryGetName().has_value());
    EXPECT_EQ(metadata.tryGetName()->get(), "Multi-channel chords Test 1");

    EXPECT_EQ(smfFeature->getNumMidiTracks(), 3);

    const seqwires::Track* tracks[3] = {};

    for (int i = 0; i < 3; ++i) {
        const auto& channelGroup =
            dynamic_cast<const smf::source::ExtensibleChannelGroup&>(smfFeature->getMidiTrack(i));
        EXPECT_EQ(channelGroup.getNumFeatures(), 2);

        const auto* channelNumFeature = channelGroup.getFeature(0)->as<const babelwires::IntFeature>();
        ASSERT_NE(channelNumFeature, nullptr);
        ASSERT_EQ(channelGroup.getStepToChild(channelNumFeature),
                  babelwires::PathStep(babelwires::ShortId("ChanNo")));
        EXPECT_EQ(channelNumFeature->get(), i);

        const auto* trackFeature = channelGroup.getFeature(1)->as<const seqwires::TrackFeature>();
        ASSERT_NE(trackFeature, nullptr);
        ASSERT_EQ(channelGroup.getStepToChild(trackFeature), babelwires::PathStep(babelwires::ShortId("Track")));
        tracks[i] = &trackFeature->get();
    }

    testUtils::testSimpleNotes(std::vector<seqwires::Pitch>{60, 62, 64, 65, 67, 69, 71, 72}, *tracks[0]);
    testUtils::testSimpleNotes(std::vector<seqwires::Pitch>{64, 65, 67, 69, 71, 72, 74, 76}, *tracks[1]);
    testUtils::testSimpleNotes(std::vector<seqwires::Pitch>{67, 69, 71, 72, 74, 76, 77, 79}, *tracks[2]);
}

TEST(SmfTestSuiteTest, multichannelChords2) {
    testUtils::TestEnvironment testEnvironment;
    seqwires::registerLib(testEnvironment.m_projectContext);
    smf::registerLib(testEnvironment.m_projectContext);

    babelwires::FileDataSource midiFile("test-multichannel-chords-2.mid");

    const auto feature = smf::parseSmfSequence(midiFile, testEnvironment.m_projectContext, testEnvironment.m_log);
    ASSERT_NE(feature, nullptr);
    auto smfFeature = feature.get()->as<const smf::source::Format1SmfFeature>();
    ASSERT_NE(smfFeature, nullptr);
    
    const auto& metadata = smf::MidiMetadata::Instance<const babelwires::ValueFeature>(smfFeature->getMidiMetadata());
    ASSERT_TRUE(metadata.tryGetName().has_value());
    EXPECT_EQ(metadata.tryGetName()->get(), "Multi-channel chords Test 2");

    EXPECT_EQ(smfFeature->getNumMidiTracks(), 2);

    const seqwires::Track* tracks[3] = {};

    const auto& channelGroup0 = dynamic_cast<const smf::source::ExtensibleChannelGroup&>(smfFeature->getMidiTrack(0));
    EXPECT_EQ(channelGroup0.getNumFeatures(), 3);

    const auto* channelNumFeature = channelGroup0.getFeature(0)->as<const babelwires::IntFeature>();
    ASSERT_NE(channelNumFeature, nullptr);
    ASSERT_EQ(channelGroup0.getStepToChild(channelNumFeature), babelwires::PathStep(babelwires::ShortId("ChanNo")));
    EXPECT_EQ(channelNumFeature->get(), 0);

    const auto* trackFeature0 = channelGroup0.getFeature(1)->as<const seqwires::TrackFeature>();
    ASSERT_NE(trackFeature0, nullptr);
    ASSERT_EQ(channelGroup0.getStepToChild(trackFeature0), babelwires::PathStep(babelwires::ShortId("Track")));
    tracks[0] = &trackFeature0->get();

    const auto* trackFeature1 = channelGroup0.getFeature(2)->as<const seqwires::TrackFeature>();
    ASSERT_NE(trackFeature1, nullptr);
    ASSERT_EQ(channelGroup0.getStepToChild(trackFeature1), babelwires::PathStep(babelwires::ShortId("ex1")));
    tracks[1] = &trackFeature1->get();

    const auto& channelGroup1 = dynamic_cast<const smf::source::ExtensibleChannelGroup&>(smfFeature->getMidiTrack(1));
    EXPECT_EQ(channelGroup1.getNumFeatures(), 2);

    const auto* channelNumFeature2 = channelGroup1.getFeature(0)->as<const babelwires::IntFeature>();
    ASSERT_NE(channelNumFeature2, nullptr);
    ASSERT_EQ(channelGroup1.getStepToChild(channelNumFeature2), babelwires::PathStep(babelwires::ShortId("ChanNo")));
    EXPECT_EQ(channelNumFeature2->get(), 2);

    const auto* trackFeature2 = channelGroup1.getFeature(1)->as<const seqwires::TrackFeature>();
    ASSERT_NE(trackFeature2, nullptr);
    ASSERT_EQ(channelGroup1.getStepToChild(trackFeature2), babelwires::PathStep(babelwires::ShortId("Track")));
    tracks[2] = &trackFeature2->get();

    testUtils::testSimpleNotes(std::vector<seqwires::Pitch>{60, 62, 64, 65, 67, 69, 71, 72}, *tracks[0]);
    testUtils::testSimpleNotes(std::vector<seqwires::Pitch>{64, 65, 67, 69, 71, 72, 74, 76}, *tracks[1]);
    testUtils::testSimpleNotes(std::vector<seqwires::Pitch>{67, 69, 71, 72, 74, 76, 77, 79}, *tracks[2]);
}

TEST(SmfTestSuiteTest, multichannelChords3) {
    testUtils::TestEnvironment testEnvironment;
    seqwires::registerLib(testEnvironment.m_projectContext);
    smf::registerLib(testEnvironment.m_projectContext);

    babelwires::FileDataSource midiFile("test-multichannel-chords-3.mid");

    const auto feature = smf::parseSmfSequence(midiFile, testEnvironment.m_projectContext, testEnvironment.m_log);
    ASSERT_NE(feature, nullptr);
    auto smfFeature = feature.get()->as<const smf::source::Format1SmfFeature>();
    ASSERT_NE(smfFeature, nullptr);

    const auto& metadata = smf::MidiMetadata::Instance<const babelwires::ValueFeature>(smfFeature->getMidiMetadata());
    ASSERT_TRUE(metadata.tryGetName().has_value());
    EXPECT_EQ(metadata.tryGetName()->get(), "Multi-channel chords Test 3");

    EXPECT_EQ(smfFeature->getNumMidiTracks(), 3);

    const seqwires::Track* tracks[3] = {};

    const int expectedChannelMapping[] = {0, 1, 0};

    for (int i = 0; i < 3; ++i) {
        const auto& channelGroup =
            dynamic_cast<const smf::source::ExtensibleChannelGroup&>(smfFeature->getMidiTrack(i));
        EXPECT_EQ(channelGroup.getNumFeatures(), 2);

        const auto* channelNumFeature = channelGroup.getFeature(0)->as<const babelwires::IntFeature>();
        ASSERT_NE(channelNumFeature, nullptr);
        ASSERT_EQ(channelGroup.getStepToChild(channelNumFeature),
                  babelwires::PathStep(babelwires::ShortId("ChanNo")));
        EXPECT_EQ(channelNumFeature->get(), expectedChannelMapping[i]);

        const auto* trackFeature = channelGroup.getFeature(1)->as<const seqwires::TrackFeature>();
        ASSERT_NE(trackFeature, nullptr);
        ASSERT_EQ(channelGroup.getStepToChild(trackFeature), babelwires::PathStep(babelwires::ShortId("Track")));
        tracks[i] = &trackFeature->get();
    }

    testUtils::testSimpleNotes(std::vector<seqwires::Pitch>{60, 62, 64, 65, 67, 69, 71, 72}, *tracks[0]);
    testUtils::testSimpleNotes(std::vector<seqwires::Pitch>{64, 65, 67, 69, 71, 72, 74, 76}, *tracks[1]);
    testUtils::testSimpleNotes(std::vector<seqwires::Pitch>{67, 69, 71, 72, 74, 76, 77, 79}, *tracks[2]);
}

TEST(SmfTestSuiteTest, trackLength) {
    testUtils::TestEnvironment testEnvironment;
    seqwires::registerLib(testEnvironment.m_projectContext);
    smf::registerLib(testEnvironment.m_projectContext);

    babelwires::FileDataSource midiFile("test-track-length.mid");

    const auto feature = smf::parseSmfSequence(midiFile, testEnvironment.m_projectContext, testEnvironment.m_log);
    ASSERT_NE(feature, nullptr);
    auto smfFeature = feature.get()->as<const smf::source::Format0SmfFeature>();
    ASSERT_NE(smfFeature, nullptr);

    EXPECT_EQ(smfFeature->getNumMidiTracks(), 1);
    const auto& channelGroup = dynamic_cast<const smf::source::RecordChannelGroup&>(smfFeature->getMidiTrack(0));
    EXPECT_EQ(channelGroup.getNumFeatures(), 1);
    const auto* trackFeature = channelGroup.getFeature(0)->as<const seqwires::TrackFeature>();
    ASSERT_NE(trackFeature, nullptr);

    const seqwires::Track& track = trackFeature->get();
    EXPECT_EQ(track.getDuration(), babelwires::Rational(3, 4));
}

TEST(SmfTestSuiteTest, tempoTest) {
    testUtils::TestEnvironment testEnvironment;
    seqwires::registerLib(testEnvironment.m_projectContext);
    smf::registerLib(testEnvironment.m_projectContext);

    babelwires::FileDataSource midiFile("test-karaoke-kar.mid");

    const auto feature = smf::parseSmfSequence(midiFile, testEnvironment.m_projectContext, testEnvironment.m_log);
    ASSERT_NE(feature, nullptr);
    auto smfFeature = feature.get()->as<const smf::source::Format1SmfFeature>();
    ASSERT_NE(smfFeature, nullptr);

    const auto& metadata = smf::MidiMetadata::Instance<const babelwires::ValueFeature>(smfFeature->getMidiMetadata());
    ASSERT_TRUE(metadata.tryGetName().has_value());
    EXPECT_EQ(metadata.tryGetName()->get(), "Karaoke .KAR Test");
    ASSERT_TRUE(metadata.tryGetTempo().has_value());
    EXPECT_EQ(metadata.tryGetTempo()->get(), 90);
}

TEST(SmfTestSuiteTest, corruptFiles) {
    testUtils::TestEnvironment testEnvironment;
    seqwires::registerLib(testEnvironment.m_projectContext);
    smf::registerLib(testEnvironment.m_projectContext);

    {
        babelwires::FileDataSource midiFile("test-corrupt-file-extra-byte.mid");
        // This is OK.
        smf::parseSmfSequence(midiFile, testEnvironment.m_projectContext, testEnvironment.m_log);
    }
    {
        babelwires::FileDataSource midiFile("test-corrupt-file-missing-byte.mid");
        EXPECT_THROW(smf::parseSmfSequence(midiFile, testEnvironment.m_projectContext, testEnvironment.m_log),
                     babelwires::ParseException);
    }
}

TEST(SmfTestSuiteTest, testAllGMPercussion) {
    testUtils::TestEnvironment testEnvironment;
    seqwires::registerLib(testEnvironment.m_projectContext);
    smf::registerLib(testEnvironment.m_projectContext);

    babelwires::FileDataSource midiFile("test-all-gm-percussion.mid");

    const auto feature = smf::parseSmfSequence(midiFile, testEnvironment.m_projectContext, testEnvironment.m_log);
    ASSERT_NE(feature, nullptr);
    auto smfFeature = feature.get()->as<const smf::source::Format0SmfFeature>();
    ASSERT_NE(smfFeature, nullptr);

    EXPECT_EQ(smfFeature->getNumMidiTracks(), 1);
    const auto& channelGroup = dynamic_cast<const smf::source::RecordChannelGroup&>(smfFeature->getMidiTrack(0));
    EXPECT_EQ(channelGroup.getNumFeatures(), 1);
    const auto* trackFeature = channelGroup.getFeature(0)->as<const seqwires::TrackFeature>();
    ASSERT_NE(trackFeature, nullptr);
    // Percussion channel
    ASSERT_EQ(channelGroup.getStepToChild(trackFeature), babelwires::PathStep(babelwires::ShortId("ch9")));

    const seqwires::Track& track = trackFeature->get();

    auto categoryMap = track.getNumEventGroupsByCategory();
    EXPECT_EQ(categoryMap.find(seqwires::NoteEvent::s_noteEventCategory), categoryMap.end());
    EXPECT_NE(categoryMap.find(seqwires::PercussionEvent::s_percussionEventCategory), categoryMap.end());

    const auto& gm2StandardPercussionSet = testEnvironment.m_typeSystem.getEntryByType<smf::GM2StandardPercussionSet>();

    // The file has each percussion instrument playing three times.
    const auto& percussionInstruments = gm2StandardPercussionSet.getValueSet();
    int eventIndex = 0;
    for (const auto& percussionEvent : seqwires::iterateOver<seqwires::PercussionEvent>(track)) {
        const int instrumentIndex = eventIndex / 6;
        EXPECT_EQ(percussionEvent.getInstrument(),
                  gm2StandardPercussionSet.tryGetInstrumentFromPitch(instrumentIndex + 27));
        if (eventIndex % 2 == 0) {
            EXPECT_TRUE(percussionEvent.as<seqwires::PercussionOnEvent>());
        } else {
            EXPECT_TRUE(percussionEvent.as<seqwires::PercussionOffEvent>());
        }
        ++eventIndex;
    }
}

TEST(SmfTestSuiteTest, testGSDrumPartChange) {
    testUtils::TestEnvironment testEnvironment;
    seqwires::registerLib(testEnvironment.m_projectContext);
    smf::registerLib(testEnvironment.m_projectContext);

    babelwires::FileDataSource midiFile("test-sysex-gs-40-1x-15-drum-part-change.mid");

    const auto feature = smf::parseSmfSequence(midiFile, testEnvironment.m_projectContext, testEnvironment.m_log);
    ASSERT_NE(feature, nullptr);
    auto smfFeature = feature.get()->as<const smf::source::Format0SmfFeature>();
    ASSERT_NE(smfFeature, nullptr);

    EXPECT_EQ(smfFeature->getNumMidiTracks(), 1);
    const auto& channelGroup = dynamic_cast<const smf::source::RecordChannelGroup&>(smfFeature->getMidiTrack(0));
    EXPECT_EQ(channelGroup.getNumFeatures(), 2);

    {
        const auto* trackFeature = channelGroup.getFeature(0)->as<const seqwires::TrackFeature>();
        ASSERT_NE(trackFeature, nullptr);

        const seqwires::Track& track = trackFeature->get();

        auto categoryMap = track.getNumEventGroupsByCategory();
        EXPECT_EQ(categoryMap.find(seqwires::NoteEvent::s_noteEventCategory), categoryMap.end());
        ASSERT_NE(categoryMap.find(seqwires::PercussionEvent::s_percussionEventCategory), categoryMap.end());
        EXPECT_EQ(categoryMap.find(seqwires::PercussionEvent::s_percussionEventCategory)->second, 4);
    }
    {
        const auto* trackFeature = channelGroup.getFeature(1)->as<const seqwires::TrackFeature>();
        ASSERT_NE(trackFeature, nullptr);

        const seqwires::Track& track = trackFeature->get();

        auto categoryMap = track.getNumEventGroupsByCategory();
        EXPECT_EQ(categoryMap.find(seqwires::PercussionEvent::s_percussionEventCategory), categoryMap.end());
        ASSERT_NE(categoryMap.find(seqwires::NoteEvent::s_noteEventCategory), categoryMap.end());
        EXPECT_EQ(categoryMap.find(seqwires::NoteEvent::s_noteEventCategory)->second, 4);
    }
}

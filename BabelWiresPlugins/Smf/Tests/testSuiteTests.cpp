#include <gtest/gtest.h>

#include <BabelWiresPlugins/Smf/Plugin/smfParser.hpp>
#include <BabelWiresPlugins/Smf/Plugin/smfSourceModel.hpp>

#include <SeqWiresLib/Features/trackFeature.hpp>
#include <SeqWiresLib/Tracks/noteEvents.hpp>

#include <BabelWires/Common/IO/fileDataSource.hpp>
#include <BabelWires/Features/Path/fieldNameRegistry.hpp>

#include <Tests/TestUtils/testLog.hpp>

TEST(SmfTest, loadAllTestFilesWithoutCrashing) {
    babelwires::FieldNameRegistryScope fieldNameRegistry;
    testUtils::TestLog log;

    for (auto& p: std::filesystem::directory_iterator(std::filesystem::current_path())) {
        if (p.path().extension() == ".mid") {
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
}

TEST(SmfTest, cMajorScale) {
    babelwires::FieldNameRegistryScope fieldNameRegistry;
    testUtils::TestLog log;

    babelwires::FileDataSource midiFile("test-c-major-scale.mid");

    const auto feature = smf::parseSmfSequence(midiFile);

    ASSERT_NE(feature, nullptr);
    auto smfFeature = dynamic_cast<const smf::source::Format0SmfFeature*>(feature.get());

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

    for (auto pitch : std::vector<seqwires::Pitch>{60, 62, 64, 65, 67, 69, 71, 72})
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


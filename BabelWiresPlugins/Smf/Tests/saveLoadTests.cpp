#include <gtest/gtest.h>

#include <BabelWiresPlugins/Smf/Plugin/smfParser.hpp>
#include <BabelWiresPlugins/Smf/Plugin/smfSourceModel.hpp>
#include <BabelWiresPlugins/Smf/Plugin/smfWriter.hpp>
#include <BabelWiresPlugins/Smf/Plugin/smfTargetModel.hpp>

#include <SeqWiresLib/Features/trackFeature.hpp>
#include <SeqWiresLib/Tracks/noteEvents.hpp>

#include <BabelWires/Common/IO/fileDataSource.hpp>

#include <Tests/TestUtils/testLog.hpp>

TEST(SmfSaveLoadTest, cMajorScale) {
    testUtils::TestLog log;
    smf::target::Format0SmfFeature smfFeature;

    smf::target::ArrayChannelGroup* tracks = dynamic_cast<smf::target::ArrayChannelGroup*>(&smfFeature.getChildFromStep(babelwires::PathStep("tracks")));
    ASSERT_NE(tracks, nullptr);

    
}

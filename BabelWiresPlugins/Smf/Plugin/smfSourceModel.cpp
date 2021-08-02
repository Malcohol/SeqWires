/**
 * Representation of a Standard MIDI File as a tree of Features.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include "BabelWiresPlugins/Smf/Plugin/smfSourceModel.hpp"

#include "BabelWires/Features/featureMixins.hpp"
#include "BabelWiresPlugins/Smf/Plugin/smfFormat.hpp"
#include "SeqWiresLib/Features/trackFeature.hpp"

#include "BabelWires/Features/Path/fieldName.hpp"

namespace {
    static const babelwires::FieldIdentifiersSource s_trackNames = {
        {"ch0", "Ch. 0", "18cb72e5-6891-428c-beae-aa17db453852"},
        {"ch1", "Ch. 1", "f4bc5369-5d7e-4e18-83ec-44560309edb7"},
        {"ch2", "Ch. 2", "8e58bcc7-af0b-40d0-8a13-ca2c35db8c5e"},
        {"ch3", "Ch. 3", "cddd1035-ca20-4094-ae29-7947d9e71b4f"},
        {"ch4", "Ch. 4", "f4d6c484-028f-4d17-bcb0-a2ddf21904d9"},
        {"ch5", "Ch. 5", "9ba09b7c-2301-4846-9ac0-ccd3708abb7b"},
        {"ch6", "Ch. 6", "2382cc8d-7920-40e8-8813-35b75f40befb"},
        {"ch7", "Ch. 7", "18d1584d-67e4-4f50-ad5a-9d887842414f"},
        {"ch8", "Ch. 8", "6925461b-02b3-448c-a7a9-4cd7f1d5c444"},
        {"ch9", "Ch. 9", "95cfcb8f-af02-4d24-95b6-5b4c19cb3c1b"},
        {"ch10", "Ch. 10", "a445a439-2335-445e-b1f9-ae0a6c23cbd7"},
        {"ch11", "Ch. 11", "b1b96d5b-4c13-4c5d-b876-a7752cdf4694"},
        {"ch12", "Ch. 12", "0df2d856-80d7-47ee-88ba-508bdffec70c"},
        {"ch13", "Ch. 13", "3b9e4821-d6eb-45da-9118-419e190dc763"},
        {"ch14", "Ch. 14", "2742e897-3182-48bf-9558-21e6184cddec"},
        {"ch15", "Ch. 15", "46fb6f3a-15bf-4c3a-972a-78e7ba2ca5b9"}};

    static const babelwires::FieldIdentifiersSource s_extraTrackNames = {
        {"ex0", "Extra Ch. 0", "1ae79cf3-41c3-4311-ae93-4cd78f7e1273"},
        {"ex1", "Extra Ch. 1", "3d02ebfa-cf59-4885-8522-d37ce08c6afb"},
        {"ex2", "Extra Ch. 2", "2afadcc4-8b05-425c-b0ff-60c2d126c02f"},
        {"ex3", "Extra Ch. 3", "bcfede88-37c9-4e29-bc4e-7b6a79d8035c"},
        {"ex4", "Extra Ch. 4", "9ba284d2-d954-409c-9f56-d39640737bed"},
        {"ex5", "Extra Ch. 5", "e345aab8-f2da-41bc-a905-8d4bf13ffbb8"},
        {"ex6", "Extra Ch. 6", "da699351-dffe-4bb0-9835-c2719b1d66e4"},
        {"ex7", "Extra Ch. 7", "c0af7db3-072b-4941-80f2-de65b55434a2"},
        {"ex8", "Extra Ch. 8", "a05f1117-b958-4711-aa6f-4491885ef8c9"},
        {"ex9", "Extra Ch. 9", "cbd0cf0e-226f-4897-83e8-8a3381d1b8a2"},
        {"ex10", "Extra Ch. 10", "f897fa6e-c835-4be4-9f91-b1052416260d"},
        {"ex11", "Extra Ch. 11", "53296f21-7197-4b54-b1b7-a09765e8ae9a"},
        {"ex12", "Extra Ch. 12", "73cb5d58-3cdb-49b1-bf55-500196633e11"},
        {"ex13", "Extra Ch. 13", "3f969eea-461f-4af2-9fa4-fe24df5ebd16"},
        {"ex14", "Extra Ch. 14", "4582d210-f0cf-4100-b554-a1f948341494"},
        {"ex15", "Extra Ch. 15", "6a3d1cde-dee0-451e-b23a-8f23d4a50c33"}};
} // namespace

seqwires::TrackFeature* smf::source::RecordChannelGroup::addTrack(int c) {
    assert((0 <= c) && "Negative channel number");
    assert((c <= 15) && "Channel number out of range");
    assert((tryGetChildFromStep(babelwires::PathStep(std::get<0>(s_trackNames[c]))) == nullptr) &&
           "A track with that channel number is already present");
    // TODO Assert that the fields are in channel order.
    return addField(std::make_unique<seqwires::TrackFeature>(), FIELD_NAME_VECTOR(s_trackNames)[c]);
}

void smf::source::ExtensibleChannelGroup::setPrivilegedTrack(int c) {
    assert((0 <= c) && "Negative channel number");
    assert((c <= 15) && "Channel number out of range");
    assert((m_channelNum == nullptr) && "The first channel was already set");
    m_channelNum = addField(std::make_unique<babelwires::HasStaticRange<babelwires::IntFeature, 0, 15>>(),
                            FIELD_NAME("ChanNo", "channel", "011e3ef1-4c06-4e40-bba4-b242dc8a3d3a"));
    m_trackFeature = addField(std::make_unique<seqwires::TrackFeature>(),
                                  FIELD_NAME("Track", "track", "b48b1dff-6fa4-4c2f-8f77-bc50f44fb09a"));
    m_channelNum->set(c);
}

seqwires::TrackFeature* smf::source::ExtensibleChannelGroup::addTrack(int c) {
    assert((0 <= c) && "Negative channel number");
    assert((c <= 15) && "Channel number out of range");
    assert(m_channelNum && "setPrivilegedTrack should have been called already");
    if (m_channelNum->get() == c) {
        return m_trackFeature;
    } else {
        assert((tryGetChildFromStep(babelwires::PathStep(std::get<0>(s_extraTrackNames[c]))) == nullptr) &&
               "A track with that channel number is already present");
        // TODO Assert that the fields are in channel order.
        return addField(std::make_unique<seqwires::TrackFeature>(), FIELD_NAME_VECTOR(s_extraTrackNames)[c]);
    }
}

smf::source::SmfFeature::SmfFeature(Format f)
    : babelwires::FileFeature(SmfSourceFormat::getThisIdentifier())
    , m_format(f) {
    assert((f != SMF_UNKNOWN_FORMAT) && "You can only construct a format 0, 1 or 2 MIDI file");
    m_metadata = addField(std::make_unique<MidiMetadata>(),
        FIELD_NAME("Meta", "Metadata", "72bbbcee-2b53-4fb2-bfb8-4f5e495f9166"));
}

smf::source::SmfFeature::Format smf::source::SmfFeature::getFormat() const {
    return m_format;
}

smf::MidiMetadata& smf::source::SmfFeature::getMidiMetadata() {
    return *m_metadata;
}

const smf::MidiMetadata& smf::source::SmfFeature::getMidiMetadata() const {
    return *m_metadata;
}

smf::source::Format0SmfFeature::Format0SmfFeature()
    : SmfFeature(SMF_FORMAT_0) {
    m_channelGroup = addField(std::make_unique<RecordChannelGroup>(),
                              FIELD_NAME("tracks", "tracks", "3fb0f062-4e8e-4b37-a598-edcd63f82971"));
}

int smf::source::Format0SmfFeature::getNumMidiTracks() const {
    return 1;
}

const smf::source::ChannelGroup& smf::source::Format0SmfFeature::getMidiTrack(int i) const {
    assert((i == 0) && "There is only 1 in format 0 smf files.");
    return *m_channelGroup;
}

smf::source::ChannelGroup* smf::source::Format0SmfFeature::getMidiTrack0() {
    return m_channelGroup;
}

smf::source::Format1SmfFeature::Format1SmfFeature()
    : SmfFeature(SMF_FORMAT_1) {
    m_tracks = addField(std::make_unique<TrackArray>(),
                        FIELD_NAME("Tracks", "tracks", "3042e0e6-62a6-4a75-b886-77b873005da8"));
}

int smf::source::Format1SmfFeature::getNumMidiTracks() const {
    return m_tracks->getNumFeatures();
}

const smf::source::ChannelGroup& smf::source::Format1SmfFeature::getMidiTrack(int i) const {
    return dynamic_cast<const ChannelGroup&>(*m_tracks->getFeature(i));
}

smf::source::ChannelGroup* smf::source::Format1SmfFeature::addMidiTrack() {
    return dynamic_cast<ChannelGroup*>(m_tracks->addEntry());
}

std::unique_ptr<babelwires::Feature> smf::source::TrackArray::createNextEntry() const {
    return std::make_unique<ExtensibleChannelGroup>();
}

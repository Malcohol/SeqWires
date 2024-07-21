/**
 * MidiTrackAndChannel pairs a track with its MIDI channel.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <Plugins/Smf/Plugin/midiTrackAndChannel.hpp>

#include <SeqWiresLib/Types/Track/trackType.hpp>

namespace {
    static const babelwires::IdentifiersSource s_extraTrackNames = {
        {"exCh0", "Extra Ch. 0", "1ae79cf3-41c3-4311-ae93-4cd78f7e1273"},
        {"exCh1", "Extra Ch. 1", "3d02ebfa-cf59-4885-8522-d37ce08c6afb"},
        {"exCh2", "Extra Ch. 2", "2afadcc4-8b05-425c-b0ff-60c2d126c02f"},
        {"exCh3", "Extra Ch. 3", "bcfede88-37c9-4e29-bc4e-7b6a79d8035c"},
        {"exCh4", "Extra Ch. 4", "9ba284d2-d954-409c-9f56-d39640737bed"},
        {"exCh5", "Extra Ch. 5", "e345aab8-f2da-41bc-a905-8d4bf13ffbb8"},
        {"exCh6", "Extra Ch. 6", "da699351-dffe-4bb0-9835-c2719b1d66e4"},
        {"exCh7", "Extra Ch. 7", "c0af7db3-072b-4941-80f2-de65b55434a2"},
        {"exCh8", "Extra Ch. 8", "a05f1117-b958-4711-aa6f-4491885ef8c9"},
        {"exCh9", "Extra Ch. 9", "cbd0cf0e-226f-4897-83e8-8a3381d1b8a2"},
        {"exCh10", "Extra Ch. 10", "f897fa6e-c835-4be4-9f91-b1052416260d"},
        {"exCh11", "Extra Ch. 11", "53296f21-7197-4b54-b1b7-a09765e8ae9a"},
        {"exCh12", "Extra Ch. 12", "73cb5d58-3cdb-49b1-bf55-500196633e11"},
        {"exCh13", "Extra Ch. 13", "3f969eea-461f-4af2-9fa4-fe24df5ebd16"},
        {"exCh14", "Extra Ch. 14", "4582d210-f0cf-4100-b554-a1f948341494"},
        {"exCh15", "Extra Ch. 15", "6a3d1cde-dee0-451e-b23a-8f23d4a50c33"}};
} // namespace

babelwires::ShortId smf::MidiTrackAndChannel::getTrackIdFromChannel(unsigned int index) {
    assert(index <= 15);
    return REGISTERED_ID_VECTOR(s_extraTrackNames)[index];
}

smf::MidiTrackAndChannel::MidiTrackAndChannel()
    : babelwires::RecordType(
          {{BW_SHORT_ID("Track", "Track", "a6db15c9-9f29-4fb3-92c4-771746b2b97f"),
            seqwires::DefaultTrackType::getThisIdentifier()},
           {BW_SHORT_ID("Chan", "Channel", "cad592ef-2355-4837-a265-38b49eae7599"), MidiChannel::getThisIdentifier()},
           {getTrackIdFromChannel(0), TypeOfTracks::getThisIdentifier(),
            babelwires::RecordType::Optionality::optionalDefaultInactive},
           {getTrackIdFromChannel(1), TypeOfTracks::getThisIdentifier(),
            babelwires::RecordType::Optionality::optionalDefaultInactive},
           {getTrackIdFromChannel(2), TypeOfTracks::getThisIdentifier(),
            babelwires::RecordType::Optionality::optionalDefaultInactive},
           {getTrackIdFromChannel(3), TypeOfTracks::getThisIdentifier(),
            babelwires::RecordType::Optionality::optionalDefaultInactive},
           {getTrackIdFromChannel(4), TypeOfTracks::getThisIdentifier(),
            babelwires::RecordType::Optionality::optionalDefaultInactive},
           {getTrackIdFromChannel(5), TypeOfTracks::getThisIdentifier(),
            babelwires::RecordType::Optionality::optionalDefaultInactive},
           {getTrackIdFromChannel(6), TypeOfTracks::getThisIdentifier(),
            babelwires::RecordType::Optionality::optionalDefaultInactive},
           {getTrackIdFromChannel(7), TypeOfTracks::getThisIdentifier(),
            babelwires::RecordType::Optionality::optionalDefaultInactive},
           {getTrackIdFromChannel(8), TypeOfTracks::getThisIdentifier(),
            babelwires::RecordType::Optionality::optionalDefaultInactive},
           {getTrackIdFromChannel(9), TypeOfTracks::getThisIdentifier(),
            babelwires::RecordType::Optionality::optionalDefaultInactive},
           {getTrackIdFromChannel(10), TypeOfTracks::getThisIdentifier(),
            babelwires::RecordType::Optionality::optionalDefaultInactive},
           {getTrackIdFromChannel(11), TypeOfTracks::getThisIdentifier(),
            babelwires::RecordType::Optionality::optionalDefaultInactive},
           {getTrackIdFromChannel(12), TypeOfTracks::getThisIdentifier(),
            babelwires::RecordType::Optionality::optionalDefaultInactive},
           {getTrackIdFromChannel(13), TypeOfTracks::getThisIdentifier(),
            babelwires::RecordType::Optionality::optionalDefaultInactive},
           {getTrackIdFromChannel(14), TypeOfTracks::getThisIdentifier(),
            babelwires::RecordType::Optionality::optionalDefaultInactive},
           {getTrackIdFromChannel(15), TypeOfTracks::getThisIdentifier(),
            babelwires::RecordType::Optionality::optionalDefaultInactive}}) {}
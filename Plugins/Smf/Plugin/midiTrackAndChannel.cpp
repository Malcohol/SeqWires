/**
 * MidiTrackAndChannel pairs a track with its MIDI channel.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <Plugins/Smf/Plugin/midiTrackAndChannel.hpp>


#include <SeqWiresLib/Types/Track/trackType.hpp>

smf::MidiTrackAndChannel::MidiTrackAndChannel()
    : babelwires::RecordType({{BW_SHORT_ID("Track", "Track", "2eee2dfd-6492-4633-b69d-393df7990f52"),
                               seqwires::DefaultTrackType::getThisIdentifier()},
                              {BW_SHORT_ID("Chan", "Channel", "0ee16945-bd15-4d8d-8e20-f04c790aaaef"),
                               MidiChannel::getThisIdentifier()}}) {}
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
    : babelwires::RecordType({{BW_SHORT_ID("Track", "Track", "a6db15c9-9f29-4fb3-92c4-771746b2b97f"),
                               seqwires::DefaultTrackType::getThisIdentifier()},
                              {BW_SHORT_ID("Chan", "Channel", "cad592ef-2355-4837-a265-38b49eae7599"),
                               MidiChannel::getThisIdentifier()}}) {}
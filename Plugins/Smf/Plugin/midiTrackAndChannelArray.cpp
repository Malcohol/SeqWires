/**
 * MidiTrackAndChannelArray is an array of MidiTrackAndChannel records.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <Plugins/Smf/Plugin/midiTrackAndChannelArray.hpp>

smf::MidiTrackAndChannelArray::MidiTrackAndChannelArray()
    : ArrayType(MidiTrackAndChannel::getThisType(), 1, 16, 1) {}


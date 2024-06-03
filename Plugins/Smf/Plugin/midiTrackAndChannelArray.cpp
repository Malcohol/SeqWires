/**
 * MidiTrackAndChannelArray is an array of MidiTrackAndChannel records.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <Plugins/Smf/Plugin/midiTrackAndChannelArray.hpp>

#include <Plugins/Smf/Plugin/midiTrackAndChannel.hpp>

smf::MidiTrackAndChannelArray::MidiTrackAndChannelArray()
    : ArrayType(MidiTrackAndChannel::getThisIdentifier(), 1, 16, 1) {}

unsigned int smf::MidiTrackAndChannelArray::getArraySize(const babelwires::ValueFeature& arrayFeature) {
    return arrayFeature.getNumFeatures();
}

const babelwires::ValueFeature& smf::MidiTrackAndChannelArray::getChild(const babelwires::ValueFeature& arrayFeature, unsigned int index) {
    return arrayFeature.getFeature(index)->is<babelwires::ValueFeature>();
}

/**
 * MidiTrackAndChannelArray is an array of MidiTrackAndChannel records.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <Plugins/Smf/Plugin/midiTrackAndChannelArray.hpp>

#include <Plugins/Smf/Plugin/midiTrackAndChannel.hpp>

#include <BabelWiresLib/Features/rootFeature.hpp>

smf::MidiTrackAndChannelArray::MidiTrackAndChannelArray()
    : ArrayType(MidiTrackAndChannel::getThisIdentifier(), 1, 16, 1) {}

unsigned int smf::MidiTrackAndChannelArray::getArraySize(const babelwires::ValueFeature& arrayFeature) {
    return arrayFeature.getNumFeatures();
}

const babelwires::ValueFeature& smf::MidiTrackAndChannelArray::getChild(const babelwires::ValueFeature& arrayFeature, unsigned int index) {
    return arrayFeature.getFeature(index)->is<babelwires::ValueFeature>();
}

void smf::MidiTrackAndChannelArray::setArraySize(babelwires::ValueFeature& arrayFeature, unsigned int newSize) {
    const auto& type = arrayFeature.getType().is<smf::MidiTrackAndChannelArray>();
    const auto& typeSystem = babelwires::RootFeature::getTypeSystemAt(arrayFeature);
    babelwires::ValueHolder value = arrayFeature.getValue();
    value.copyContentsAndGetNonConst();
    type.setSize(typeSystem, value, newSize);
    arrayFeature.setValue(value);
}

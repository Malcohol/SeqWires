/**
 * MidiTrackAndChannelArray is an array of MidiTrackAndChannel records.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <BabelWiresLib/Features/valueFeature.hpp>
#include <BabelWiresLib/TypeSystem/primitiveType.hpp>
#include <BabelWiresLib/Types/Array/arrayType.hpp>
#include <Plugins/Smf/Plugin/midiTrackAndChannel.hpp>

namespace smf {
    // an array of MidiTrackAndChannel records.
    class MidiTrackAndChannelArray : public babelwires::ArrayType {
      public:
        PRIMITIVE_TYPE("MidiTrackArray", "MIDI Track and Channels", "b5462658-be68-4570-b0a5-d2dcb03f4d8e", 1);
        MidiTrackAndChannelArray();
    };
} // namespace smf

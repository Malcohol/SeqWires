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

namespace smf {
    // an array of MidiTrackAndChannel records.
    class MidiTrackAndChannelArray : public babelwires::ArrayType {
      public:
        PRIMITIVE_TYPE("MidiTrackArray", "MIDI Track and Channels", "b5462658-be68-4570-b0a5-d2dcb03f4d8e", 1);
        MidiTrackAndChannelArray();

        // TODO These could be generic array util methods.

        static unsigned int getArraySize(const babelwires::ValueFeature& arrayFeature);
        static const babelwires::ValueFeature& getChild(const babelwires::ValueFeature& arrayFeature,
                                                        unsigned int index);
        static babelwires::ValueFeature& getChild(babelwires::ValueFeature& arrayFeature,
                                                        unsigned int index);
        static void setArraySize(babelwires::ValueFeature& arrayFeature, unsigned int newSize);
        static void setChild(babelwires::ValueFeature& arrayFeature, unsigned int index,
                             babelwires::ValueHolder newValue);
    };
} // namespace smf

/**
 * Representation of a Standard MIDI File as a tree of Features.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <Plugins/Smf/Plugin/midiMetadata.hpp>
#include <Plugins/Smf/Plugin/midiTrackAndChannelArray.hpp>

#include <BabelWiresLib/Types/RecordWithVariants/recordWithVariantsType.hpp>
#include <BabelWiresLib/Instance/instance.hpp>

#include <BabelWiresLib/Features/valueFeature.hpp>

namespace seqwires {
    class Track;
    class TrackFeature;
} // namespace seqwires

namespace babelwires {}

namespace smf {

    class SmfType : public babelwires::RecordWithVariantsType {
      public:
        PRIMITIVE_TYPE("SmfType", "Standard MIDI File", "d4c70fb2-fb67-4e69-82ca-328ec242b0a8", 1);
        SmfType();

        DECLARE_INSTANCE_BEGIN(SmfType)
        DECLARE_INSTANCE_FIELD(Meta, MidiMetadata)
        DECLARE_INSTANCE_FIELD(Tracks, MidiTrackAndChannelArray);
        DECLARE_INSTANCE_END()
    };
}

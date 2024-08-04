/**
 *
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <Plugins/Smf/Plugin/instanceWithChannelOptionals.hpp>
#include <Plugins/Smf/Plugin/smfCommon.hpp>

#include <SeqWiresLib/Types/Track/trackInstance.hpp>
#include <SeqWiresLib/Types/Track/trackType.hpp>

#include <BabelWiresLib/Instance/instance.hpp>
#include <BabelWiresLib/Types/Record/recordType.hpp>

namespace smf {

    class RecordOfMidiTracks : public babelwires::RecordType {
      public:
        PRIMITIVE_TYPE("recordOfTracks", "Tracks", "44244cf8-8a05-4071-91a3-af77514ee03b", 1);

        RecordOfMidiTracks();

        static babelwires::ShortId getTrackIdFromChannel(unsigned int channel);

        /// It's easier to work with a custom instance than the one the standard DSL would produce.
        /// The parent class provides index based access to the 16 optional tracks.
        template <typename VALUE_FEATURE>
        class InstanceImpl : public InstanceWithChannelOptionals<VALUE_FEATURE, RecordOfMidiTracks> {
          public:
            InstanceImpl(VALUE_FEATURE& valueFeature)
                : InstanceWithChannelOptionals<VALUE_FEATURE, RecordOfMidiTracks>(valueFeature) {}
        };
        using Instance = InstanceImpl<babelwires::ValueFeature>;
        using ConstInstance = InstanceImpl<const babelwires::ValueFeature>;
    };
} // namespace smf

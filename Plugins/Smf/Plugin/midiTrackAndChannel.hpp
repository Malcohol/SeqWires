/**
 * MidiTrackAndChannel pairs a track with its MIDI channel.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <BabelWiresLib/TypeSystem/standardFeatureWrappers.hpp>
#include <BabelWiresLib/TypeSystem/primitiveType.hpp>
#include <BabelWiresLib/Types/Record/recordFeatureUtils.hpp>
#include <BabelWiresLib/Types/Record/recordType.hpp>

#include <SeqWiresLib/Types/Track/trackType.hpp>
#include <SeqWiresLib/Types/Track/trackFeatureWrapper.hpp>

#include <Plugins/Smf/Plugin/midiChannel.hpp>

namespace smf {

    class MidiTrackAndChannel : public babelwires::RecordType {
      public:
        PRIMITIVE_TYPE("TrackChannel", "Track and Channel", "5e9b395c-ec13-4bdb-9b2b-b060ba7fb707", 1);
        MidiTrackAndChannel();

        FIELD_INT(Chan);
        FIELD_VALUE(Track, seqwires::Track);

        FEATURE_WRAPPER_BEGIN(MidiTrackAndChannel)
        FEATURE_WRAPPER_FIELD(Chan, MidiChannel)
        FEATURE_WRAPPER_FIELD(Track, seqwires::TrackType);
        FEATURE_WRAPPER_END()
    };
} // namespace smf

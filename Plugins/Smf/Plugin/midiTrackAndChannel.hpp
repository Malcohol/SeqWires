/**
 * MidiTrackAndChannel pairs a track with its MIDI channel.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <SeqWiresLib/Types/Track/track.hpp>

#include <BabelWiresLib/TypeSystem/primitiveType.hpp>
#include <BabelWiresLib/Types/Record/recordFeatureUtils.hpp>
#include <BabelWiresLib/Types/Record/recordType.hpp>

namespace smf {

    class MidiTrackAndChannel : public babelwires::RecordType {
      public:
        PRIMITIVE_TYPE("TrackChannel", "Track and Channel", "5e9b395c-ec13-4bdb-9b2b-b060ba7fb707", 1);
        MidiTrackAndChannel();

        FIELD_INT(Chan);
        FIELD_VALUE(Track, seqwires::Track);
    };
} // namespace smf

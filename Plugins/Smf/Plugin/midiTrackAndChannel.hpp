/**
 * 
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <BabelWiresLib/Types/Record/recordType.hpp>
#include <BabelWiresLib/TypeSystem/primitiveType.hpp>

namespace seqwires {
    class TrackFeature;
}

namespace smf {

    class MidiTrackAndChannel : public babelwires::RecordType {
        public:
            PRIMITIVE_TYPE("Track&Channel", "Track and Channel", "5e9b395c-ec13-4bdb-9b2b-b060ba7fb707", 1);

            MidiTrackAndChannel();
    };
}

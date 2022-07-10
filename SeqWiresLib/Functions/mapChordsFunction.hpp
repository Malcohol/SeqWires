/**
 * Function which applies maps to chord events.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include "SeqWiresLib/Tracks/track.hpp"

namespace babelwires {
    class MapData;
    class TypeSystem;
}

namespace seqwires {
    /// Create tracks of chord events from tracks of note events.
    Track mapChordsFunction(const babelwires::TypeSystem& typeSystem, const Track& sourceTrack, const babelwires::MapData& chordTypeMapData, const babelwires::MapData& pitchClassMapData);
}

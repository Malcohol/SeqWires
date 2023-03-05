/**
 * Function which applies maps to chord events.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <SeqWiresLib/Tracks/track.hpp>

namespace babelwires {
    class MapData;
    class TypeSystem;
}

namespace seqwires {
    babelwires::TypeRef getMapChordFunctionChordTypeRef();
    babelwires::TypeRef getMapChordFunctionPitchClassRef();

    /// Apply maps to chord events in the track.
    /// You can specify a chord that should be active when no chord in the sourceTrack is active
    /// by having blanks in the source map.
    /// Until the type system has cross products, this will be somewhat limited in value.
    Track mapChordsFunction(const babelwires::TypeSystem& typeSystem, const Track& sourceTrack, const babelwires::MapData& chordTypeMapData, const babelwires::MapData& pitchClassMapData);
}

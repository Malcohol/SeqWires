/**
 * Function which maps notes events using a percussion map.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <SeqWiresLib/Types/Track/track.hpp>

namespace babelwires {
    class MapData;
    class TypeSystem;
}

namespace seqwires {
    /// 
    Track mapPercussionFunction(const babelwires::TypeSystem& typeSystem, const Track& sourceTrack, const babelwires::MapData& percussionMapData);
}

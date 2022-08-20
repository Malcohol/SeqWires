/**
 * Quantize track events.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <SeqWiresLib/Tracks/track.hpp>

namespace babelwires {
    class MapData;
    class TypeSystem;
    class Rational;
}

namespace seqwires {
    /// 
    Track quantize(const Track& trackIn, ModelDuration beat);
}

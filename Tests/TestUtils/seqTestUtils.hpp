#pragma once

#include <SeqWiresLib/musicTypes.hpp>

#include <vector>

namespace seqwires {
    class Track;
}

namespace testUtils {
    void testSimpleNotes(const std::vector<seqwires::Pitch>& expectedPitches, const seqwires::Track& track);
}

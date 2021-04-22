#include "SeqwiresLib/Utilities/musicUtilities.hpp"
#include "SeqwiresLib/Tracks/track.hpp"

int seqwires::getMinimumDenominator(const Track& track) {
    int denominator = 1;

    for (const auto& e : track) {
        denominator = babelwires::lcm(denominator, e.getTimeSinceLastEvent().getDenominator());
    }

    return denominator;
}

/**
 * Some utility functions useful for manipulating music sequence data.
 * 
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <SeqWiresLib/Utilities/musicUtilities.hpp>

#include <SeqWiresLib/Tracks/track.hpp>

int seqwires::getMinimumDenominator(const Track& track) {
    int denominator = 1;

    for (const auto& e : track) {
        denominator = babelwires::lcm(denominator, e.getTimeSinceLastEvent().getDenominator());
    }

    return denominator;
}

/**
 * The TrackFeature is a ValueFeature which carries a track of sequence data.
 * 
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include "SeqwiresLib/Features/trackFeature.hpp"

std::string seqwires::TrackFeature::doGetValueType() const {
    return "trk";
}

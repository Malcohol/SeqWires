/**
 * The TrackFeature is a ValueFeature which carries a track of sequence data.
 * 
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <SeqWiresLib/Features/trackFeature.hpp>

seqwires::TrackFeature::TrackFeature()
    : SimpleValueFeature(DefaultTrackType::getThisIdentifier())
{
}

const seqwires::Track& seqwires::TrackFeature::get() const {
    return getValue().is<Track>();
}

void seqwires::TrackFeature::set(Track track) {
    setValue(std::move(track));
}

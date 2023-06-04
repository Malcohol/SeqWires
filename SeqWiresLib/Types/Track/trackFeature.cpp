/**
 * The TrackFeature is a ValueFeature which carries a track of sequence data.
 * 
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <SeqWiresLib/Types/Track/trackFeature.hpp>

seqwires::TrackFeature::TrackFeature()
    : SimpleValueFeature(DefaultTrackType::getThisIdentifier())
{
}

const seqwires::Track& seqwires::TrackFeature::get() const {
    return getValueHolder()->is<Track>();
}

void seqwires::TrackFeature::set(Track track) {
    setValueHolder(std::move(track));
}

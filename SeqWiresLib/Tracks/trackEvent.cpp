/**
 * A TrackEvent is the base class of the events that occur in tracks.
 * 
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <SeqWiresLib/Tracks/trackEvent.hpp>

#include <Common/Hash/hash.hpp>

const char* seqwires::TrackEvent::GroupingInfo::s_genericCategory = "Generic";

bool seqwires::TrackEvent::operator!=(const TrackEvent& other) const {
    return !(*this == other);
}

seqwires::TrackEvent::GroupingInfo seqwires::TrackEvent::getGroupingInfo() const {
    return GroupingInfo();
}

void seqwires::TrackEvent::transpose(int pitchOffset) {
}

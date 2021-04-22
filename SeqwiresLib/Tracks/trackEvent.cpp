#include "SeqwiresLib/Tracks/trackEvent.hpp"

#include "Common/Utilities/hash.hpp"

const char* seqwires::TrackEvent::GroupingInfo::s_genericCategory = "Generic";

bool seqwires::TrackEvent::operator!=(const TrackEvent& other) const {
    return !(*this == other);
}

seqwires::TrackEvent::GroupingInfo seqwires::TrackEvent::getGroupingInfo() const {
    return GroupingInfo();
}

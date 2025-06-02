/**
 * The TrackTraverser aids in the traversal of a track in time chunks which may lie between events.
 * 
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
template <typename TRACK_ITERATOR>
template <typename SPAN>
seqwires::TrackTraverser<TRACK_ITERATOR>::TrackTraverser(const Track& track, const SPAN& span)
    : m_track(track)
    , m_iterator(span.begin())
    , m_endIterator(span.end())
    , m_timeToNextEvent(-1) // For debugging.
{
    if (m_iterator != m_endIterator) {
        m_timeToNextEvent = m_iterator->getTimeSinceLastEvent();
    }
}

template <typename TRACK_ITERATOR>
void seqwires::TrackTraverser<TRACK_ITERATOR>::leastUpperBoundDuration(ModelDuration& duration) const {
    const ModelDuration d = m_track.getDuration();
    if (d > duration) {
        duration = d;
    }
}

template <typename TRACK_ITERATOR>
void seqwires::TrackTraverser<TRACK_ITERATOR>::greatestLowerBoundNextEvent(ModelDuration& duration) const {
    if (m_iterator != m_endIterator) {
        if (duration > m_timeToNextEvent) {
            duration = m_timeToNextEvent;
        }
    }
}

template <typename TRACK_ITERATOR>
void seqwires::TrackTraverser<TRACK_ITERATOR>::advance(
    ModelDuration time, const std::function<void(const typename TRACK_ITERATOR::value_type&)>& eventVisitor) {
    if (m_iterator != m_endIterator) {
        assert((time <= m_timeToNextEvent) && "You cannot advance beyond the next event");
        m_timeToNextEvent -= time;
        if (m_timeToNextEvent == 0) {
            do {
                eventVisitor(*m_iterator);
                ++m_iterator;
            } while ((m_iterator != m_endIterator) && ((*m_iterator).getTimeSinceLastEvent() == 0));
            if (m_iterator != m_endIterator) {
                m_timeToNextEvent = m_iterator->getTimeSinceLastEvent();
            }
        }
    }
}

template <typename TRACK_ITERATOR>
void seqwires::TrackTraverser<TRACK_ITERATOR>::advance(ModelDuration time, EventsAtTime& eventsAtTimeOut,
                                                       bool clearEventsAtTimeOutFirst) {
    if (clearEventsAtTimeOutFirst) {
        eventsAtTimeOut.clear();
    }
    advance(time, [&eventsAtTimeOut](const typename TRACK_ITERATOR::value_type& event) {
        eventsAtTimeOut.emplace_back(event);
    });
}

template <typename TRACK_ITERATOR>
bool seqwires::TrackTraverser<TRACK_ITERATOR>::isNextEventAt(ModelDuration time) const {
    if (m_iterator == m_endIterator) {
        return false;
    }
    assert((time <= m_timeToNextEvent) && "You should not query beyond the next event");
    return (m_timeToNextEvent == time);
}

template <typename TRACK_ITERATOR> bool seqwires::TrackTraverser<TRACK_ITERATOR>::hasMoreEvents() const {
    return (m_iterator != m_endIterator);
}

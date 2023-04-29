/**
 * Tracks describe a sequence of events, typically notes and chords.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <SeqWiresLib/Tracks/trackType.hpp>

#include <SeqWiresLib/Tracks/track.hpp>

seqwires::TrackType::TrackType(ModelDuration defaultDuration)
    : m_defaultDuration(defaultDuration) {}

babelwires::NewValueHolder seqwires::TrackType::createValue() const {
    auto newValueHolder = babelwires::ValueHolder::makeValue<Track>();
    newValueHolder.m_nonConstReference.is<Track>().setDuration(m_defaultDuration);
    return newValueHolder;
}

bool seqwires::TrackType::isValidValue(const babelwires::Value& v) const {
    return v.as<Track>();
}

std::string seqwires::TrackType::getKind() const {
    // TODO: Decide on a standard scheme for non-serialized types.
    return "Track";
}
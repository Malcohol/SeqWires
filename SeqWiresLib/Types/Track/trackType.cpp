/**
 * Tracks describe a sequence of events, typically notes and chords.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <SeqWiresLib/Types/Track/trackType.hpp>

#include <SeqWiresLib/Types/Track/track.hpp>

seqwires::TrackType::TrackType(ModelDuration defaultDuration)
    : m_defaultDuration(defaultDuration) {}

babelwires::NewValueHolder seqwires::TrackType::createValue(const babelwires::TypeSystem& typeSystem) const {
    return babelwires::ValueHolder::makeValue<Track>(m_defaultDuration);
}

bool seqwires::TrackType::isValidValue(const babelwires::TypeSystem& typeSystem, const babelwires::Value& v) const {
    return v.as<Track>();
}

std::string seqwires::TrackType::getKind() const {
    // TODO: Decide on a standard scheme for non-serialized types.
    return "Track";
}

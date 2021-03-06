/**
 * Some common type definitions for musical data.
 * 
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <SeqWiresLib/musicTypes.hpp>

#include <Common/exceptions.hpp>

#include <sstream>
#include <cctype>
#include <charconv>
#include <cassert>

std::string seqwires::pitchToString(Pitch p) {
    const int pitchClass = p % 12;
    const int octave = (p / 12) - 1;

    std::ostringstream stringOut;
    stringOut << PitchClass::valueToString(static_cast<PitchClass::Value>(pitchClass)) << octave;

    return stringOut.str();
}

seqwires::Pitch seqwires::stringToPitch(std::string_view s) {
    const char* it = s.data();
    const char *const end = s.data() + s.length();
    while ((it != end) && std::isspace(*it)) {
        ++it;
    }

    if (it == end) {
        throw babelwires::ParseException() << "No contents when parsing pitch value";
    }

    int p = 0;

    switch (*it) {
        case 'a':
        case 'A':
            p = 9;
            break;
        case 'b':
        case 'B':
            p = 11;
            break;
        case 'c':
        case 'C':
            p = 0;
            break;
        case 'd':
        case 'D':
            p = 2;
            break;
        case 'e':
        case 'E':
            p = 4;
            break;
        case 'f':
        case 'F':
            p = 5;
            break;
        case 'g':
        case 'G':
            p = 7;
            break;
        default:
            throw babelwires::ParseException() << "Unexpected character when parsing pitch value";
    }
    ++it;
    if ((it != end) && (*it == '#')) {
        ++p;
        ++it;
    }
    if ((it == end) || std::isspace(*it)) {
        throw babelwires::ParseException() << "No octave value when parsing pitch value";
    }
    int octave = 0;
    const auto result = std::from_chars(it, end, octave);
    if (result.ec != std::errc()) {
        throw babelwires::ParseException() << "Could not parse octave when parsing pitch value";
    }
    if ((octave < -1) || (octave > 9))  {
        throw babelwires::ParseException() << "Octave out of range when parsing pitch value";
    }
    it = result.ptr;
    while (it != end) {
        if (!std::isspace(*it)) {
            throw babelwires::ParseException() << "Trailing characters when parsing pitch value";
        }
        ++it;
    }
    p += (octave + 1) * 12;
    assert(p >= 0);
    if (p > 127) {
        // G#9, A9, A#9 and B9 are unrepresentable
        throw babelwires::ParseException() << "Note out of range when parsing pitch value";
    }
    return static_cast<Pitch>(p);
}

std::string seqwires::durationToString(ModelDuration d) {
    std::ostringstream stringOut;
    stringOut << d;
    return stringOut.str();
}

seqwires::PitchClass::Value seqwires::pitchToPitchClass(seqwires::Pitch p) {
    return static_cast<seqwires::PitchClass::Value>(p % 12);
}
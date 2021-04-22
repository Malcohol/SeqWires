#include <SeqwiresLib/musicTypes.hpp>
#include <sstream>

namespace {

    const char* s_pitchClassNames[seqwires::NUM_PITCH_CLASSES] = {"C",  "C#", "D",  "D#", "E",  "F",
                                                                  "F#", "G",  "G#", "A",  "A#", "B"};

    const char* s_chordTypeNames[seqwires::ChordType::NUM_CHORD_TYPES] = {
        "M", "m", "7", "m7", "M7", "6", "m6", "+5", "m7-5", "DIM", "SUS4", "M+9", "m+9"};
} // namespace

std::string seqwires::pitchToString(Pitch p) {
    const int pitchClass = p % 12;
    const int octave = p / 12;

    std::ostringstream stringOut;
    stringOut << s_pitchClassNames[pitchClass] << octave;

    return stringOut.str();
}

std::string seqwires::durationToString(ModelDuration d) {
    std::ostringstream stringOut;
    stringOut << d;
    return stringOut.str();
}

std::string seqwires::chordTypeToString(ChordType t) {
    return s_chordTypeNames[t];
}

std::string seqwires::pitchClassToString(PitchClass p) {
    return s_pitchClassNames[p];
}

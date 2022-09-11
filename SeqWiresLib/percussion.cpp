/**
 * Defines enums for standard percussion sets.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <SeqWiresLib/percussion.hpp>

#include <Common/Identifiers/identifierRegistry.hpp>

ENUM_DEFINE_ENUM_VALUE_SOURCE(GM2_STANDARD_PERCUSSION_VALUES);

babelwires::LongIdentifier seqwires::GM2Percussion::getThisIdentifier() {
    return REGISTERED_LONGID("GM2Percussion", "General MIDI 2 Percussion", "9fc0c107-f76c-432a-af58-c794f01df455");
}

seqwires::GM2Percussion::GM2Percussion()
    : Enum(getThisIdentifier(), 1, ENUM_IDENTIFIER_VECTOR(GM2_STANDARD_PERCUSSION_VALUES), 0) {}

seqwires::Pitch seqwires::GM2Percussion::getPitchFromValue(Value value) {
    const unsigned int index = static_cast<unsigned int>(value);
    return index + 27;
}

bool seqwires::GM2Percussion::tryGetValueFromPitch(Pitch pitch, Value& valueOut) const {
    if ((pitch < 27) || (pitch > 87)) {
        return false;
    }
    valueOut = static_cast<Value>(pitch - 27);
    return true;
}

seqwires::GMPercussion::GMPercussion()
    : babelwires::Enum(
          getThisIdentifier(), 1,
          babelwires::Enum::EnumValues{"AcBass", "Bass1",  "SStick", "AcSnr",  "Clap",   "ElSnr",  "LFlTom", "ClHHat",
                                       "HFlTom", "PdHHat", "LwTom",  "OpHHat", "LMTom",  "HMTom",  "Crash1", "HTom",
                                       "Ride1",  "ChnCym", "RideBl", "Tamb",   "SplCym", "Cowbl",  "Crash2", "VibraS",
                                       "Ride2",  "HBongo", "LBongo", "MHCnga", "OHCnga", "LConga", "HTimbl", "LTimbl",
                                       "HAgogo", "LAgogo", "Cabasa", "Maracs", "SWhisl", "LWhisl", "SGuiro", "LGuiro",
                                       "Claves", "HWoodB", "LWoodB", "MCuica", "OCuica", "MTrian", "OTrian"},
          1, seqwires::GM2Percussion::getThisIdentifier()) {}

babelwires::LongIdentifier seqwires::GMPercussion::getThisIdentifier() {
    return REGISTERED_LONGID("GMPercussion", "General MIDI Percussion", "9f6dec59-4d75-4a4b-9cba-e7ff76390c5f");
}

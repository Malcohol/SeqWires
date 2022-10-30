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

seqwires::PercussionKit::PercussionKit(babelwires::LongIdentifier identifier, babelwires::VersionNumber version,
                                       EnumValues values, unsigned int indexOfDefaultValue,
                                       std::optional<babelwires::LongIdentifier> parentTypeId)
    : Enum(identifier, version, values, indexOfDefaultValue, parentTypeId) {}

babelwires::LongIdentifier seqwires::GM2StandardPercussionKit::getThisIdentifier() {
    return REGISTERED_LONGID("GM2Percussion", "General MIDI 2 Percussion", "9fc0c107-f76c-432a-af58-c794f01df455");
}

seqwires::GM2StandardPercussionKit::GM2StandardPercussionKit()
    : PercussionKit(getThisIdentifier(), 1, ENUM_IDENTIFIER_VECTOR(GM2_STANDARD_PERCUSSION_VALUES), 0) {}

seqwires::Pitch seqwires::GM2StandardPercussionKit::getPitchFromValue(Value value) {
    const unsigned int index = static_cast<unsigned int>(value);
    return index + 27;
}

bool seqwires::GM2StandardPercussionKit::tryGetValueFromPitch(Pitch pitch, Value& valueOut) {
    if ((pitch < 27) || (pitch > 87)) {
        return false;
    }
    valueOut = static_cast<Value>(pitch - 27);
    return true;
}

seqwires::GMPercussionKit::GMPercussionKit()
    : PercussionKit(getThisIdentifier(), 1,
                    babelwires::Enum::EnumValues{"AcBass", "Bass1",  "SStick", "AcSnr",  "Clap",   "ElSnr",  "LFlTom",
                                                 "ClHHat", "HFlTom", "PdHHat", "LwTom",  "OpHHat", "LMTom",  "HMTom",
                                                 "Crash1", "HTom",   "Ride1",  "ChnCym", "RideBl", "Tamb",   "SplCym",
                                                 "Cowbll", "Crash2", "VibraS", "Ride2",  "HBongo", "LBongo", "MHCnga",
                                                 "OHCnga", "LConga", "HTimbl", "LTimbl", "HAgogo", "LAgogo", "Cabasa",
                                                 "Maracs", "SWhisl", "LWhisl", "SGuiro", "LGuiro", "Claves", "HWoodB",
                                                 "LWoodB", "MCuica", "OCuica", "MTrian", "OTrian"},
                    1, seqwires::GM2StandardPercussionKit::getThisIdentifier()) {}

babelwires::LongIdentifier seqwires::GMPercussionKit::getThisIdentifier() {
    return REGISTERED_LONGID("GMPercussion", "General MIDI Percussion", "9f6dec59-4d75-4a4b-9cba-e7ff76390c5f");
}

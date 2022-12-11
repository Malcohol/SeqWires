/**
 * Defines enums for standard percussion sets.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <SeqWiresLib/percussion.hpp>

#include <Common/Identifiers/identifierRegistry.hpp>

ENUM_DEFINE_ENUM_VALUE_SOURCE(BUILT_IN_PERCUSSION_INSTRUMENTS);

seqwires::BuiltInPercussionInstruments::BuiltInPercussionInstruments()
    : Enum(getThisIdentifier(), 1, ENUM_IDENTIFIER_VECTOR(BUILT_IN_PERCUSSION_INSTRUMENTS), 0) {}

babelwires::LongIdentifier seqwires::BuiltInPercussionInstruments::getThisIdentifier() {
    return REGISTERED_LONGID("BuiltInPercussion", "Built In Percussion", "c67f1a9e-653d-42b5-bf73-1fdc2f8a9b1a");
}

babelwires::Enum::EnumValues
seqwires::BuiltInPercussionInstruments::convertToIdentifiers(const std::vector<Value>& values) const {
    babelwires::Enum::EnumValues identifiers;
    std::transform(
        values.cbegin(), values.cend(), std::back_inserter(identifiers),
        [this](seqwires::BuiltInPercussionInstruments::Value value) { return getIdentifierFromValue(value); });
    return identifiers;
}

seqwires::PercussionKit::PercussionKit(babelwires::LongIdentifier identifier, babelwires::VersionNumber version,
                                       EnumValues values, unsigned int indexOfDefaultValue)
    : Enum(identifier, version, values, indexOfDefaultValue) {}

seqwires::GMPercussionKit::GMPercussionKit(const BuiltInPercussionInstruments& builtInInstruments)
    : PercussionKit(getThisIdentifier(), 1,
                    builtInInstruments.convertToIdentifiers(
                        {BuiltInPercussionInstruments::Value::AcBass, BuiltInPercussionInstruments::Value::Bass1,
                         BuiltInPercussionInstruments::Value::SStick, BuiltInPercussionInstruments::Value::AcSnr,
                         BuiltInPercussionInstruments::Value::Clap,   BuiltInPercussionInstruments::Value::ElSnr,
                         BuiltInPercussionInstruments::Value::LFlTom, BuiltInPercussionInstruments::Value::ClHHat,
                         BuiltInPercussionInstruments::Value::HFlTom, BuiltInPercussionInstruments::Value::PdHHat,
                         BuiltInPercussionInstruments::Value::LwTom,  BuiltInPercussionInstruments::Value::OpHHat,
                         BuiltInPercussionInstruments::Value::LMTom,  BuiltInPercussionInstruments::Value::HMTom,
                         BuiltInPercussionInstruments::Value::Crash1, BuiltInPercussionInstruments::Value::HTom,
                         BuiltInPercussionInstruments::Value::Ride1,  BuiltInPercussionInstruments::Value::ChnCym,
                         BuiltInPercussionInstruments::Value::RideBl, BuiltInPercussionInstruments::Value::Tamb,
                         BuiltInPercussionInstruments::Value::SplCym, BuiltInPercussionInstruments::Value::Cowbll,
                         BuiltInPercussionInstruments::Value::Crash2, BuiltInPercussionInstruments::Value::VibraS,
                         BuiltInPercussionInstruments::Value::Ride2,  BuiltInPercussionInstruments::Value::HBongo,
                         BuiltInPercussionInstruments::Value::LBongo, BuiltInPercussionInstruments::Value::MHCnga,
                         BuiltInPercussionInstruments::Value::OHCnga, BuiltInPercussionInstruments::Value::LConga,
                         BuiltInPercussionInstruments::Value::HTimbl, BuiltInPercussionInstruments::Value::LTimbl,
                         BuiltInPercussionInstruments::Value::HAgogo, BuiltInPercussionInstruments::Value::LAgogo,
                         BuiltInPercussionInstruments::Value::Cabasa, BuiltInPercussionInstruments::Value::Maracs,
                         BuiltInPercussionInstruments::Value::SWhisl, BuiltInPercussionInstruments::Value::LWhisl,
                         BuiltInPercussionInstruments::Value::SGuiro, BuiltInPercussionInstruments::Value::LGuiro,
                         BuiltInPercussionInstruments::Value::Claves, BuiltInPercussionInstruments::Value::HWoodB,
                         BuiltInPercussionInstruments::Value::LWoodB, BuiltInPercussionInstruments::Value::MCuica,
                         BuiltInPercussionInstruments::Value::OCuica, BuiltInPercussionInstruments::Value::MTrian,
                         BuiltInPercussionInstruments::Value::OTrian}),
                    0) {}

babelwires::LongIdentifier seqwires::GMPercussionKit::getThisIdentifier() {
    return REGISTERED_LONGID("GMPercussion", "General MIDI Percussion", "9f6dec59-4d75-4a4b-9cba-e7ff76390c5f");
}

std::optional<seqwires::Pitch>
seqwires::GMPercussionKit::tryGetPitchFromInstrument(babelwires::Identifier identifier) const {
    const int index = tryGetIndexFromIdentifier(identifier);
    if (index != -1) {
        return index + 35;
    }
    return {};
}

std::optional<babelwires::Identifier> seqwires::GMPercussionKit::tryGetInstrumentFromPitch(Pitch pitch) const {
    if ((pitch >= 35) && (pitch <= 81)) {
        return getEnumValues()[pitch - 35];
    }
    return {};
}

seqwires::GM2StandardPercussionKit::GM2StandardPercussionKit(const BuiltInPercussionInstruments& builtInInstruments)
    : PercussionKit(getThisIdentifier(), 1,
                    builtInInstruments.convertToIdentifiers({seqwires::BuiltInPercussionInstruments::Value::HighQ,
                                                             seqwires::BuiltInPercussionInstruments::Value::Slap,
                                                             seqwires::BuiltInPercussionInstruments::Value::ScrPsh,
                                                             seqwires::BuiltInPercussionInstruments::Value::ScrPll,
                                                             seqwires::BuiltInPercussionInstruments::Value::Stcks,
                                                             seqwires::BuiltInPercussionInstruments::Value::SqClck,
                                                             seqwires::BuiltInPercussionInstruments::Value::MtrClk,
                                                             seqwires::BuiltInPercussionInstruments::Value::MtrBll,
                                                             seqwires::BuiltInPercussionInstruments::Value::AcBass,
                                                             seqwires::BuiltInPercussionInstruments::Value::Bass1,
                                                             seqwires::BuiltInPercussionInstruments::Value::SStick,
                                                             seqwires::BuiltInPercussionInstruments::Value::AcSnr,
                                                             seqwires::BuiltInPercussionInstruments::Value::Clap,
                                                             seqwires::BuiltInPercussionInstruments::Value::ElSnr,
                                                             seqwires::BuiltInPercussionInstruments::Value::LFlTom,
                                                             seqwires::BuiltInPercussionInstruments::Value::ClHHat,
                                                             seqwires::BuiltInPercussionInstruments::Value::HFlTom,
                                                             seqwires::BuiltInPercussionInstruments::Value::PdHHat,
                                                             seqwires::BuiltInPercussionInstruments::Value::LwTom,
                                                             seqwires::BuiltInPercussionInstruments::Value::OpHHat,
                                                             seqwires::BuiltInPercussionInstruments::Value::LMTom,
                                                             seqwires::BuiltInPercussionInstruments::Value::HMTom,
                                                             seqwires::BuiltInPercussionInstruments::Value::Crash1,
                                                             seqwires::BuiltInPercussionInstruments::Value::HTom,
                                                             seqwires::BuiltInPercussionInstruments::Value::Ride1,
                                                             seqwires::BuiltInPercussionInstruments::Value::ChnCym,
                                                             seqwires::BuiltInPercussionInstruments::Value::RideBl,
                                                             seqwires::BuiltInPercussionInstruments::Value::Tamb,
                                                             seqwires::BuiltInPercussionInstruments::Value::SplCym,
                                                             seqwires::BuiltInPercussionInstruments::Value::Cowbll,
                                                             seqwires::BuiltInPercussionInstruments::Value::Crash2,
                                                             seqwires::BuiltInPercussionInstruments::Value::VibraS,
                                                             seqwires::BuiltInPercussionInstruments::Value::Ride2,
                                                             seqwires::BuiltInPercussionInstruments::Value::HBongo,
                                                             seqwires::BuiltInPercussionInstruments::Value::LBongo,
                                                             seqwires::BuiltInPercussionInstruments::Value::MHCnga,
                                                             seqwires::BuiltInPercussionInstruments::Value::OHCnga,
                                                             seqwires::BuiltInPercussionInstruments::Value::LConga,
                                                             seqwires::BuiltInPercussionInstruments::Value::HTimbl,
                                                             seqwires::BuiltInPercussionInstruments::Value::LTimbl,
                                                             seqwires::BuiltInPercussionInstruments::Value::HAgogo,
                                                             seqwires::BuiltInPercussionInstruments::Value::LAgogo,
                                                             seqwires::BuiltInPercussionInstruments::Value::Cabasa,
                                                             seqwires::BuiltInPercussionInstruments::Value::Maracs,
                                                             seqwires::BuiltInPercussionInstruments::Value::SWhisl,
                                                             seqwires::BuiltInPercussionInstruments::Value::LWhisl,
                                                             seqwires::BuiltInPercussionInstruments::Value::SGuiro,
                                                             seqwires::BuiltInPercussionInstruments::Value::LGuiro,
                                                             seqwires::BuiltInPercussionInstruments::Value::Claves,
                                                             seqwires::BuiltInPercussionInstruments::Value::HWoodB,
                                                             seqwires::BuiltInPercussionInstruments::Value::LWoodB,
                                                             seqwires::BuiltInPercussionInstruments::Value::MCuica,
                                                             seqwires::BuiltInPercussionInstruments::Value::OCuica,
                                                             seqwires::BuiltInPercussionInstruments::Value::MTrian,
                                                             seqwires::BuiltInPercussionInstruments::Value::OTrian,
                                                             seqwires::BuiltInPercussionInstruments::Value::Shaker,
                                                             seqwires::BuiltInPercussionInstruments::Value::JngBll,
                                                             seqwires::BuiltInPercussionInstruments::Value::BlTree,
                                                             seqwires::BuiltInPercussionInstruments::Value::Cstnts,
                                                             seqwires::BuiltInPercussionInstruments::Value::MSurdo,
                                                             seqwires::BuiltInPercussionInstruments::Value::OSurdo}),
                    0) {}

babelwires::LongIdentifier seqwires::GM2StandardPercussionKit::getThisIdentifier() {
    return REGISTERED_LONGID("GM2Percussion", "General MIDI 2 Percussion", "9fc0c107-f76c-432a-af58-c794f01df455");
}

std::optional<seqwires::Pitch>
seqwires::GM2StandardPercussionKit::tryGetPitchFromInstrument(babelwires::Identifier identifier) const {
    const int index = tryGetIndexFromIdentifier(identifier);
    if (index != -1) {
        return index + 27;
    }
    return {};
}

std::optional<babelwires::Identifier> seqwires::GM2StandardPercussionKit::tryGetInstrumentFromPitch(Pitch pitch) const {
    if ((pitch >= 27) && (pitch <= 87)) {
        return getEnumValues()[pitch - 35];
    }
    return {};
}
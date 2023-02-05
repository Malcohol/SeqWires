/**
 * A PercussionSet corresponding to the original General MIDI Percussion Set.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <Plugins/Smf/Plugin/Percussion/gmPercussionSet.hpp>

smf::GMPercussionSet::GMPercussionSet(const seqwires::BuiltInPercussionInstruments& builtInInstruments)
    : PercussionSetWithPitchMap(getThisIdentifier(), 1,
                    InstrumentBlock{{seqwires::BuiltInPercussionInstruments::Value::AcBass,
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
                                     seqwires::BuiltInPercussionInstruments::Value::OTrian},
                                    35,
                                    &builtInInstruments},
                    35) {}

babelwires::LongIdentifier smf::GMPercussionSet::getThisIdentifier() {
    return REGISTERED_LONGID("GMPerc", "General MIDI Percussion", "7571c9ca-1c7f-4547-9218-391a339bae7d");
}

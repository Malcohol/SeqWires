/**
 * A PercussionSet corresponding to the instruments of GS Standard Percussion Set.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <Plugins/Smf/Plugin/Percussion/gs808909PercussionSet.hpp>

smf::Gs808909PercussionSet::Gs808909PercussionSet(const seqwires::BuiltInPercussionInstruments& builtInInstruments)
    : PercussionSetWithPitchMap(getThisIdentifier(), 1,
                    InstrumentBlock{{seqwires::BuiltInPercussionInstruments::Value::SnrRll,
                                     seqwires::BuiltInPercussionInstruments::Value::FngSnp,
                                     seqwires::BuiltInPercussionInstruments::Value::HighQ,
                                     seqwires::BuiltInPercussionInstruments::Value::Slap,
                                     seqwires::BuiltInPercussionInstruments::Value::ScrPsh,
                                     seqwires::BuiltInPercussionInstruments::Value::ScrPll,
                                     seqwires::BuiltInPercussionInstruments::Value::Stcks,
                                     seqwires::BuiltInPercussionInstruments::Value::SqClck,
                                     seqwires::BuiltInPercussionInstruments::Value::MtrClk,
                                     seqwires::BuiltInPercussionInstruments::Value::MtrBll,
                                     seqwires::BuiltInPercussionInstruments::Value::Bs909,
                                     seqwires::BuiltInPercussionInstruments::Value::Bs808,
                                     seqwires::BuiltInPercussionInstruments::Value::RimSht, // 808 Rim Shot
                                     seqwires::BuiltInPercussionInstruments::Value::ElSnr,  // 808 Snare 1
                                     seqwires::BuiltInPercussionInstruments::Value::Clap,
                                     seqwires::BuiltInPercussionInstruments::Value::ElSnr2, // 909 Snare 1
                                     seqwires::BuiltInPercussionInstruments::Value::ELwTm2, // 808 Low Tom 2
                                     seqwires::BuiltInPercussionInstruments::Value::ClHHat, // 808 CHH
                                     seqwires::BuiltInPercussionInstruments::Value::ELwTm1, // 808 Low Tom 1
                                     seqwires::BuiltInPercussionInstruments::Value::PdHHat, // 808 CHH
                                     seqwires::BuiltInPercussionInstruments::Value::EMdTm2, // 808 Mid Tom 2
                                     seqwires::BuiltInPercussionInstruments::Value::OpHHat, // 808 OHH
                                     seqwires::BuiltInPercussionInstruments::Value::EMdTm1, // 808 Low Tom 1
                                     seqwires::BuiltInPercussionInstruments::Value::EHiTm2, // 808 Hi Tom 2
                                     seqwires::BuiltInPercussionInstruments::Value::Crash1, // 808 Cymbal
                                     seqwires::BuiltInPercussionInstruments::Value::EHiTm1, // 808 Hi Tom 1
                                     seqwires::BuiltInPercussionInstruments::Value::Ride1,
                                     seqwires::BuiltInPercussionInstruments::Value::ChnCym,
                                     seqwires::BuiltInPercussionInstruments::Value::RideBl,
                                     seqwires::BuiltInPercussionInstruments::Value::Tamb,
                                     seqwires::BuiltInPercussionInstruments::Value::SplCym,
                                     seqwires::BuiltInPercussionInstruments::Value::Cowbll, // 808 Cowbell
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
                                     seqwires::BuiltInPercussionInstruments::Value::OSurdo},
                                    25,
                                    &builtInInstruments},
                    25) {}

babelwires::LongIdentifier smf::Gs808909PercussionSet::getThisIdentifier() {
    return REGISTERED_LONGID("GS808909Perc", "Roland GS 808/909 Percussion", "fc25e9e7-0462-4581-adb6-924e3aee22be");
}

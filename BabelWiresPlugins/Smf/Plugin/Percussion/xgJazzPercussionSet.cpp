/**
 * A PercussionSet corresponding to the instruments of XG Jazz Percussion Set.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <BabelWiresPlugins/Smf/Plugin/Percussion/xgJazzPercussionSet.hpp>

// Follow GM2 and do not distinguish Jazz toms from regular toms.
smf::XgJazzPercussionSet::XgJazzPercussionSet(const seqwires::BuiltInPercussionInstruments& builtInInstruments)
    : PercussionSet(getThisIdentifier(), 1,
                    InstrumentBlock{builtInInstruments.convertToIdentifiers(
                                        {seqwires::BuiltInPercussionInstruments::Value::MSurdo,
                                         seqwires::BuiltInPercussionInstruments::Value::OSurdo,
                                         seqwires::BuiltInPercussionInstruments::Value::HighQ,
                                         seqwires::BuiltInPercussionInstruments::Value::Slap,
                                         seqwires::BuiltInPercussionInstruments::Value::ScrPsh,
                                         seqwires::BuiltInPercussionInstruments::Value::ScrPll,
                                         seqwires::BuiltInPercussionInstruments::Value::FngSnp,
                                         seqwires::BuiltInPercussionInstruments::Value::SqClck,
                                         seqwires::BuiltInPercussionInstruments::Value::MtrClk,
                                         seqwires::BuiltInPercussionInstruments::Value::MtrBll,
                                         seqwires::BuiltInPercussionInstruments::Value::BeepLo, // Seq Click L
                                         seqwires::BuiltInPercussionInstruments::Value::BeepHi, // Seq Click H
                                         seqwires::BuiltInPercussionInstruments::Value::BrTap,
                                         seqwires::BuiltInPercussionInstruments::Value::BrSwrL,
                                         seqwires::BuiltInPercussionInstruments::Value::BrSlap,
                                         seqwires::BuiltInPercussionInstruments::Value::BrSwrH,
                                         seqwires::BuiltInPercussionInstruments::Value::SnrRll,
                                         seqwires::BuiltInPercussionInstruments::Value::Cstnts,
                                         seqwires::BuiltInPercussionInstruments::Value::AcSnr, // Snare L
                                         seqwires::BuiltInPercussionInstruments::Value::Stcks,
                                         seqwires::BuiltInPercussionInstruments::Value::Bass1, // Bass Drum L
                                         seqwires::BuiltInPercussionInstruments::Value::RimSht,
                                         seqwires::BuiltInPercussionInstruments::Value::AcBass,
                                         seqwires::BuiltInPercussionInstruments::Value::JKick1,
                                         seqwires::BuiltInPercussionInstruments::Value::SStick,
                                         seqwires::BuiltInPercussionInstruments::Value::AcSnr,
                                         seqwires::BuiltInPercussionInstruments::Value::Clap,
                                         seqwires::BuiltInPercussionInstruments::Value::ElSnr,
                                         seqwires::BuiltInPercussionInstruments::Value::LFlTom, // Jazz Tom 1
                                         seqwires::BuiltInPercussionInstruments::Value::ClHHat,
                                         seqwires::BuiltInPercussionInstruments::Value::HFlTom, // Jazz Tom 2
                                         seqwires::BuiltInPercussionInstruments::Value::PdHHat,
                                         seqwires::BuiltInPercussionInstruments::Value::LwTom, // Jazz Tom 3
                                         seqwires::BuiltInPercussionInstruments::Value::OpHHat,
                                         seqwires::BuiltInPercussionInstruments::Value::LMTom, // Jazz Tom 4
                                         seqwires::BuiltInPercussionInstruments::Value::HMTom, // Jazz Tom 5
                                         seqwires::BuiltInPercussionInstruments::Value::Crash1,
                                         seqwires::BuiltInPercussionInstruments::Value::HTom, // Jazz Tom 6
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
                                         seqwires::BuiltInPercussionInstruments::Value::BlTree}),
                                    13},
                    13) {}

babelwires::LongIdentifier smf::XgJazzPercussionSet::getThisIdentifier() {
    return REGISTERED_LONGID("XGJazzPerc", "Yamaha XG Jazz Percussion", "101d8344-28c4-4045-83aa-52dc87fe5235");
}

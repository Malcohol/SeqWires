/**
 * A PercussionSet corresponding to the instruments of GS Standard Percussion Set.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <BabelWiresPlugins/Smf/Plugin/Percussion/gsBrushPercussionSet.hpp>

smf::GsBrushPercussionSet::GsBrushPercussionSet(
    const seqwires::BuiltInPercussionInstruments& builtInInstruments)
    : PercussionSet(getThisIdentifier(), 1,
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
                                     seqwires::BuiltInPercussionInstruments::Value::JKick2,
                                     seqwires::BuiltInPercussionInstruments::Value::JKick1,
                                     seqwires::BuiltInPercussionInstruments::Value::SStick,
                                     seqwires::BuiltInPercussionInstruments::Value::BrTap,
                                     seqwires::BuiltInPercussionInstruments::Value::BrSlap,
                                     seqwires::BuiltInPercussionInstruments::Value::BrSwrL,
                                     seqwires::BuiltInPercussionInstruments::Value::LFlTom, // Brush Low Tom 2
                                     seqwires::BuiltInPercussionInstruments::Value::ClHHat,
                                     seqwires::BuiltInPercussionInstruments::Value::HFlTom, // Brush Low Tom 1
                                     seqwires::BuiltInPercussionInstruments::Value::PdHHat,
                                     seqwires::BuiltInPercussionInstruments::Value::LwTom, // Brush Mid Tom 2
                                     seqwires::BuiltInPercussionInstruments::Value::OpHHat,
                                     seqwires::BuiltInPercussionInstruments::Value::LMTom,  // Brush Mid Tom 1
                                     seqwires::BuiltInPercussionInstruments::Value::HMTom,  // Brush Hi Tom 2
                                     seqwires::BuiltInPercussionInstruments::Value::Crash1, // Brush Crash Cymbal
                                     seqwires::BuiltInPercussionInstruments::Value::HTom,   // Brush Hi Tom 1
                                     seqwires::BuiltInPercussionInstruments::Value::Ride1,  // Brush Ride Cymbal
                                     seqwires::BuiltInPercussionInstruments::Value::ChnCym,
                                     seqwires::BuiltInPercussionInstruments::Value::RideBl, // Brush Ride Bell
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
                                     seqwires::BuiltInPercussionInstruments::Value::OSurdo},
                                    25,
                                    &builtInInstruments},
                    25) {}

babelwires::LongIdentifier smf::GsBrushPercussionSet::getThisIdentifier() {
    return REGISTERED_LONGID("GSBrushPerc", "Roland GS Brush Percussion",
                             "8a2d5bde-833f-43ea-9c6c-86c22174688b");
}

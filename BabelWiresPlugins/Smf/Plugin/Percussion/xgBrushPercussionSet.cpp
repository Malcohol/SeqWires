/**
 * A PercussionSet corresponding to the instruments of XG BrushPercussion Set.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <BabelWiresPlugins/Smf/Plugin/Percussion/xgBrushPercussionSet.hpp>

// Follow GM2 and do not distinguish Brush toms from regular toms.
// Choosing JKick2 instead of JKick1 to ensure its set of instruments differs from the XG Jazz set.
// (It needs to exist, since it has a different layout to the Jazz set, but a different instrument
// set ensures it's easier to test.)
smf::XgBrushPercussionSet::XgBrushPercussionSet(
    const seqwires::BuiltInPercussionInstruments& builtInInstruments)
    : PercussionSet(getThisIdentifier(), 1,
                    builtInInstruments.convertToIdentifiers({seqwires::BuiltInPercussionInstruments::Value::MSurdo,
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
                                                             seqwires::BuiltInPercussionInstruments::Value::BrSlap, // Snare L
                                                             seqwires::BuiltInPercussionInstruments::Value::Stcks,
                                                             seqwires::BuiltInPercussionInstruments::Value::Bass1, // Bass Drum L
                                                             seqwires::BuiltInPercussionInstruments::Value::RimSht,
                                                             seqwires::BuiltInPercussionInstruments::Value::AcBass,
                                                             seqwires::BuiltInPercussionInstruments::Value::JKick2, // BD Soft
                                                             seqwires::BuiltInPercussionInstruments::Value::SStick,
                                                             seqwires::BuiltInPercussionInstruments::Value::BrSlap,
                                                             seqwires::BuiltInPercussionInstruments::Value::Clap,
                                                             seqwires::BuiltInPercussionInstruments::Value::BrTap,
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
                                                             seqwires::BuiltInPercussionInstruments::Value::BlTree}),
                    0, 13) {}

babelwires::LongIdentifier smf::XgBrushPercussionSet::getThisIdentifier() {
    return REGISTERED_LONGID("XGBrushPerc", "Yamaha XG BrushPercussion",
                             "096bbcd8-5302-42e0-9ac9-560fd7a570f3");
}

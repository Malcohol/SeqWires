/**
 * A PercussionSet corresponding to the instruments of GM2 Standard Percussion Set.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <BabelWiresPlugins/Smf/Plugin/Percussion/gm2BrushPercussionSet.hpp>

smf::GM2BrushPercussionSet::GM2BrushPercussionSet(
    const seqwires::BuiltInPercussionInstruments& builtInInstruments)
    : PercussionSet(getThisIdentifier(), 1,
                    builtInInstruments.convertToIdentifiers({seqwires::BuiltInPercussionInstruments::Value::HighQ,
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
                                                             seqwires::BuiltInPercussionInstruments::Value::BrSwrl,
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
                    0, 27) {}

babelwires::LongIdentifier smf::GM2BrushPercussionSet::getThisIdentifier() {
    return REGISTERED_LONGID("GM2BrushPerc", "General MIDI 2 Brush Percussion", "9686b521-ffb2-4275-b98a-e7e6cdc1f91d");
}
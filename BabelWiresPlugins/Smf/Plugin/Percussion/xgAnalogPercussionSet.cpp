/**
 * A PercussionSet corresponding to the instruments of XG Analog Percussion Set.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <BabelWiresPlugins/Smf/Plugin/Percussion/xgAnalogPercussionSet.hpp>

smf::XgAnalogPercussionSet::XgAnalogPercussionSet(
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
                                                             seqwires::BuiltInPercussionInstruments::Value::RevCym, // Swirl H
                                                             seqwires::BuiltInPercussionInstruments::Value::SnrRll,
                                                             seqwires::BuiltInPercussionInstruments::Value::HighQ,
                                                             seqwires::BuiltInPercussionInstruments::Value::AcSnr, // SD Rock H
                                                             seqwires::BuiltInPercussionInstruments::Value::Stcks,
                                                             seqwires::BuiltInPercussionInstruments::Value::Bass1, // Bass Drum M
                                                             seqwires::BuiltInPercussionInstruments::Value::RimSht,
                                                             seqwires::BuiltInPercussionInstruments::Value::AnBass, // BD Analog L
                                                             seqwires::BuiltInPercussionInstruments::Value::AnBass, // BD Analog H
                                                             seqwires::BuiltInPercussionInstruments::Value::SStick, // Analog Side Stick
                                                             seqwires::BuiltInPercussionInstruments::Value::AcSnr, // Analog Snare L
                                                             seqwires::BuiltInPercussionInstruments::Value::Clap,
                                                             seqwires::BuiltInPercussionInstruments::Value::ElSnr, // Analog Snare H
                                                             seqwires::BuiltInPercussionInstruments::Value::ALwTm2,
                                                             seqwires::BuiltInPercussionInstruments::Value::ACHH1,
                                                             seqwires::BuiltInPercussionInstruments::Value::ALwTm1,
                                                             seqwires::BuiltInPercussionInstruments::Value::ACHH2,
                                                             seqwires::BuiltInPercussionInstruments::Value::AMdTm2,
                                                             seqwires::BuiltInPercussionInstruments::Value::AOHH,
                                                             seqwires::BuiltInPercussionInstruments::Value::AMdTm1,
                                                             seqwires::BuiltInPercussionInstruments::Value::AHiTm2,
                                                             seqwires::BuiltInPercussionInstruments::Value::AnCym,
                                                             seqwires::BuiltInPercussionInstruments::Value::AHiTm1,
                                                             seqwires::BuiltInPercussionInstruments::Value::Ride1,
                                                             seqwires::BuiltInPercussionInstruments::Value::ChnCym,
                                                             seqwires::BuiltInPercussionInstruments::Value::RideBl,
                                                             seqwires::BuiltInPercussionInstruments::Value::Tamb,
                                                             seqwires::BuiltInPercussionInstruments::Value::SplCym,
                                                             seqwires::BuiltInPercussionInstruments::Value::ACwbll,
                                                             seqwires::BuiltInPercussionInstruments::Value::Crash2,
                                                             seqwires::BuiltInPercussionInstruments::Value::VibraS,
                                                             seqwires::BuiltInPercussionInstruments::Value::Ride2,
                                                             seqwires::BuiltInPercussionInstruments::Value::HBongo,
                                                             seqwires::BuiltInPercussionInstruments::Value::LBongo,
                                                             seqwires::BuiltInPercussionInstruments::Value::AHCnga,
                                                             seqwires::BuiltInPercussionInstruments::Value::AMCnga,
                                                             seqwires::BuiltInPercussionInstruments::Value::ALCnga,
                                                             seqwires::BuiltInPercussionInstruments::Value::HTimbl,
                                                             seqwires::BuiltInPercussionInstruments::Value::LTimbl,
                                                             seqwires::BuiltInPercussionInstruments::Value::HAgogo,
                                                             seqwires::BuiltInPercussionInstruments::Value::LAgogo,
                                                             seqwires::BuiltInPercussionInstruments::Value::Cabasa,
                                                             seqwires::BuiltInPercussionInstruments::Value::AMrcas,
                                                             seqwires::BuiltInPercussionInstruments::Value::SWhisl,
                                                             seqwires::BuiltInPercussionInstruments::Value::LWhisl,
                                                             seqwires::BuiltInPercussionInstruments::Value::SGuiro,
                                                             seqwires::BuiltInPercussionInstruments::Value::LGuiro,
                                                             seqwires::BuiltInPercussionInstruments::Value::AClavs,
                                                             seqwires::BuiltInPercussionInstruments::Value::HWoodB,
                                                             seqwires::BuiltInPercussionInstruments::Value::LWoodB,
                                                             seqwires::BuiltInPercussionInstruments::Value::ScrPsh,
                                                             seqwires::BuiltInPercussionInstruments::Value::ScrPll,
                                                             seqwires::BuiltInPercussionInstruments::Value::MTrian,
                                                             seqwires::BuiltInPercussionInstruments::Value::OTrian,
                                                             seqwires::BuiltInPercussionInstruments::Value::Shaker,
                                                             seqwires::BuiltInPercussionInstruments::Value::JngBll,
                                                             seqwires::BuiltInPercussionInstruments::Value::BlTree}),
                    0, 13) {}

babelwires::LongIdentifier smf::XgAnalogPercussionSet::getThisIdentifier() {
    return REGISTERED_LONGID("XGAnalogPerc", "Yamaha XG Analog Percussion",
                             "f172fe57-7ec0-43de-9031-3a4ebe3da84d");
}

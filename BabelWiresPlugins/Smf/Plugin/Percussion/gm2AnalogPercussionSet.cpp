/**
 * A PercussionSet corresponding to the instruments of GM2 standard percussion set.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <BabelWiresPlugins/Smf/Plugin/Percussion/gm2AnalogPercussionSet.hpp>

smf::GM2AnalogPercussionSet::GM2AnalogPercussionSet(
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
                                                             seqwires::BuiltInPercussionInstruments::Value::AcBass,
                                                             seqwires::BuiltInPercussionInstruments::Value::AnBass,
                                                             seqwires::BuiltInPercussionInstruments::Value::ARmSht,
                                                             seqwires::BuiltInPercussionInstruments::Value::AnSnr1,
                                                             seqwires::BuiltInPercussionInstruments::Value::Clap,
                                                             seqwires::BuiltInPercussionInstruments::Value::ElSnr,
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

babelwires::LongIdentifier smf::GM2AnalogPercussionSet::getThisIdentifier() {
    return REGISTERED_LONGID("GM2AnalogPerc", "General MIDI 2 Analog Percussion", "a077c9c6-e66d-46d2-b5e2-0e839b99ae70");
}

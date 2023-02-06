/**
 * A PercussionSet corresponding to the instruments of GM2 Standard Percussion Set.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <Plugins/Smf/Plugin/Percussion/gm2OrchestraPercussionSet.hpp>

smf::GM2OrchestraPercussionSet::GM2OrchestraPercussionSet(
    const seqwires::BuiltInPercussionInstruments& builtInInstruments)
    : PercussionSetWithPitchMap(getThisIdentifier(), 1,
                    InstrumentBlock{{seqwires::BuiltInPercussionInstruments::Value::ClHH2,
                                     seqwires::BuiltInPercussionInstruments::Value::PdHHat,
                                     seqwires::BuiltInPercussionInstruments::Value::OpHH2,
                                     seqwires::BuiltInPercussionInstruments::Value::Ride1,
                                     seqwires::BuiltInPercussionInstruments::Value::Stcks,
                                     seqwires::BuiltInPercussionInstruments::Value::SqClck,
                                     seqwires::BuiltInPercussionInstruments::Value::MtrClk,
                                     seqwires::BuiltInPercussionInstruments::Value::MtrBll,
                                     seqwires::BuiltInPercussionInstruments::Value::CnBD2,
                                     seqwires::BuiltInPercussionInstruments::Value::CnBD1,
                                     seqwires::BuiltInPercussionInstruments::Value::SStick,
                                     seqwires::BuiltInPercussionInstruments::Value::CnSD,
                                     seqwires::BuiltInPercussionInstruments::Value::Cstnts,
                                     seqwires::BuiltInPercussionInstruments::Value::CnSD,
                                     seqwires::BuiltInPercussionInstruments::Value::TimpF,
                                     seqwires::BuiltInPercussionInstruments::Value::TimpFs,
                                     seqwires::BuiltInPercussionInstruments::Value::TimpG,
                                     seqwires::BuiltInPercussionInstruments::Value::TimpGs,
                                     seqwires::BuiltInPercussionInstruments::Value::TimpA,
                                     seqwires::BuiltInPercussionInstruments::Value::TimpAs,
                                     seqwires::BuiltInPercussionInstruments::Value::TimpB,
                                     seqwires::BuiltInPercussionInstruments::Value::Timpc,
                                     seqwires::BuiltInPercussionInstruments::Value::Timpcs,
                                     seqwires::BuiltInPercussionInstruments::Value::Timpd,
                                     seqwires::BuiltInPercussionInstruments::Value::Timpds,
                                     seqwires::BuiltInPercussionInstruments::Value::Timpe,
                                     seqwires::BuiltInPercussionInstruments::Value::Timpf,
                                     seqwires::BuiltInPercussionInstruments::Value::Tamb,
                                     seqwires::BuiltInPercussionInstruments::Value::SplCym,
                                     seqwires::BuiltInPercussionInstruments::Value::Cowbll,
                                     seqwires::BuiltInPercussionInstruments::Value::Crash2,
                                     seqwires::BuiltInPercussionInstruments::Value::CnCym2,
                                     seqwires::BuiltInPercussionInstruments::Value::Ride2,
                                     seqwires::BuiltInPercussionInstruments::Value::CnCym1,
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
                                     seqwires::BuiltInPercussionInstruments::Value::OSurdo,
                                     seqwires::BuiltInPercussionInstruments::Value::Aplaus},
                                    27,
                                    &builtInInstruments},
                    27) {}

babelwires::LongIdentifier smf::GM2OrchestraPercussionSet::getThisIdentifier() {
    return REGISTERED_LONGID("GM2OrchestraPerc", "General MIDI 2 Orchestra Percussion",
                             "6a77f085-35c7-409c-92e1-5f4a20f3c750");
}

/**
 * A PercussionSet corresponding to the instruments of XG SFX 2 Percussion Set.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <Plugins/Smf/Plugin/Percussion/xgSFX2PercussionSet.hpp>

smf::XgSFX2PercussionSet::XgSFX2PercussionSet(const seqwires::BuiltInPercussionInstruments& builtInInstruments)
    : PercussionSetWithPitchMap(
          std::vector<InstrumentBlock>{
              {{seqwires::BuiltInPercussionInstruments::Value::DialTn,
                seqwires::BuiltInPercussionInstruments::Value::DoorCr,
                seqwires::BuiltInPercussionInstruments::Value::Door,   // Door Slam
                seqwires::BuiltInPercussionInstruments::Value::ScrPsh, // Scratch
                seqwires::BuiltInPercussionInstruments::Value::ScrPll, // Scratch 2
                seqwires::BuiltInPercussionInstruments::Value::WChime,
                seqwires::BuiltInPercussionInstruments::Value::TeleRn},
               36,
               &builtInInstruments},
              {{seqwires::BuiltInPercussionInstruments::Value::CarEng, // Engine Start
                seqwires::BuiltInPercussionInstruments::Value::TireSc,
                seqwires::BuiltInPercussionInstruments::Value::CarPss,
                seqwires::BuiltInPercussionInstruments::Value::CarCrs,
                seqwires::BuiltInPercussionInstruments::Value::Siren,
                seqwires::BuiltInPercussionInstruments::Value::Train,
                seqwires::BuiltInPercussionInstruments::Value::Jetpln,
                seqwires::BuiltInPercussionInstruments::Value::Strshp,
                seqwires::BuiltInPercussionInstruments::Value::Siren,
                REGISTERED_ID("XgBNse", "XG Burst Noise", "b57aa245-2f69-458d-94df-b77dccc456d8"),
                REGISTERED_ID("XgCstr", "XG Coaster", "ccee260e-6319-49eb-b04f-c9f1ded56b26"),
                seqwires::BuiltInPercussionInstruments::Value::Submrn},
               52,
               &builtInInstruments},
              {{seqwires::BuiltInPercussionInstruments::Value::Laugh,
                seqwires::BuiltInPercussionInstruments::Value::Scream,
                seqwires::BuiltInPercussionInstruments::Value::Punch,
                seqwires::BuiltInPercussionInstruments::Value::HeartB,
                seqwires::BuiltInPercussionInstruments::Value::Foot1,
                seqwires::BuiltInPercussionInstruments::Value::Aplaus},
               68,
               &builtInInstruments},
              {{seqwires::BuiltInPercussionInstruments::Value::MGun,
                seqwires::BuiltInPercussionInstruments::Value::Lsrgun,
                seqwires::BuiltInPercussionInstruments::Value::Explsn,
                seqwires::BuiltInPercussionInstruments::Value::Frewrk},
               84,
               &builtInInstruments},
          },
          36) {}

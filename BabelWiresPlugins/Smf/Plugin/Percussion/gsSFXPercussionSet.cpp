/**
 * A PercussionSet corresponding to the instruments of GS SFX Percussion Set.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <BabelWiresPlugins/Smf/Plugin/Percussion/gsSFXPercussionSet.hpp>

smf::GsSFXPercussionSet::GsSFXPercussionSet(const seqwires::BuiltInPercussionInstruments& builtInInstruments)
    : PercussionSet(
          getThisIdentifier(), 1,
          InstrumentBlock{
              {
                  seqwires::BuiltInPercussionInstruments::Value::ScrPsh, // Scratch Push 2
                  seqwires::BuiltInPercussionInstruments::Value::ScrPll, // Scratch Push 1
                  seqwires::BuiltInPercussionInstruments::Value::GCutUp, // Cutting Noise Up 2
                  seqwires::BuiltInPercussionInstruments::Value::GCutDn, // Cutting Noise Down 2
                  seqwires::BuiltInPercussionInstruments::Value::GCutUp, // Distortion Guitar Cutting Noise Up
                  seqwires::BuiltInPercussionInstruments::Value::GCutDn, // Distortion Guitar Cutting Noise Down
                  seqwires::BuiltInPercussionInstruments::Value::BssSld,
                  seqwires::BuiltInPercussionInstruments::Value::PckScr,
                  seqwires::BuiltInPercussionInstruments::Value::HighQ,
                  seqwires::BuiltInPercussionInstruments::Value::Slap,
                  seqwires::BuiltInPercussionInstruments::Value::ScrPsh,
                  seqwires::BuiltInPercussionInstruments::Value::ScrPll,
                  seqwires::BuiltInPercussionInstruments::Value::Stcks,
                  seqwires::BuiltInPercussionInstruments::Value::SqClck,
                  seqwires::BuiltInPercussionInstruments::Value::MtrClk,
                  seqwires::BuiltInPercussionInstruments::Value::MtrBll,
                  seqwires::BuiltInPercussionInstruments::Value::GFret,
                  seqwires::BuiltInPercussionInstruments::Value::GCutUp,
                  seqwires::BuiltInPercussionInstruments::Value::GCutDn,
                  seqwires::BuiltInPercussionInstruments::Value::DBsSlp,
                  seqwires::BuiltInPercussionInstruments::Value::FKyClk,
                  seqwires::BuiltInPercussionInstruments::Value::Laugh,
                  seqwires::BuiltInPercussionInstruments::Value::Scream,
                  seqwires::BuiltInPercussionInstruments::Value::Punch,
                  seqwires::BuiltInPercussionInstruments::Value::HeartB,
                  seqwires::BuiltInPercussionInstruments::Value::Foot1,
                  seqwires::BuiltInPercussionInstruments::Value::Foot2,
                  seqwires::BuiltInPercussionInstruments::Value::Aplaus,
                  seqwires::BuiltInPercussionInstruments::Value::DoorCr,
                  seqwires::BuiltInPercussionInstruments::Value::Door,
                  seqwires::BuiltInPercussionInstruments::Value::Scrtch,
                  seqwires::BuiltInPercussionInstruments::Value::WChime,
                  seqwires::BuiltInPercussionInstruments::Value::CarEng,
                  seqwires::BuiltInPercussionInstruments::Value::CarStp,
                  seqwires::BuiltInPercussionInstruments::Value::CarPss,
                  seqwires::BuiltInPercussionInstruments::Value::CarCrs,
                  seqwires::BuiltInPercussionInstruments::Value::Siren,
                  seqwires::BuiltInPercussionInstruments::Value::Train,
                  seqwires::BuiltInPercussionInstruments::Value::Jetpln,
                  seqwires::BuiltInPercussionInstruments::Value::Heli,
                  seqwires::BuiltInPercussionInstruments::Value::Strshp,
                  seqwires::BuiltInPercussionInstruments::Value::GunSht,
                  seqwires::BuiltInPercussionInstruments::Value::MGun,
                  seqwires::BuiltInPercussionInstruments::Value::Lsrgun,
                  seqwires::BuiltInPercussionInstruments::Value::Explsn,
                  seqwires::BuiltInPercussionInstruments::Value::Dog,
                  seqwires::BuiltInPercussionInstruments::Value::Gallop,
                  seqwires::BuiltInPercussionInstruments::Value::Birds,
                  seqwires::BuiltInPercussionInstruments::Value::Rain,
                  seqwires::BuiltInPercussionInstruments::Value::Thnder,
                  seqwires::BuiltInPercussionInstruments::Value::Wind,
                  seqwires::BuiltInPercussionInstruments::Value::Seashr,
                  seqwires::BuiltInPercussionInstruments::Value::Stream,
                  seqwires::BuiltInPercussionInstruments::Value::Bubble,
                  seqwires::BuiltInPercussionInstruments::Value::Kitty,
                  seqwires::BuiltInPercussionInstruments::Value::Birds, // Bird 2
                  seqwires::BuiltInPercussionInstruments::Value::Growl,
                  seqwires::BuiltInPercussionInstruments::Value::Aplaus, // Applause 2
                  seqwires::BuiltInPercussionInstruments::Value::TeleRn, // Telephone 1
                  seqwires::BuiltInPercussionInstruments::Value::TeleRn  // Telephone 2
              },
              31,
              &builtInInstruments},
          31) {}

babelwires::LongIdentifier smf::GsSFXPercussionSet::getThisIdentifier() {
    return REGISTERED_LONGID("GSSFXPerc", "Roland GS SFX Percussion", "135823fa-ec1b-427e-9e03-f888be7383c8");
}

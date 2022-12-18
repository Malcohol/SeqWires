/**
 * A PercussionSet corresponding to the instruments of GM2 SFX Percussion Set.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <BabelWiresPlugins/Smf/Plugin/Percussion/gm2SFXPercussionSet.hpp>

smf::GM2SFXPercussionSet::GM2SFXPercussionSet(
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
                                                             seqwires::BuiltInPercussionInstruments::Value::Bubble}),
                    0, 39) {}

babelwires::LongIdentifier smf::GM2SFXPercussionSet::getThisIdentifier() {
    return REGISTERED_LONGID("GM2SFXPerc", "General MIDI 2 SFX Percussion", "5d51ad28-2f32-467e-a5be-8caafe3e6b39");
}

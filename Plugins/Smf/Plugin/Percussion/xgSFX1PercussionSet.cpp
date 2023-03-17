/**
 * A PercussionSet corresponding to the instruments of XG SFX 1 Percussion Set.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <Plugins/Smf/Plugin/Percussion/xgSFX1PercussionSet.hpp>

smf::XgSFX1PercussionSet::XgSFX1PercussionSet(const seqwires::BuiltInPercussionInstruments& builtInInstruments)
    : PercussionSetWithPitchMap(
          std::vector<InstrumentBlock>{
              {{seqwires::BuiltInPercussionInstruments::Value::GCutUp, // Guitar Cutting Noise
                seqwires::BuiltInPercussionInstruments::Value::GCutDn, // Guitar Cutting Noise 2
                seqwires::BuiltInPercussionInstruments::Value::DCutNs,
                seqwires::BuiltInPercussionInstruments::Value::DBsSlp,
                seqwires::BuiltInPercussionInstruments::Value::BssSld,
                seqwires::BuiltInPercussionInstruments::Value::PckScr},
               36,
               &builtInInstruments},
              {{seqwires::BuiltInPercussionInstruments::Value::FKyClk}, 52, &builtInInstruments},
              {{seqwires::BuiltInPercussionInstruments::Value::Rain,
                seqwires::BuiltInPercussionInstruments::Value::Thnder,
                seqwires::BuiltInPercussionInstruments::Value::Wind,
                seqwires::BuiltInPercussionInstruments::Value::Stream,
                seqwires::BuiltInPercussionInstruments::Value::Bubble,
                BW_SHORT_ID("XgFeed", "XG Feed", "68af6288-ccda-41f8-b651-d703f5fa56eb")},
               68,
               &builtInInstruments},
              {{seqwires::BuiltInPercussionInstruments::Value::Dog,
                seqwires::BuiltInPercussionInstruments::Value::Gallop,
                seqwires::BuiltInPercussionInstruments::Value::Birds,
                seqwires::BuiltInPercussionInstruments::Value::Kitty,
                seqwires::BuiltInPercussionInstruments::Value::Growl,
                seqwires::BuiltInPercussionInstruments::Value::Ghost,
                BW_SHORT_ID("XgMaou", "XG Maou", "8545a36b-2dfc-4826-8873-76989550d7c7")},
               84,
               &builtInInstruments},
          },
          36) {}

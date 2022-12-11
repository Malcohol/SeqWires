/**
 * Defines an enum for the built-in percussion instruments.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <SeqWiresLib/musicTypes.hpp>

#include <BabelWiresLib/Enums/enumWithCppEnum.hpp>

#include <optional>

/// A master list of all the built-in percussion instruments.
/// We assume the General MIDI standard v2 gives good coverage of percussion instruments, so we use the instruments
/// found there. Note that the notion of a percussion instrument is extensible: it is not required that a percussion
/// event have a value defined by this set.
#define BUILT_IN_PERCUSSION_INSTRUMENTS(X)                                                                             \
    /* Instruments of the GM2 Standard Kit */                                                                          \
    X(HighQ, "High Q", "22074fd9-40ed-4716-8e5b-4e174daa03b3")                                                         \
    X(Slap, "Slap", "3aedf5de-904b-46e2-bc87-445c9f065868")                                                            \
    X(ScrPsh, "Scratch Push", "3087699b-94bd-4612-b1bd-b5acf037c325")                                                  \
    X(ScrPll, "Scratch Pull", "8a74f73e-691d-445f-bd2e-fd75bc91a8ef")                                                  \
    X(Stcks, "Sticks", "c61880f7-2ed2-431b-b23a-14a5457bacfd")                                                         \
    X(SqClck, "Square Click", "4d887ce2-c748-4127-b3ed-f8430237a82f")                                                  \
    X(MtrClk, "Metronome Click", "a012ad55-b894-4687-8754-6574f492f90e")                                               \
    X(MtrBll, "Metronome Bell", "876abe67-81a4-41dd-9d7b-dbc3652c47a1")                                                \
    /* Instruments of the GM kit */                                                                                    \
    X(AcBass, "Acoustic Bass Drum", "6ce9c1ac-3439-41eb-9776-dac41184255c")                                            \
    X(Bass1, "Bass Drum 1", "1c39fd51-ed6b-439e-ac8e-3c0fc7cb0d3b")                                                    \
    X(SStick, "Side Stick", "7762bf9b-ee64-4af2-9397-f6a55751f845")                                                    \
    X(AcSnr, "Acoustic Snare", "86d0c201-7866-4d81-bf54-2417ff8d94b6")                                                 \
    X(Clap, "Hand Clap", "377aa8da-201c-4279-8db0-8bb1bfff48b3")                                                       \
    X(ElSnr, "Electric Snare", "d89ecda6-b911-47d3-b4a1-5bf24e358a52")                                                 \
    X(LFlTom, "Low Floor Tom", "1c125e5d-8764-4704-919b-a00a505d48bf")                                                 \
    X(ClHHat, "Closed Hi Hat", "594b33b1-61e6-44ee-877b-e12ff7309bf3")                                                 \
    X(HFlTom, "High Floor Tom", "df566de2-2e0d-4417-9ad0-a0225a28be81")                                                \
    X(PdHHat, "Pedal Hi Hat", "e79e3113-34da-4faa-ad71-fe6200c23a4b")                                                  \
    X(LwTom, "Low Tom", "4e0844b0-b79b-40ef-ae6d-8fbb8476799c")                                                        \
    X(OpHHat, "Open Hi Hat", "0fba2ee3-8223-4275-b4ac-06186824571d")                                                   \
    X(LMTom, "Low-Mid Tom", "8daf9af5-5839-4a60-a0fa-9ce7f8ddce23")                                                    \
    X(HMTom, "Hi-Mid Tom", "d05899d0-bee6-42ab-ae27-288bd7d35894")                                                     \
    X(Crash1, "Crash Cymbal 1", "19b94421-a2db-481c-a5c6-d760884e8b3f")                                                \
    X(HTom, "High Tom", "9a125d11-0376-4ae5-a4b0-8870fae943f6")                                                        \
    X(Ride1, "Ride Cymbal 1", "325540cc-bce6-4f4f-babd-89b56a48002d")                                                  \
    X(ChnCym, "Chinese Cymbal", "963bc408-32e7-4c33-a743-764ed26423d8")                                                \
    X(RideBl, "Ride Bell", "b0fa8c86-df41-4e4b-b9ce-e04a935f1db4")                                                     \
    X(Tamb, "Tambourine", "1c7b945f-d6c8-4213-a6c2-e15667399f9f")                                                      \
    X(SplCym, "Splash Cymbal", "3a630d65-0fb0-4272-ac30-ede579203e6d")                                                 \
    X(Cowbll, "Cowbell", "513e999a-40bf-4a3f-8164-c4b881300e4f")                                                       \
    X(Crash2, "Crash Cymbal 2", "b8604a23-c381-4cc5-aed4-fafa243a96f9")                                                \
    X(VibraS, "Vibraslap", "8948c9eb-31a1-4eb4-8b75-9b4a23c3b900")                                                     \
    X(Ride2, "Ride Cymbal 2", "266e41ce-c70c-4d51-8387-04025bf1b58d")                                                  \
    X(HBongo, "Hi Bongo", "a04a6551-b076-4bcc-a909-51ee8547e46c")                                                      \
    X(LBongo, "Low Bongo", "023c2b30-5616-42ce-93ca-0051b26487ba")                                                     \
    X(MHCnga, "Mute Hi Conga", "baaf8149-a3a4-479a-916f-cc34e8eaffc7")                                                 \
    X(OHCnga, "Open Hi Conga", "7188bb81-1e73-4c66-91c0-c9af3a8dcd38")                                                 \
    X(LConga, "Low Conga", "9c9eba72-632f-4a46-8c43-46425c0f8918")                                                     \
    X(HTimbl, "High Timbale", "da75f840-55a7-41c9-b74b-69c3ee16a299")                                                  \
    X(LTimbl, "Low Timbale", "fa81b07d-7c14-4331-8390-046166dc869f")                                                   \
    X(HAgogo, "High Agogo", "5b6f72e1-c491-42b4-b8b8-1e49648190fd")                                                    \
    X(LAgogo, "Low Agogo", "5709ee1f-50a0-44a2-a4e2-be2cf0b0affb")                                                     \
    X(Cabasa, "Cabasa", "feb12af2-bcaa-42cc-8325-ac5f73fdd835")                                                        \
    X(Maracs, "Maracas", "d824a9de-1c48-460d-b186-cdbe92329eaa")                                                       \
    X(SWhisl, "Short Whistle", "122a08b8-37cb-4caa-a12c-5ae4888475ac")                                                 \
    X(LWhisl, "Long Whistle", "f47406bb-47a1-47a1-b526-004ed56e0a16")                                                  \
    X(SGuiro, "Short Guiro", "cb801da6-8aad-4e29-bfed-83ed25490ff4")                                                   \
    X(LGuiro, "Long Guiro", "3bbd1626-0258-41d1-a84c-8c59939e3313")                                                    \
    X(Claves, "Claves", "76262dc4-7b66-4d8e-a68a-2ee29f675562")                                                        \
    X(HWoodB, "Hi Wood Block", "568178ca-43a8-454e-8415-02bceb1ecb36")                                                 \
    X(LWoodB, "Low Wood Block", "9e8ff317-af8c-4f99-a6d0-41a38ca6da96")                                                \
    X(MCuica, "Mute Cuica", "419d9a21-3b53-45f5-bf5a-fcdc72837708")                                                    \
    X(OCuica, "Open Cuica", "2e523d69-fea3-4145-96b7-1402f61472d5")                                                    \
    X(MTrian, "Mute Triangle", "8d72a586-72ee-48fe-9bbd-c1a3500ab648")                                                 \
    X(OTrian, "Open Triangle", "8ffa30b4-287f-4dcf-8d18-92def658e0d6")                                                 \
    /* Instruments of the GM2 Standard Kit */                                                                          \
    X(Shaker, "Shaker", "ee9c029a-98af-4679-adc4-3c5185ca2e95")                                                        \
    X(JngBll, "Jingle Bell", "4a3da07d-a820-44f0-98fc-8c409768f7c2")                                                   \
    X(BlTree, "Bell Tree", "770dd65c-6dd1-43ba-89f8-c4507c158709")                                                     \
    X(Cstnts, "Castanets", "f742bbe3-3679-4cbd-9727-5252fd1bfff1")                                                     \
    X(MSurdo, "Mute Surdo", "79286a74-fd6d-4918-8f4c-f79606e48d4e")                                                    \
    X(OSurdo, "Open Surdo", "16a9961e-26a2-4e85-bb8c-32b139713694")                                                    \
    /* Instruments from the GM2 Room kit */                                                                            \
    X(RLwTm2, "Room Low Tom 2", "d2c5e125-7853-46f8-bfd3-7dc707e62141")                                                \
    X(RLwTm1, "Room Low Tom 1", "e6b25f32-4a7e-4203-9aa3-76f0c6085ced")                                                \
    X(RMdTm2, "Room Mid Tom 2", "371bd6eb-33cb-40c8-ba57-3c18eac00787")                                                \
    X(RMdTm1, "Room Mid Tom 1", "58339244-fb5d-49d1-9403-8cf812388ab7")                                                \
    X(RHTm2, "Room Hi Tom 2", "46d6b67b-9e66-46fb-ae5e-24b1296c856c")                                                  \
    X(RHTm1, "Room Hi Tom 1", "a4b7fc45-3e90-4da4-b3b8-1e437d9a1e00")                                                  \
    /* Instruments from the GM2 Power kit */                                                                           \
    X(PKick, "Power Kick Drum", "326850a1-707f-4b6d-9940-0a9ca28d5a8b")                                                \
    X(PSnare, "Power Snare Drum", "26b107dc-8ad3-4875-a6ea-c7aff1d852ce")                                              \
    X(PLwTm2, "Power Low Tom 2", "90dddc4a-18d3-45ca-a942-f667a80a5963")                                               \
    X(PLwTm1, "Power Low Tom 1", "d8038c6c-5a57-404a-a819-f1f9aa152288")                                               \
    X(PMdTm2, "Power Mid Tom 2", "ad05c742-99a9-4ac1-98c3-163ba57458a4")                                               \
    X(PMdTm1, "Power Mid Tom 1", "a117afa9-9efa-4940-a856-f1f65352e25f")                                               \
    X(PHTm2, "Power Hi Tom 2", "c0c794da-5353-46bb-9b65-a695ee8deae7")                                                 \
    X(PHTm1, "Power Hi Tom 1", "b18634b2-bdba-44be-8259-822b4b96be8a")

namespace seqwires {
    /// All percussion instruments built into SeqWires.
    /// This is just a holder of instruments. It cannot be used as a PercussionKit because the values do not have a 1-1
    /// mapping to pitch values.
    class BuiltInPercussionInstruments : public babelwires::Enum {
      public:
        BuiltInPercussionInstruments();

        ENUM_DEFINE_CPP_ENUM(BUILT_IN_PERCUSSION_INSTRUMENTS);

        static babelwires::LongIdentifier getThisIdentifier();

        /// Convert a vector of selected percussion instruments to a vector of identifiers.
        babelwires::Enum::EnumValues convertToIdentifiers(const std::vector<Value>& values) const;
    };
} // namespace seqwires

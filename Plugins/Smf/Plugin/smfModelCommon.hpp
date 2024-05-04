/**
 * Representation of a Standard MIDI File as a tree of Features.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <Plugins/Smf/Plugin/gmSpec.hpp>

#include <SeqWiresLib/Features/tempoFeature.hpp>

#include <BabelWiresLib/Features/recordWithOptionalsFeature.hpp>
#include <BabelWiresLib/Types/Record/recordType.hpp>
#include <BabelWiresLib/Types/String/stringFeature.hpp>

#include <optional>

namespace seqwires {
    class TrackFeature;
}

namespace smf {
    class GmSpecTypeFeature : public babelwires::EnumWithCppEnumFeature<GMSpecType> {};

    class MidiMetadata : public babelwires::RecordType {
      public:
        PRIMITIVE_TYPE("midiMetadata", "MidiMetadata", "5032054f-d1b7-4cb4-8bac-2de60a1bc078", 1);

        MidiMetadata();

        // TODO Move into a new RecordTypeFeature class.
        static const babelwires::ValueFeature& getChild(const babelwires::ValueFeature& recordFeature, babelwires::ShortId id);
        static babelwires::ValueFeature& getChild(babelwires::ValueFeature& recordFeature, babelwires::ShortId id);
        static const babelwires::ValueFeature* tryGetChild(const babelwires::ValueFeature& recordFeature, babelwires::ShortId id);
        static babelwires::ValueFeature& activateAndGetChild(babelwires::ValueFeature& recordFeature, babelwires::ShortId id);
        
        static GMSpecType::Value getSpecValue(const babelwires::ValueFeature& midiMetadataFeature);
        static void setSpecValue(babelwires::ValueFeature& midiMetadataFeature, GMSpecType::Value newSpec);

        static std::optional<int> tryGetTempo(const babelwires::ValueFeature& midiMetadataFeature);
        static void setTempo(babelwires::ValueFeature& midiMetadataFeature, int newTempo);

        static std::optional<std::string> tryGetCopyright(const babelwires::ValueFeature& midiMetadataFeature);
        static void setCopyright(babelwires::ValueFeature& midiMetadataFeature, std::string newCopyright);

        static std::optional<std::string> tryGetSequenceName(const babelwires::ValueFeature& midiMetadataFeature);
        static void setSequenceName(babelwires::ValueFeature& midiMetadataFeature, std::string newSequenceName);
    };

    class MidiMetadataFeature : public babelwires::SimpleValueFeature {
      public:
        MidiMetadataFeature();
    };
} // namespace smf

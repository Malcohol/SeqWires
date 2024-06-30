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

#include <BabelWiresLib/Types/Record/recordType.hpp>
#include <BabelWiresLib/Types/Record/recordFeatureUtils.hpp>
#include <BabelWiresLib/TypeSystem/standardFeatureWrappers.hpp>

namespace smf {
    class MidiMetadata : public babelwires::RecordType {
      public:
        PRIMITIVE_TYPE("midiMetadata", "MIDI Metadata", "5032054f-d1b7-4cb4-8bac-2de60a1bc078", 1);

        MidiMetadata();

        FEATURE_WRAPPER_BEGIN(MidiMetadata)
        FEATURE_WRAPPER_FIELD(Spec, GMSpecType)
        FEATURE_WRAPPER_FIELD_OPTIONAL(Tempo, seqwires::Tempo)
        FEATURE_WRAPPER_FIELD_OPTIONAL(Name, babelwires::StringType)
        FEATURE_WRAPPER_FIELD_OPTIONAL(CopyR, babelwires::StringType)
        FEATURE_WRAPPER_END()
    };

    class MidiMetadataFeature : public babelwires::SimpleValueFeature {
      public:
        MidiMetadataFeature();
    };
} // namespace smf

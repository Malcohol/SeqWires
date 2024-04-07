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

#include <BabelWiresLib/Types/String/stringFeature.hpp>
#include <BabelWiresLib/Features/recordWithOptionalsFeature.hpp>
#include <BabelWiresLib/Types/Record/recordType.hpp>

namespace seqwires {
    class TrackFeature;
}

namespace smf {
    class GmSpecTypeFeature : public babelwires::EnumWithCppEnumFeature<GMSpecType> {};

    class MidiMetadata : public babelwires::RecordType {
      public:
        PRIMITIVE_TYPE("midiMetadata", "MidiMetadata", "5032054f-d1b7-4cb4-8bac-2de60a1bc078", 1);

        MidiMetadata();

        GMSpecType::Value getSpec(const babelwires::TypeSystem& typeSystem, const babelwires::ValueHolder& spec) const;
        void setSpec(const babelwires::TypeSystem& typeSystem, babelwires::ValueHolder& value, GMSpecType::Value newSpec) const;
    };

    class MidiMetadataFeature : public babelwires::RecordWithOptionalsFeature {
      public:
        MidiMetadataFeature();

        const GmSpecTypeFeature* getSpecFeature() const;        
        const seqwires::TempoFeature* getTempoFeature() const;
        const babelwires::StringFeature* getCopyright() const;
        const babelwires::StringFeature* getSequenceName() const;

        GmSpecTypeFeature* getSpecFeature();

        seqwires::TempoFeature& getActivatedTempoFeature();
        babelwires::StringFeature& getActivatedCopyright();
        babelwires::StringFeature& getActivatedSequenceName();

      protected:
        GmSpecTypeFeature* m_specFeature;
        babelwires::StringFeature* m_sequenceName;
        babelwires::StringFeature* m_copyright;
        seqwires::TempoFeature* m_tempo;
    };
}

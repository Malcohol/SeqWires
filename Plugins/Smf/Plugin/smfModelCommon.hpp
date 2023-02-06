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

#include <BabelWiresLib/Features/stringFeature.hpp>
#include <BabelWiresLib/Features/recordWithOptionalsFeature.hpp>

namespace seqwires {
    class TrackFeature;
}

namespace smf {
    class GmSpecTypeFeature : public babelwires::EnumWithCppEnumFeature<GMSpecType> {};

    class MidiMetadata : public babelwires::RecordWithOptionalsFeature {
      public:
        MidiMetadata();

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

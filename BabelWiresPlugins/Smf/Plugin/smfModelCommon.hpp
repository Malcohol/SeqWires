/**
 * Representation of a Standard MIDI File as a tree of Features.
 * 
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include "BabelWiresLib/Features/stringFeature.hpp"
#include "SeqWiresLib/Features/tempoFeature.hpp"
#include "BabelWiresLib/Features/recordWithOptionalsFeature.hpp"

namespace seqwires {
    class TrackFeature;
}

namespace smf {
    class MidiMetadata : public babelwires::RecordWithOptionalsFeature {
      public:
        MidiMetadata();

        const seqwires::TempoFeature* getTempoFeature() const;
        const babelwires::StringFeature* getCopyright() const;
        const babelwires::StringFeature* getSequenceName() const;

        seqwires::TempoFeature& getActivatedTempoFeature();
        babelwires::StringFeature& getActivatedCopyright();
        babelwires::StringFeature& getActivatedSequenceName();

      protected:
        babelwires::StringFeature* m_sequenceName;
        babelwires::StringFeature* m_copyright;
        seqwires::TempoFeature* m_tempo;
    };
}

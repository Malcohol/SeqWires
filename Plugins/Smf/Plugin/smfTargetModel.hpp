/**
 * Representation of a Standard MIDI File as a tree of Features.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <Plugins/Smf/Plugin/midiTrackAndChannelArray.hpp>
#include <Plugins/Smf/Plugin/midiMetadata.hpp>

#include <BabelWiresLib/Features/arrayFeature.hpp>
#include <BabelWiresLib/Features/unionFeature.hpp>
#include <BabelWiresLib/FileFormat/fileFeature.hpp>
#include <BabelWiresLib/Types/Int/intFeature.hpp>
#include <BabelWiresLib/Types/Array/arrayType.hpp>

namespace seqwires {
    class Track;
    class TrackFeature;
} // namespace seqwires

namespace smf {
    namespace target {
        /// This is a UnionFeature in expectation of exploiting a structural difference
        /// between format 0 and 1 files. For now, the structure is the same.
        class SmfFormatFeature : public babelwires::UnionFeature {
          public:
            SmfFormatFeature();

            const MidiMetadataFeature& getMidiMetadata() const;
            MidiMetadataFeature& getMidiMetadata();

            // Convenience: Dispatches to the MidiTrackAndChannelArray.
            int getNumMidiTracks() const;
            const babelwires::ValueFeature& getMidiTrack(int index) const;

            Style getStyle() const override;

          protected:
            MidiMetadataFeature* m_metadata;
            babelwires::ValueFeature* m_midiTrackAndChannelArray;
        };

        /// Common to all formats of SmfFeature.
        class SmfFeature : public babelwires::FileFeature {
          public:
            SmfFeature(const babelwires::ProjectContext& projectContext);

            const SmfFormatFeature& getFormatFeature() const;
            SmfFormatFeature& getFormatFeature();

          protected:
            SmfFormatFeature* m_formatFeature;
        };
    } // namespace target
} // namespace smf

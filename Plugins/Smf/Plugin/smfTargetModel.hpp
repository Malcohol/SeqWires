/**
 * Representation of a Standard MIDI File as a tree of Features.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <Plugins/Smf/Plugin/midiMetadata.hpp>
#include <Plugins/Smf/Plugin/midiTrackAndChannelArray.hpp>

#include <BabelWiresLib/Features/arrayFeature.hpp>
#include <BabelWiresLib/Features/unionFeature.hpp>
#include <BabelWiresLib/FileFormat/fileFeature.hpp>
#include <BabelWiresLib/Types/Array/arrayType.hpp>
#include <BabelWiresLib/Types/Int/intFeature.hpp>
#include <BabelWiresLib/Types/RecordWithVariants/recordWithVariantsType.hpp>
#include <BabelWiresLib/Types/Record/recordFeatureWrapper.hpp>
#include <BabelWiresLib/TypeSystem/featureWrapper.hpp>

#include <BabelWiresLib/Features/valueFeature.hpp>

namespace seqwires {
    class Track;
    class TrackFeature;
} // namespace seqwires

namespace babelwires {}

namespace smf {

    class SmfType : public babelwires::RecordWithVariantsType {
      public:
        PRIMITIVE_TYPE("SmfType", "Standard MIDI File", "d4c70fb2-fb67-4e69-82ca-328ec242b0a8", 1);
        SmfType();

        FEATURE_WRAPPER_BEGIN(SmfType)
        FEATURE_WRAPPER_FIELD(Meta, MidiMetadata)
        FEATURE_WRAPPER_FIELD(Tracks, MidiTrackAndChannelArray);
        FEATURE_WRAPPER_END()
    };

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

            SmfType::FeatureWrapper<const babelwires::ValueFeature> getSmfTypeFeature() const;
            SmfType::FeatureWrapper<babelwires::ValueFeature> getSmfTypeFeature();

            const SmfFormatFeature& getFormatFeature() const;
            SmfFormatFeature& getFormatFeature();

          protected:
            SmfFormatFeature* m_formatFeature;
            babelwires::ValueFeature* m_smfTypeFeature;
        };
    } // namespace target
} // namespace smf

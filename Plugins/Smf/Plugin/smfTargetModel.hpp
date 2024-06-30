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
#include <BabelWiresLib/Instance/recordWithVariantsTypeInstance.hpp>
#include <BabelWiresLib/Instance/instanceOf.hpp>

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

        RECORD_WITH_VARIANTS_BEGIN(SmfType)
        DECLARE_INSTANCE_FIELD(Meta, MidiMetadata)
        DECLARE_INSTANCE_FIELD(Tracks, MidiTrackAndChannelArray);
        RECORD_WITH_VARIANTS_END()
    };

    namespace target {
        /// Common to all formats of SmfFeature.
        class SmfFeature : public babelwires::FileFeature {
          public:
            SmfFeature(const babelwires::ProjectContext& projectContext);

            SmfType::Instance<const babelwires::ValueFeature> getSmfTypeFeature() const;
            SmfType::Instance<babelwires::ValueFeature> getSmfTypeFeature();

          protected:
            babelwires::ValueFeature* m_smfTypeFeature;
        };
    } // namespace target
} // namespace smf

/**
 * Representation of a Standard MIDI File as a tree of Features.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <Plugins/Smf/Plugin/smfSequence.hpp>

#include <BabelWiresLib/Features/valueFeature.hpp>
#include <BabelWiresLib/FileFormat/fileFeature.hpp>
#include <BabelWiresLib/Types/File/fileType.hpp>
#include <BabelWiresLib/TypeSystem/primitiveType.hpp>
#include <BabelWiresLib/Instance/instance.hpp>

namespace smf {

    class SmfFile : public babelwires::FileType {
      public:
        PRIMITIVE_TYPE("SmfFile", "SMF File", "0656a8fa-dfbc-4a4c-a6d3-700eedbd86cd", 1);

        SmfFile();

        DECLARE_INSTANCE_BEGIN(SmfFile)
        DECLARE_INSTANCE_FIELD(SmfSeq, SmfSequence);
        DECLARE_INSTANCE_END()
    };

    class SmfFeature : public babelwires::FileFeature {
      public:
        SmfFeature(const babelwires::ProjectContext& projectContext);

        SmfSequence::ConstInstance getSmfSequence() const;
        SmfSequence::Instance getSmfSequence();

      protected:
        babelwires::ValueFeature* m_smfTypeFeature;
    };
} // namespace smf

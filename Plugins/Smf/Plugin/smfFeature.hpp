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

    class SmfFeature : public babelwires::FileFeature {
      public:
        SmfFeature(const babelwires::ProjectContext& projectContext);

        SmfSequence::ConstInstance getSmfSequence() const;
        SmfSequence::Instance getSmfSequence();

      protected:
        babelwires::ValueFeature* m_smfTypeFeature;
    };
} // namespace smf

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

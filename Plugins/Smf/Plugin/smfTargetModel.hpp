/**
 * Representation of a Standard MIDI File as a tree of Features.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <Plugins/Smf/Plugin/smfSequenceType.hpp>

#include <BabelWiresLib/FileFormat/fileFeature.hpp>
#include <BabelWiresLib/Features/valueFeature.hpp>

namespace smf {
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

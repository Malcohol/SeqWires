/**
 *
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <Plugins/Smf/Plugin/smfCommon.hpp>

#include <SeqWiresLib/Types/Track/trackInstance.hpp>
#include <SeqWiresLib/Types/Track/trackType.hpp>

#include <BabelWiresLib/Instance/instance.hpp>

namespace smf {
    /// Provide indexed based access to a record of 16 optional tracks.
    template <typename VALUE_TREE_NODE, typename TypeWithChannelOptionals>
    class InstanceWithChannelOptionals
        : public babelwires::InstanceCommonBase<VALUE_TREE_NODE, TypeWithChannelOptionals> {
      public:
        InstanceWithChannelOptionals(VALUE_TREE_NODE& valueFeature)
            : babelwires::InstanceCommonBase<VALUE_TREE_NODE, TypeWithChannelOptionals>(valueFeature) {}

        babelwires::ConstInstance<TypeOfTracks> getTrack(unsigned int channel) const {
            return babelwires::InstanceUtils::getChild(this->m_valueTreeNode,
                                                       TypeWithChannelOptionals::getTrackIdFromChannel(channel));
        }

        template <typename VALUE_FEATURE_M = VALUE_TREE_NODE>
        std::enable_if_t<!std::is_const_v<VALUE_FEATURE_M>,
                         babelwires::Instance<TypeOfTracks>>
        getTrack(unsigned int channel) {
            return babelwires::InstanceUtils::getChild(this->m_valueTreeNode,
                                                       TypeWithChannelOptionals::getTrackIdFromChannel(channel));
        }

        std::optional<babelwires::ConstInstance<TypeOfTracks>>
        tryGetTrack(unsigned int channel) const {
            if (const babelwires::ValueTreeNode* valueFeature = babelwires::InstanceUtils::tryGetChild(
                    this->m_valueTreeNode, TypeWithChannelOptionals::getTrackIdFromChannel(channel))) {
                return {*valueFeature};
            } else {
                return {};
            }
        }

        template <typename VALUE_FEATURE_M = VALUE_TREE_NODE>
        std::enable_if_t<!std::is_const_v<VALUE_FEATURE_M>,
                         babelwires::Instance<TypeOfTracks>>
        activateAndGetTrack(unsigned int channel) {
            return babelwires::InstanceUtils::activateAndGetChild(
                this->m_valueTreeNode, TypeWithChannelOptionals::getTrackIdFromChannel(channel));
        }
    };

} // namespace smf
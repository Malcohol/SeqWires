/**
 *
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <SeqWiresLib/Types/Track/trackInstance.hpp>
#include <SeqWiresLib/Types/Track/trackType.hpp>

#include <BabelWiresLib/Instance/instance.hpp>
#include <BabelWiresLib/Types/Record/recordType.hpp>

namespace smf {

    class RecordOfMidiTracks : public babelwires::RecordType {
      public:
        PRIMITIVE_TYPE("recordOfTracks", "Tracks", "44244cf8-8a05-4071-91a3-af77514ee03b", 1);

        RecordOfMidiTracks();

        static babelwires::ShortId getTrackIdFromChannel(unsigned int channel);
        static unsigned int getChannelFromTrackId(babelwires::ShortId id);

        using TypeOfTracks = seqwires::DefaultTrackType;

        /// It's easier to work with a custom instance than the one the standard DSL would produce.
        template <typename VALUE_FEATURE>
        class Instance : public babelwires::InstanceCommonBase<VALUE_FEATURE, RecordOfMidiTracks> {
          public:
            Instance(VALUE_FEATURE& valueFeature)
                : babelwires::InstanceCommonBase<VALUE_FEATURE, RecordOfMidiTracks>(valueFeature) {}

            babelwires::Instance<VALUE_FEATURE, TypeOfTracks> getTrack(unsigned int channel) const {
                return babelwires::InstanceUtils::getChild(this->m_valueFeature,
                                                           RecordOfMidiTracks::getTrackIdFromChannel(channel));
            }

            template <typename VALUE_FEATURE_M = VALUE_FEATURE>
            std::enable_if_t<!std::is_const_v<VALUE_FEATURE_M>,
                             babelwires::Instance<babelwires::ValueFeature, TypeOfTracks>>
            getTrack(unsigned int channel) {
                return babelwires::InstanceUtils::getChild(this->m_valueFeature,
                                                           RecordOfMidiTracks::getTrackIdFromChannel(channel));
            }

            std::optional<babelwires::Instance<const babelwires::ValueFeature, TypeOfTracks>>
            tryGetTrack(unsigned int channel) const {
                if (const babelwires::ValueFeature* valueFeature = babelwires::InstanceUtils::tryGetChild(
                        this->m_valueFeature, RecordOfMidiTracks::getTrackIdFromChannel(channel))) {
                    return {*valueFeature};
                } else {
                    return {};
                }
            }

            template <typename VALUE_FEATURE_M = VALUE_FEATURE>
            std::enable_if_t<!std::is_const_v<VALUE_FEATURE_M>,
                             babelwires::Instance<babelwires::ValueFeature, TypeOfTracks>>
                activateAndGetTrack(unsigned int channel) {
                return babelwires::InstanceUtils::activateAndGetChild(this->m_valueFeature, RecordOfMidiTracks::getTrackIdFromChannel(channel));
            }
        };
    };
} // namespace smf

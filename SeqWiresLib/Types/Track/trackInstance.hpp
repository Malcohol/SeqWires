/**
 *
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <SeqWiresLib/Types/Track/trackType.hpp>
#include <SeqWiresLib/Types/Track/track.hpp>

#include <BabelWiresLib/TypeSystem/valueHolder.hpp>
#include <BabelWiresLib/Instance/instanceTemplates.hpp>

namespace babelwires {
    template <typename VALUE_FEATURE, typename TRACK_TYPE>
        requires std::is_base_of_v<seqwires::TrackType, TRACK_TYPE>
    class Instance<VALUE_FEATURE, TRACK_TYPE> : public InstanceCommonBase<VALUE_FEATURE, TRACK_TYPE> {
      public:
        Instance(VALUE_FEATURE* valueFeature)
            : InstanceCommonBase<VALUE_FEATURE, TRACK_TYPE>(valueFeature) {}

        const typename seqwires::Track& get() const {
            assert(this->m_valueFeature);
            return this->m_valueFeature->getValue()->template is<seqwires::Track>();
        }
        template <typename VALUE_FEATURE_M = VALUE_FEATURE>
        std::enable_if_t<!std::is_const_v<VALUE_FEATURE_M>, void> set(babelwires::ValueHolder newValue) {
            assert(this->m_valueFeature);
            this->m_valueFeature->setValue(std::move(newValue));
        }
    };

} // namespace babelwires

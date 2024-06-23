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
#include <BabelWiresLib/TypeSystem/featureWrapper.hpp>

namespace babelwires {
    template <typename VALUE_FEATURE, typename T>
        requires std::is_base_of_v<seqwires::TrackType, T>
    class FeatureWrapper<VALUE_FEATURE, T> {
        VALUE_FEATURE* m_valueFeature;

      public:
        FeatureWrapper(VALUE_FEATURE* valueFeature)
            : m_valueFeature(valueFeature) {
            assert(!valueFeature || valueFeature->getType().template as<seqwires::TrackType>());
        }
        operator bool() const { return m_valueFeature; }

        typename seqwires::Track& get() const {
            assert(m_valueFeature);
            return m_valueFeature.getValue()->template is<seqwires::Track>();
        }
        template <typename VALUE_FEATURE_M = VALUE_FEATURE>
        std::enable_if_t<!std::is_const_v<VALUE_FEATURE_M>, void> set(babelwires::ValueHolder newValue) {
            assert(m_valueFeature);
            m_valueFeature->setValue(std::move(newValue));
        }
    };

} // namespace babelwires

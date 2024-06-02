/**
 * Representation of a Standard MIDI File as a tree of Features.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <Plugins/Smf/Plugin/gmSpec.hpp>

#include <SeqWiresLib/Features/tempoFeature.hpp>

#include <BabelWiresLib/Features/recordWithOptionalsFeature.hpp>
#include <BabelWiresLib/Types/Record/recordType.hpp>
#include <BabelWiresLib/Types/String/stringFeature.hpp>
#include <BabelWiresLib/Types/Record/recordFeatureUtils.hpp>
#include <BabelWiresLib/Types/Enum/enumValue.hpp>
#include <BabelWiresLib/Types/String/stringValue.hpp>

#include <optional>

namespace seqwires {
    class TrackFeature;
}

#define FIELD_INT_OPTIONAL(FIELD_NAME) \
  static std::optional<int> tryGet ## FIELD_NAME(const babelwires::ValueFeature& recordFeature) { \
    if (const auto* childFeature = babelwires::RecordFeatureUtils::tryGetChild(recordFeature, #FIELD_NAME)) { \
        const auto& intValue = childFeature->getValue()->is<babelwires::IntValue>(); \
        return intValue.get(); \
    } \
    return {}; \
  } \
  static void set ## FIELD_NAME(babelwires::ValueFeature& recordFeature, int newValue) { \
    auto& childFeature = babelwires::RecordFeatureUtils::activateAndGetChild(recordFeature, #FIELD_NAME); \
    childFeature.setValue(babelwires::IntValue(newValue)); \
  }

#define FIELD_STRING_OPTIONAL(FIELD_NAME) \
  static std::optional<std::string> tryGet ## FIELD_NAME(const babelwires::ValueFeature& recordValue) { \
    if (const auto* childFeature = babelwires::RecordFeatureUtils::tryGetChild(recordValue, #FIELD_NAME)) { \
        const auto& stringValue = childFeature->getValue()->is<babelwires::StringValue>(); \
        return stringValue.get(); \
    } \
    return {}; \
  } \
  static void set ## FIELD_NAME(babelwires::ValueFeature& recordValue, std::string newValue) { \
    auto& childFeature = babelwires::RecordFeatureUtils::activateAndGetChild(recordValue, #FIELD_NAME); \
    childFeature.setValue(babelwires::StringValue(newValue)); \
  }

#define FIELD_ENUM(FIELD_NAME, ENUM_TYPE) \
  static ENUM_TYPE::Value get ## FIELD_NAME(const babelwires::ValueFeature& recordFeature) { \
    const auto& childFeature = babelwires::RecordFeatureUtils::getChild(recordFeature, #FIELD_NAME); \
    const babelwires::EnumValue& enumValue = childFeature.getValue()->is<babelwires::EnumValue>(); \
    const ENUM_TYPE& enumType = childFeature.getType().is<ENUM_TYPE>(); \
    return enumType.getValueFromIdentifier(enumValue.get()); \
  } \
  static void set ## FIELD_NAME(babelwires::ValueFeature& recordFeature, ENUM_TYPE::Value newValue) { \
    auto& childFeature = babelwires::RecordFeatureUtils::getChild(recordFeature, #FIELD_NAME); \
    const ENUM_TYPE& enumType = childFeature.getType().is<ENUM_TYPE>(); \
    childFeature.setValue(babelwires::EnumValue(enumType.getIdentifierFromValue(newValue))); \
  }

namespace smf {
    class GmSpecTypeFeature : public babelwires::EnumWithCppEnumFeature<GMSpecType> {};

    class MidiMetadata : public babelwires::RecordType {
      public:
        PRIMITIVE_TYPE("midiMetadata", "MidiMetadata", "5032054f-d1b7-4cb4-8bac-2de60a1bc078", 1);

        MidiMetadata();

        FIELD_ENUM(Spec, GMSpecType)
        FIELD_INT_OPTIONAL(Tempo)
        FIELD_STRING_OPTIONAL(Name)
        FIELD_STRING_OPTIONAL(CopyR)
    };

    class MidiMetadataFeature : public babelwires::SimpleValueFeature {
      public:
        MidiMetadataFeature();
    };
} // namespace smf

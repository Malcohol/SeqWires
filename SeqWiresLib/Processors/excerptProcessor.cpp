/**
 * A processor which extracts a section of sequence data from a track.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <SeqWiresLib/Features/durationFeature.hpp>
#include <SeqWiresLib/Functions/excerptFunction.hpp>
#include <SeqWiresLib/Processors/excerptProcessor.hpp>
#include <SeqWiresLib/Types/Track/trackInstance.hpp>

#include <BabelWiresLib/Features/arrayFeature.hpp>
#include <BabelWiresLib/Features/rootFeature.hpp>
#include <BabelWiresLib/Types/Rational/rationalValue.hpp>

#include <BabelWiresLib/Project/projectContext.hpp>
#include <BabelWiresLib/TypeSystem/typeSystem.hpp>
#include <BabelWiresLib/Types/Array/arrayType.hpp>
#include <BabelWiresLib/Types/Array/arrayTypeConstructor.hpp>
#include <BabelWiresLib/Types/Array/arrayValue.hpp>

#include <Common/Identifiers/registeredIdentifier.hpp>

#include <algorithm>
#include <array>
#include <execution>
#include <numeric>

namespace {
    babelwires::TypeRef getParallelArray(babelwires::TypeRef&& entryType) {
        return babelwires::TypeRef{
            babelwires::ArrayTypeConstructor::getThisIdentifier(),
            {{std::move(entryType)}, {babelwires::IntValue(1), babelwires::IntValue(16), babelwires::IntValue(1)}}};
    }

    std::vector<babelwires::RecordType::Field>&& addArray(std::vector<babelwires::RecordType::Field>&& commonData,
                                                          babelwires::ShortId arrayId, babelwires::TypeRef entryType) {
        commonData.emplace_back(babelwires::RecordType::Field{arrayId, getParallelArray(std::move(entryType))});
        return std::move(commonData);
    }
} // namespace

babelwires::ParallelValueProcessorInputBase::ParallelValueProcessorInputBase(std::vector<RecordType::Field> commonData,
                                                                             ShortId arrayId, TypeRef entryType)
    : RecordType(addArray(std::move(commonData), arrayId, entryType)) {}

babelwires::ParallelValueProcessorOutputBase::ParallelValueProcessorOutputBase(ShortId arrayId, TypeRef entryType)
    : RecordType({{arrayId, getParallelArray(std::move(entryType))}}) {}

babelwires::ParallelValueProcessor::ParallelValueProcessor(const ProjectContext& projectContext,
                                                           const TypeRef& parallelInput, const TypeRef& parallelOutput)
    : ValueProcessor(projectContext, parallelInput, parallelOutput) {
#ifndef NDEBUG
    const auto* inputType = parallelInput.resolve(projectContext.m_typeSystem).as<ParallelValueProcessorInputBase>();
    assert(inputType && "The ParallelValueProcessor input type should be a ParallelValueProcessorInputBase");
    const auto* outputType = parallelOutput.resolve(projectContext.m_typeSystem).as<ParallelValueProcessorOutputBase>();
    assert(outputType && "The ParallelValueProcessor output type should be a ParallelValueProcessorOutputBase");
    assert(inputType->getFields().size() >= 1);
    assert(outputType->getFields().size() == 1);
    // Note: The two IDs don't have to have the same distinguisher and can be registered separately.
    assert((inputType->getFields()[inputType->getFields().size() - 1].m_identifier ==
            outputType->getFields()[0].m_identifier) &&
           "The ParallelValueProcessor input and output IDs must agree");
#endif
}

void babelwires::ParallelValueProcessor::processValue(UserLogger& userLogger, const ValueFeature& inputFeature,
                                                      ValueFeature& outputFeature) const {
    bool shouldProcessAll = false;
    // Iterate through all features _except_ for the array, look for changes to the common input.
    for (unsigned int i = 0; i < inputFeature.getNumFeatures() - 1; ++i) {
        if (inputFeature.getFeature(i)->isChanged(Feature::Changes::SomethingChanged)) {
            shouldProcessAll = true;
        }
    }

    const auto& arrayInputFeature = inputFeature.getFeature(inputFeature.getNumFeatures() - 1)->is<ValueFeature>();
    auto& arrayOutputFeature = outputFeature.getFeature(outputFeature.getNumFeatures() - 1)->is<ValueFeature>();

    if (arrayInputFeature.isChanged(Feature::Changes::StructureChanged)) {
        // TODO: This is very inefficient in cases where a single entry has been added or removed.
        // In the old feature system I was able to maintain a mapping between input and output entries
        // to avoid this inefficiency. Perhaps that can be done with values too.
        shouldProcessAll = true;
        InstanceUtils::setArraySize(arrayOutputFeature, arrayInputFeature.getNumFeatures());
    }

    struct EntryData {
        EntryData(const TypeSystem& typeSystem, unsigned int index, const ValueFeature& inputEntry, const ValueFeature& outputEntry)
            : m_index(index)
            , m_inputEntry(inputEntry)
            , m_outputEntry(std::make_unique<SimpleValueFeature>(typeSystem, outputEntry.getTypeRef()))
            {
                m_outputEntry->setValue(outputEntry.getValue());
            }

        const unsigned int m_index;
        const ValueFeature& m_inputEntry;
        std::unique_ptr<SimpleValueFeature> m_outputEntry;
        std::string m_failureString;
    };
    std::vector<EntryData> entriesToProcess;
    entriesToProcess.reserve(s_maxParallelFeatures);

    const TypeSystem& typeSystem = inputFeature.getTypeSystem();

    for (unsigned int i = 0; i < arrayInputFeature.getNumFeatures(); ++i) {
        const ValueFeature& inputEntry = arrayInputFeature.getFeature(i)->is<ValueFeature>();
        if (shouldProcessAll || arrayInputFeature.getFeature(i)->isChanged(Feature::Changes::SomethingChanged)) {
            ValueFeature& outputEntry = arrayOutputFeature.getFeature(i)->is<ValueFeature>();
            entriesToProcess.emplace_back(EntryData{typeSystem, i, inputEntry, outputEntry});
        }
    }

    bool isFailed = false;
    std::for_each(
#ifndef __APPLE__
        std::execution::par,
#endif
        entriesToProcess.begin(), entriesToProcess.end(),
        [this, &inputFeature, &userLogger, &isFailed](EntryData& data) {
            try {
                processEntry(userLogger, inputFeature, data.m_inputEntry, *(data.m_outputEntry));
            } catch (const BaseException& e) {
                data.m_failureString = e.what();
                isFailed = true;
            }
        });
    
    if (isFailed) {
        // TODO: Need a more precise way to signal failure.
        ModelException compositeException;
        const char* newline = "";
        for (const auto& entry : entriesToProcess) {
            if (!entry.m_failureString.empty()) {
                compositeException << newline << "Failure processing entry " << entry.m_index << ": "
                                   << entry.m_failureString;
                newline = "\n";
            }
        }
        throw compositeException;
    }

    ArrayValue newOutput = arrayOutputFeature.getValue()->is<ArrayValue>();
    for (EntryData& data : entriesToProcess) {
        newOutput.setValue(data.m_index, data.m_outputEntry->getValue());
    }
    arrayOutputFeature.setValue(newOutput);
}

seqwires::ExcerptProcessorInput::ExcerptProcessorInput()
    : babelwires::ParallelValueProcessorInputBase(
          {{BW_SHORT_ID("Start", "Start", "4b95f5db-a542-4660-a8db-97d3a5f831ca"), Duration::getThisIdentifier()},
           {BW_SHORT_ID("Duratn", "Duration", "d83ebbc2-1492-4578-a3b8-4969eb6a2042"), Duration::getThisIdentifier()}},
          ExcerptProcessor::getCommonArrayId(), seqwires::DefaultTrackType::getThisIdentifier()) {}

seqwires::ExcerptProcessorOutput::ExcerptProcessorOutput()
    : babelwires::ParallelValueProcessorOutputBase(ExcerptProcessor::getCommonArrayId(),
                                                   seqwires::DefaultTrackType::getThisIdentifier()) {}

babelwires::ShortId seqwires::ExcerptProcessor::getCommonArrayId() {
    return BW_SHORT_ID("Tracks", "Tracks", "983b3bcb-7086-4791-8e18-d8c7550d45d3");
}

seqwires::ExcerptProcessor::ExcerptProcessor(const babelwires::ProjectContext& projectContext)
    : babelwires::ParallelValueProcessor(projectContext, ExcerptProcessorInput::getThisIdentifier(), ExcerptProcessorOutput::getThisIdentifier()) {}

void seqwires::ExcerptProcessor::processEntry(babelwires::UserLogger& userLogger, const babelwires::ValueFeature& inputFeature,
                          const babelwires::ValueFeature& inputEntry, babelwires::ValueFeature& outputEntry)  const {
    ExcerptProcessorInput::ConstInstance input{inputFeature};                        
    babelwires::ConstInstance<TrackType> entryIn{inputEntry};
    babelwires::Instance<TrackType> entryOut{outputEntry};

    entryOut.set(getTrackExcerpt(entryIn.get(), input.getStart().get(), input.getDuratn().get()));
}

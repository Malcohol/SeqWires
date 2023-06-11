/**
 * An abstraction for Enums of percussion instruments which can map to pitch values.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <SeqWiresLib/Percussion/percussionSetWithPitchMap.hpp>

#include <SeqWiresLib/Percussion/percussionTypeTag.hpp>

#include <unordered_set>

class seqwires::PercussionSetWithPitchMap::ComplexConstructorArguments {
  public:
    babelwires::EnumType::ValueSet m_enumValues;
    std::unordered_map<seqwires::Pitch, babelwires::ShortId> m_pitchToInstrument;
    std::unordered_map<babelwires::ShortId, seqwires::Pitch> m_instrumentToPitch;
    int m_indexOfDefaultValue = -1;

    ComplexConstructorArguments(const InstrumentBlock& instrumentBlock, seqwires::Pitch pitchOfDefaultValue) {
        addInstruments(instrumentBlock, pitchOfDefaultValue);
    }

    ComplexConstructorArguments(const std::vector<InstrumentBlock>& instrumentBlocks,
                                seqwires::Pitch pitchOfDefaultValue) {
        std::for_each(instrumentBlocks.begin(), instrumentBlocks.end(),
                      [this, pitchOfDefaultValue](const auto& b) { addInstruments(b, pitchOfDefaultValue); });
    }

    void addInstruments(const InstrumentBlock& instrumentBlock, seqwires::Pitch pitchOfDefaultValue) {
        seqwires::Pitch pitch = instrumentBlock.m_pitchOfLowestInstrument;
        for (auto v : instrumentBlock.m_instruments) {
            assert(((v.index() == 0) || (instrumentBlock.m_builtInPercussionInstruments)) &&
                   "You must provide the m_builtInPercussionInstruments if you use the value branch of the variant");
            const babelwires::ShortId id =
                (v.index() == 0)
                    ? std::get<0>(v)
                    : instrumentBlock.m_builtInPercussionInstruments->getIdentifierFromValue(std::get<1>(v));
            if (m_alreadySeen.find(id) == m_alreadySeen.end()) {
                if (pitchOfDefaultValue == pitch) {
                    m_indexOfDefaultValue = m_enumValues.size();
                }
                m_instrumentToPitch[id] = pitch;
                m_enumValues.emplace_back(id);
                m_alreadySeen.insert(id);
            }
            assert((m_pitchToInstrument.find(pitch) == m_pitchToInstrument.end()) &&
                   "Duplicate pitch probably because of overlapping blocks");
            m_pitchToInstrument[pitch] = id;
            ++pitch;
        }
    }

    ~ComplexConstructorArguments() {
        assert((m_indexOfDefaultValue != -1) &&
               "The default pitch was not found or was the non-lowest pitch of duplicate instrument");
    }

  private:
    std::unordered_set<babelwires::ShortId> m_alreadySeen;
};

seqwires::PercussionSetWithPitchMap::PercussionSetWithPitchMap(ComplexConstructorArguments&& removeDuplicates)
    : EnumType(std::move(removeDuplicates.m_enumValues), removeDuplicates.m_indexOfDefaultValue)
    , m_pitchToInstrument(std::move(removeDuplicates.m_pitchToInstrument))
    , m_instrumentToPitch(std::move(removeDuplicates.m_instrumentToPitch)) {
    addTag(percussionTypeTag());
}

seqwires::PercussionSetWithPitchMap::PercussionSetWithPitchMap(InstrumentBlock instruments,
                                                               seqwires::Pitch pitchOfDefaultInstrument)
    : PercussionSetWithPitchMap(ComplexConstructorArguments(instruments, pitchOfDefaultInstrument)) {}

seqwires::PercussionSetWithPitchMap::PercussionSetWithPitchMap(std::vector<InstrumentBlock> instruments,
                                                               seqwires::Pitch pitchOfDefaultInstrument)
    : PercussionSetWithPitchMap(ComplexConstructorArguments(instruments, pitchOfDefaultInstrument)) {}

std::optional<seqwires::Pitch>
seqwires::PercussionSetWithPitchMap::tryGetPitchFromInstrument(babelwires::ShortId identifier) const {
    const auto it = m_instrumentToPitch.find(identifier);
    if (it != m_instrumentToPitch.end()) {
        return it->second;
    }
    return {};
}

std::optional<babelwires::ShortId>
seqwires::PercussionSetWithPitchMap::tryGetInstrumentFromPitch(seqwires::Pitch pitch) const {
    const auto it = m_pitchToInstrument.find(pitch);
    if (it != m_pitchToInstrument.end()) {
        return it->second;
    }
    return {};
}

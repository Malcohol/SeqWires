/**
 *
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <BabelWiresPlugins/Smf/Plugin/Percussion/standardPercussionSets.hpp>

#include <BabelWiresPlugins/Smf/Plugin/Percussion/gm2AnalogPercussionSet.hpp>
#include <BabelWiresPlugins/Smf/Plugin/Percussion/gm2BrushPercussionSet.hpp>
#include <BabelWiresPlugins/Smf/Plugin/Percussion/gm2ElectronicPercussionSet.hpp>
#include <BabelWiresPlugins/Smf/Plugin/Percussion/gm2JazzPercussionSet.hpp>
#include <BabelWiresPlugins/Smf/Plugin/Percussion/gm2OrchestraPercussionSet.hpp>
#include <BabelWiresPlugins/Smf/Plugin/Percussion/gm2PowerPercussionSet.hpp>
#include <BabelWiresPlugins/Smf/Plugin/Percussion/gm2RoomPercussionSet.hpp>
#include <BabelWiresPlugins/Smf/Plugin/Percussion/gm2SFXPercussionSet.hpp>
#include <BabelWiresPlugins/Smf/Plugin/Percussion/gm2StandardPercussionSet.hpp>
#include <BabelWiresPlugins/Smf/Plugin/Percussion/gmPercussionSet.hpp>
#include <BabelWiresPlugins/Smf/Plugin/Percussion/gsStandard1PercussionSet.hpp>
#include <BabelWiresPlugins/Smf/Plugin/Percussion/xgRockPercussionSet.hpp>
#include <BabelWiresPlugins/Smf/Plugin/Percussion/xgRoomPercussionSet.hpp>
#include <BabelWiresPlugins/Smf/Plugin/Percussion/xgStandard1PercussionSet.hpp>
#include <BabelWiresPlugins/Smf/Plugin/Percussion/xgElectroPercussionSet.hpp>

#include <BabelWiresLib/Project/projectContext.hpp>
#include <BabelWiresLib/TypeSystem/typeSystem.hpp>

#include <cassert>

smf::StandardPercussionSets::StandardPercussionSets(const babelwires::ProjectContext& projectContext) {
    m_knownSets[GM_PERCUSSION_SET] =
        &projectContext.m_typeSystem.getRegisteredEntry(smf::GMPercussionSet::getThisIdentifier())
             .is<smf::PercussionSet>();
    m_knownSets[GM2_STANDARD_PERCUSSION_SET] =
        &projectContext.m_typeSystem.getRegisteredEntry(smf::GM2StandardPercussionSet::getThisIdentifier())
             .is<smf::PercussionSet>();
    m_knownSets[GM2_ANALOG_PERCUSSION_SET] =
        &projectContext.m_typeSystem.getRegisteredEntry(smf::GM2AnalogPercussionSet::getThisIdentifier())
             .is<smf::PercussionSet>();
    m_knownSets[GM2_BRUSH_PERCUSSION_SET] =
        &projectContext.m_typeSystem.getRegisteredEntry(smf::GM2BrushPercussionSet::getThisIdentifier())
             .is<smf::PercussionSet>();
    m_knownSets[GM2_ELECTRONIC_PERCUSSION_SET] =
        &projectContext.m_typeSystem.getRegisteredEntry(smf::GM2ElectronicPercussionSet::getThisIdentifier())
             .is<smf::PercussionSet>();
    m_knownSets[GM2_JAZZ_PERCUSSION_SET] =
        &projectContext.m_typeSystem.getRegisteredEntry(smf::GM2JazzPercussionSet::getThisIdentifier())
             .is<smf::PercussionSet>();
    m_knownSets[GM2_ORCHESTRA_PERCUSSION_SET] =
        &projectContext.m_typeSystem.getRegisteredEntry(smf::GM2OrchestraPercussionSet::getThisIdentifier())
             .is<smf::PercussionSet>();
    m_knownSets[GM2_POWER_PERCUSSION_SET] =
        &projectContext.m_typeSystem.getRegisteredEntry(smf::GM2PowerPercussionSet::getThisIdentifier())
             .is<smf::PercussionSet>();
    m_knownSets[GM2_ROOM_PERCUSSION_SET] =
        &projectContext.m_typeSystem.getRegisteredEntry(smf::GM2RoomPercussionSet::getThisIdentifier())
             .is<smf::PercussionSet>();
    m_knownSets[GM2_SFX_PERCUSSION_SET] =
        &projectContext.m_typeSystem.getRegisteredEntry(smf::GM2SFXPercussionSet::getThisIdentifier())
             .is<smf::PercussionSet>();
    m_knownSets[GS_STANDARD_1_PERCUSSION_SET] =
        &projectContext.m_typeSystem.getRegisteredEntry(smf::GsStandard1PercussionSet::getThisIdentifier())
             .is<smf::PercussionSet>();
    m_knownSets[XG_STANDARD_1_PERCUSSION_SET] =
        &projectContext.m_typeSystem.getRegisteredEntry(smf::XgStandard1PercussionSet::getThisIdentifier())
             .is<smf::PercussionSet>();
    m_knownSets[XG_ROOM_PERCUSSION_SET] =
        &projectContext.m_typeSystem.getRegisteredEntry(smf::XgRoomPercussionSet::getThisIdentifier())
             .is<smf::PercussionSet>();
    m_knownSets[XG_ROCK_PERCUSSION_SET] =
        &projectContext.m_typeSystem.getRegisteredEntry(smf::XgRockPercussionSet::getThisIdentifier())
             .is<smf::PercussionSet>();
    m_knownSets[XG_ELECTRO_PERCUSSION_SET] =
        &projectContext.m_typeSystem.getRegisteredEntry(smf::XgElectroPercussionSet::getThisIdentifier())
             .is<smf::PercussionSet>();
}

const smf::PercussionSet* smf::StandardPercussionSets::getDefaultPercussionSet(GMSpecType::Value gmSpec,
                                                                               int channelNumber) {
    switch (gmSpec) {
        case GMSpecType::Value::GM:
            if (channelNumber == 9) {
                return m_knownSets[GM_PERCUSSION_SET];
            }
        case GMSpecType::Value::XG:
            if (channelNumber == 9) {
                return m_knownSets[XG_STANDARD_1_PERCUSSION_SET];
            }
        case GMSpecType::Value::GS:
            if (channelNumber == 9) {
                return m_knownSets[GS_STANDARD_1_PERCUSSION_SET];
            }
        case GMSpecType::Value::GM2:
            if (channelNumber == 9) {
                return m_knownSets[GM2_STANDARD_PERCUSSION_SET];
            }
        case GMSpecType::Value::NONE:
        default:
            break;
    }
    return nullptr;
}

const smf::PercussionSet*
smf::StandardPercussionSets::getPercussionSetFromChannelSetupInfo(GMSpecType::Value gmSpec,
                                                                  ChannelSetupInfo channelSetupInfo) {
    if (gmSpec == GMSpecType::Value::GM2) {
        if (channelSetupInfo.m_bankMSB == 0x78) {
            switch (channelSetupInfo.m_program) {
                case 1:
                    return m_knownSets[GM2_STANDARD_PERCUSSION_SET];
                case 9:
                    return m_knownSets[GM2_ROOM_PERCUSSION_SET];
                case 17:
                    return m_knownSets[GM2_POWER_PERCUSSION_SET];
                case 25:
                    return m_knownSets[GM2_ELECTRONIC_PERCUSSION_SET];
                case 26:
                    return m_knownSets[GM2_ANALOG_PERCUSSION_SET];
                case 33:
                    return m_knownSets[GM2_JAZZ_PERCUSSION_SET];
                case 41:
                    return m_knownSets[GM2_BRUSH_PERCUSSION_SET];
                case 49:
                    return m_knownSets[GM2_ORCHESTRA_PERCUSSION_SET];
                case 57:
                    return m_knownSets[GM2_SFX_PERCUSSION_SET];
            }
            return m_knownSets[GM2_STANDARD_PERCUSSION_SET];
        } else if (channelSetupInfo.m_bankMSB == 0x79) {
            // Melody
            return nullptr;
        } else {
            // Not specified by GM2
        }
    } else if (gmSpec == GMSpecType::Value::XG) {
        if (channelSetupInfo.m_bankMSB == 0x7f) {
            // Percussion
            switch (channelSetupInfo.m_program) {
                case 9:
                    return m_knownSets[XG_ROOM_PERCUSSION_SET];
                case 17:
                    return m_knownSets[XG_ROCK_PERCUSSION_SET];
                case 25:
                    return m_knownSets[XG_ELECTRO_PERCUSSION_SET];
                case 1:
                case 2: /* Also used for Standard 2 */
                default:
                    return m_knownSets[XG_STANDARD_1_PERCUSSION_SET];
            }
        } else if (channelSetupInfo.m_bankMSB == 0x7e) {
            // SFX Percussion
        } else if (channelSetupInfo.m_bankMSB == 0x00) {
            // Voice
            return nullptr;
        } else if (channelSetupInfo.m_bankMSB == 0x40) {
            // SFX
            return nullptr;
        } else {
            // Not specified by XG
        }
    } else if (gmSpec == GMSpecType::Value::GS) {
        if (channelSetupInfo.m_gsPartMode == 1) {
            // Percussion mode 1
            return m_knownSets[GS_STANDARD_1_PERCUSSION_SET];
        } else if (channelSetupInfo.m_gsPartMode == 2) {
            // Percussion mode 2
            return m_knownSets[GS_STANDARD_1_PERCUSSION_SET];
        } else {
            // Not a percussion voice
            return nullptr;
        }
    }
    return nullptr;
}

void smf::StandardPercussionSets::ensureInstrumentSets() {
    if (m_instrumentSets[0].size() > 0) {
        return;
    }
    for (int i = 0; i < NUM_KNOWN_PERCUSSION_SETS; ++i) {
        const babelwires::Enum::EnumValues& instruments = m_knownSets[i]->getEnumValues();
        std::for_each(instruments.begin(), instruments.end(),
                      [this, i](auto instrument) { m_instrumentSets[i].insert(instrument); });
    }
}

const smf::PercussionSet* smf::StandardPercussionSets::getBestPercussionSetInRange(
    int startIndex, int endIndex, const std::unordered_set<babelwires::Identifier>& instrumentsInUse,
    std::unordered_set<babelwires::Identifier>& excludedInstrumentsOut) {
    ensureInstrumentSets();

    int bestFit = -1;
    std::unordered_set<babelwires::Identifier> candidateExclusions;

    for (int i = startIndex; i <= endIndex; ++i) {
        candidateExclusions.clear();
        for (auto instrument : instrumentsInUse) {
            if (m_instrumentSets[i].find(instrument) == m_instrumentSets[i].end()) {
                candidateExclusions.insert(instrument);
            }
        }
        if ((bestFit == -1) || (candidateExclusions.size() < excludedInstrumentsOut.size())) {
            bestFit = i;
            excludedInstrumentsOut.swap(candidateExclusions);
        }
    }
    return m_knownSets[bestFit];
}

const smf::PercussionSet*
smf::StandardPercussionSets::getBestPercussionSet(GMSpecType::Value gmSpec, int channelNumber,
                                                  const std::unordered_set<babelwires::Identifier>& instrumentsInUse,
                                                  std::unordered_set<babelwires::Identifier>& excludedInstrumentsOut) {
    // TODO XG / chan 10.
    if (channelNumber != 9) {
        excludedInstrumentsOut = instrumentsInUse;
        return nullptr;
    }

    if (gmSpec == GMSpecType::Value::GM) {
        return getBestPercussionSetInRange(GM_PERCUSSION_SET, GM_PERCUSSION_SET, instrumentsInUse,
                                           excludedInstrumentsOut);
    } else if (gmSpec == GMSpecType::Value::GM2) {
        return getBestPercussionSetInRange(GM2_SETS_START, GM2_SETS_END, instrumentsInUse, excludedInstrumentsOut);
    } else if (gmSpec == GMSpecType::Value::GS) {
        return getBestPercussionSetInRange(GS_SETS_START, GS_SETS_END, instrumentsInUse, excludedInstrumentsOut);
    } else if (gmSpec == GMSpecType::Value::XG) {
        return getBestPercussionSetInRange(XG_SETS_START, XG_SETS_END, instrumentsInUse, excludedInstrumentsOut);
    }
    return nullptr;
}

smf::StandardPercussionSets::KnownPercussionSets
smf::StandardPercussionSets::getKnownPercussionSetFromPercussionSet(const PercussionSet* percussionSet) {
    const auto it = std::find(m_knownSets.begin(), m_knownSets.end(), percussionSet);
    assert((it != m_knownSets.end()) && "Percussion set not known");
    return static_cast<KnownPercussionSets>(it - m_knownSets.begin());
}

std::optional<smf::StandardPercussionSets::ChannelSetupInfo>
smf::StandardPercussionSets::getChannelSetupInfoFromKnownPercussionSet(KnownPercussionSets percussionSet) {
    switch (percussionSet) {
        case GM_PERCUSSION_SET:
            return {};
        case GM2_STANDARD_PERCUSSION_SET:
            return {{0x78, 0, 1, 0}};
        case GM2_ROOM_PERCUSSION_SET:
            return {{0x78, 0, 9, 0}};
        case GM2_POWER_PERCUSSION_SET:
            return {{0x78, 0, 17, 0}};
        case GM2_ELECTRONIC_PERCUSSION_SET:
            return {{0x78, 0, 25, 0}};
        case GM2_ANALOG_PERCUSSION_SET:
            return {{0x78, 0, 26, 0}};
        case GM2_JAZZ_PERCUSSION_SET:
            return {{0x78, 0, 33, 0}};
        case GM2_BRUSH_PERCUSSION_SET:
            return {{0x78, 0, 41, 0}};
        case GM2_ORCHESTRA_PERCUSSION_SET:
            return {{0x78, 0, 49, 0}};
        case GM2_SFX_PERCUSSION_SET:
            return {{0x78, 0, 57, 0}};
        case GS_STANDARD_1_PERCUSSION_SET:
            return {{0x02, 0, 1, 1}};
        case XG_STANDARD_1_PERCUSSION_SET:
            return {{0x7f, 0, 1, 0}};
        case XG_ROOM_PERCUSSION_SET:
            return {{0x7f, 0, 9, 0}};
        case XG_ROCK_PERCUSSION_SET:
            return {{0x7f, 0, 17, 0}};
        case XG_ELECTRO_PERCUSSION_SET:
            return {{0x7f, 0, 25, 0}};
        default:
        case NOT_PERCUSSION:
            return {};
    }
}

std::optional<smf::StandardPercussionSets::ChannelSetupInfo>
smf::StandardPercussionSets::getChannelSetupInfoFromPercussionSet(const PercussionSet* percussionSet) {
    if (percussionSet) {
        const KnownPercussionSets knownPercussionSet = getKnownPercussionSetFromPercussionSet(percussionSet);
        return getChannelSetupInfoFromKnownPercussionSet(knownPercussionSet);
    } else {
        return {};
    }
}

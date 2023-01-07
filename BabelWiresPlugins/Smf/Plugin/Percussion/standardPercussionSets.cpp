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
#include <BabelWiresPlugins/Smf/Plugin/Percussion/gs808909PercussionSet.hpp>
#include <BabelWiresPlugins/Smf/Plugin/Percussion/gsBrushPercussionSet.hpp>
#include <BabelWiresPlugins/Smf/Plugin/Percussion/gsElectronicPercussionSet.hpp>
#include <BabelWiresPlugins/Smf/Plugin/Percussion/gsJazzPercussionSet.hpp>
#include <BabelWiresPlugins/Smf/Plugin/Percussion/gsOrchestraPercussionSet.hpp>
#include <BabelWiresPlugins/Smf/Plugin/Percussion/gsPowerPercussionSet.hpp>
#include <BabelWiresPlugins/Smf/Plugin/Percussion/gsRoomPercussionSet.hpp>
#include <BabelWiresPlugins/Smf/Plugin/Percussion/gsStandard1PercussionSet.hpp>
#include <BabelWiresPlugins/Smf/Plugin/Percussion/xgAnalogPercussionSet.hpp>
#include <BabelWiresPlugins/Smf/Plugin/Percussion/xgBrushPercussionSet.hpp>
#include <BabelWiresPlugins/Smf/Plugin/Percussion/xgClassicPercussionSet.hpp>
#include <BabelWiresPlugins/Smf/Plugin/Percussion/xgElectroPercussionSet.hpp>
#include <BabelWiresPlugins/Smf/Plugin/Percussion/xgJazzPercussionSet.hpp>
#include <BabelWiresPlugins/Smf/Plugin/Percussion/xgRockPercussionSet.hpp>
#include <BabelWiresPlugins/Smf/Plugin/Percussion/xgRoomPercussionSet.hpp>
#include <BabelWiresPlugins/Smf/Plugin/Percussion/xgSFX1PercussionSet.hpp>
#include <BabelWiresPlugins/Smf/Plugin/Percussion/xgSFX2PercussionSet.hpp>
#include <BabelWiresPlugins/Smf/Plugin/Percussion/xgStandard1PercussionSet.hpp>

#include <BabelWiresLib/Project/projectContext.hpp>
#include <BabelWiresLib/TypeSystem/typeSystem.hpp>

#include <cassert>

smf::StandardPercussionSets::StandardPercussionSets(const babelwires::ProjectContext& projectContext) {
#define DECLARE_PERCUSSION_SET(ENUM, CLASS)                                                                            \
    m_knownSets[ENUM] =                                                                                                \
        &projectContext.m_typeSystem.getRegisteredEntry(CLASS::getThisIdentifier()).is<smf::PercussionSet>();

    DECLARE_PERCUSSION_SET(GM_PERCUSSION_SET, smf::GMPercussionSet)
    DECLARE_PERCUSSION_SET(GM2_STANDARD_PERCUSSION_SET, smf::GM2StandardPercussionSet)
    DECLARE_PERCUSSION_SET(GM2_ANALOG_PERCUSSION_SET, smf::GM2AnalogPercussionSet)
    DECLARE_PERCUSSION_SET(GM2_BRUSH_PERCUSSION_SET, smf::GM2BrushPercussionSet)
    DECLARE_PERCUSSION_SET(GM2_ELECTRONIC_PERCUSSION_SET, smf::GM2ElectronicPercussionSet)
    DECLARE_PERCUSSION_SET(GM2_JAZZ_PERCUSSION_SET, smf::GM2JazzPercussionSet)
    DECLARE_PERCUSSION_SET(GM2_ORCHESTRA_PERCUSSION_SET, smf::GM2OrchestraPercussionSet)
    DECLARE_PERCUSSION_SET(GM2_POWER_PERCUSSION_SET, smf::GM2PowerPercussionSet)
    DECLARE_PERCUSSION_SET(GM2_ROOM_PERCUSSION_SET, smf::GM2RoomPercussionSet)
    DECLARE_PERCUSSION_SET(GM2_SFX_PERCUSSION_SET, smf::GM2SFXPercussionSet)
    DECLARE_PERCUSSION_SET(GS_STANDARD_1_PERCUSSION_SET, smf::GsStandard1PercussionSet)
    DECLARE_PERCUSSION_SET(GS_ROOM_PERCUSSION_SET, smf::GsRoomPercussionSet)
    DECLARE_PERCUSSION_SET(GS_POWER_PERCUSSION_SET, smf::GsPowerPercussionSet)
    DECLARE_PERCUSSION_SET(GS_ELECTRONIC_PERCUSSION_SET, smf::GsElectronicPercussionSet)
    DECLARE_PERCUSSION_SET(GS_808_909_PERCUSSION_SET, smf::Gs808909PercussionSet)
    DECLARE_PERCUSSION_SET(GS_JAZZ_PERCUSSION_SET, smf::GsJazzPercussionSet)
    DECLARE_PERCUSSION_SET(GS_BRUSH_PERCUSSION_SET, smf::GsBrushPercussionSet)
    DECLARE_PERCUSSION_SET(GS_ORCHESTRA_PERCUSSION_SET, smf::GsOrchestraPercussionSet)
    DECLARE_PERCUSSION_SET(XG_STANDARD_1_PERCUSSION_SET, smf::XgStandard1PercussionSet)
    DECLARE_PERCUSSION_SET(XG_ROOM_PERCUSSION_SET, smf::XgRoomPercussionSet)
    DECLARE_PERCUSSION_SET(XG_ROCK_PERCUSSION_SET, smf::XgRockPercussionSet)
    DECLARE_PERCUSSION_SET(XG_ELECTRO_PERCUSSION_SET, smf::XgElectroPercussionSet)
    DECLARE_PERCUSSION_SET(XG_ANALOG_PERCUSSION_SET, smf::XgAnalogPercussionSet)
    DECLARE_PERCUSSION_SET(XG_JAZZ_PERCUSSION_SET, smf::XgJazzPercussionSet)
    DECLARE_PERCUSSION_SET(XG_BRUSH_PERCUSSION_SET, smf::XgBrushPercussionSet)
    DECLARE_PERCUSSION_SET(XG_CLASSIC_PERCUSSION_SET, smf::XgClassicPercussionSet)
    DECLARE_PERCUSSION_SET(XG_SFX_1_PERCUSSION_SET, smf::XgSFX1PercussionSet)
    DECLARE_PERCUSSION_SET(XG_SFX_2_PERCUSSION_SET, smf::XgSFX2PercussionSet)

#undef DECLARE_PERCUSSION_SET
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
                default:
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
                case 1:
                case 2: /* Also used for Standard 2 */
                default:
                    return m_knownSets[XG_STANDARD_1_PERCUSSION_SET];
                case 9:
                    return m_knownSets[XG_ROOM_PERCUSSION_SET];
                case 17:
                    return m_knownSets[XG_ROCK_PERCUSSION_SET];
                case 25:
                    return m_knownSets[XG_ELECTRO_PERCUSSION_SET];
                case 26:
                    return m_knownSets[XG_ANALOG_PERCUSSION_SET];
                case 33:
                    return m_knownSets[XG_JAZZ_PERCUSSION_SET];
                case 41:
                    return m_knownSets[XG_BRUSH_PERCUSSION_SET];
                case 49:
                    return m_knownSets[XG_CLASSIC_PERCUSSION_SET];
            }
        } else if (channelSetupInfo.m_bankMSB == 0x7e) {
            // SFX Percussion
            switch (channelSetupInfo.m_program) {
                case 1:
                default:
                    return m_knownSets[XG_SFX_1_PERCUSSION_SET];
                case 2:
                    return m_knownSets[XG_SFX_2_PERCUSSION_SET];
            }
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
        if ((channelSetupInfo.m_gsPartMode == 1) || (channelSetupInfo.m_gsPartMode == 2)) {
            // Percussion
            switch (channelSetupInfo.m_program) {
                case 1:
                case 2:
                default:
                    return m_knownSets[GS_STANDARD_1_PERCUSSION_SET];
                case 9:
                    return m_knownSets[GS_ROOM_PERCUSSION_SET];
                case 17:
                    return m_knownSets[GS_POWER_PERCUSSION_SET];
                case 25:
                case 27:
                    return m_knownSets[GS_ELECTRONIC_PERCUSSION_SET];
                case 26:
                    return m_knownSets[GS_808_909_PERCUSSION_SET];
                case 33:
                    return m_knownSets[GS_JAZZ_PERCUSSION_SET];
                case 41:
                    return m_knownSets[GS_BRUSH_PERCUSSION_SET];
                case 49:
                    return m_knownSets[GS_ORCHESTRA_PERCUSSION_SET];
            }
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
        case GS_ROOM_PERCUSSION_SET:
            return {{0x02, 0, 9, 1}};
        case GS_POWER_PERCUSSION_SET:
            return {{0x02, 0, 17, 1}};
        case GS_ELECTRONIC_PERCUSSION_SET:
            return {{0x02, 0, 25, 1}};
        case GS_808_909_PERCUSSION_SET:
            return {{0x02, 0, 26, 1}};
        case GS_JAZZ_PERCUSSION_SET:
            return {{0x02, 0, 33, 1}};
        case GS_BRUSH_PERCUSSION_SET:
            return {{0x02, 0, 41, 1}};
        case GS_ORCHESTRA_PERCUSSION_SET:
            return {{0x02, 0, 49, 1}};
        case XG_STANDARD_1_PERCUSSION_SET:
            return {{0x7f, 0, 1, 0}};
        case XG_ROOM_PERCUSSION_SET:
            return {{0x7f, 0, 9, 0}};
        case XG_ROCK_PERCUSSION_SET:
            return {{0x7f, 0, 17, 0}};
        case XG_ELECTRO_PERCUSSION_SET:
            return {{0x7f, 0, 25, 0}};
        case XG_ANALOG_PERCUSSION_SET:
            return {{0x7f, 0, 26, 0}};
        case XG_JAZZ_PERCUSSION_SET:
            return {{0x7f, 0, 33, 0}};
        case XG_BRUSH_PERCUSSION_SET:
            return {{0x7f, 0, 41, 0}};
        case XG_CLASSIC_PERCUSSION_SET:
            return {{0x7f, 0, 49, 0}};
        case XG_SFX_1_PERCUSSION_SET:
            return {{0x7e, 0, 1, 0}};
        case XG_SFX_2_PERCUSSION_SET:
            return {{0x7e, 0, 2, 0}};
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

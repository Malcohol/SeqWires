/**
 * Registration of factories for the Standard MIDI File support.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <Plugins/Smf/Plugin/libRegistration.hpp>

#include <SeqWiresLib/Percussion/builtInPercussionInstruments.hpp>
#include <SeqWiresLib/Percussion/abstractPercussionSet.hpp>

#include <BabelWiresLib/Project/projectContext.hpp>
#include <BabelWiresLib/TypeSystem/typeSystem.hpp>

#include <Plugins/Smf/Plugin/Percussion/gm2AnalogPercussionSet.hpp>
#include <Plugins/Smf/Plugin/Percussion/gm2BrushPercussionSet.hpp>
#include <Plugins/Smf/Plugin/Percussion/gm2ElectronicPercussionSet.hpp>
#include <Plugins/Smf/Plugin/Percussion/gm2JazzPercussionSet.hpp>
#include <Plugins/Smf/Plugin/Percussion/gm2OrchestraPercussionSet.hpp>
#include <Plugins/Smf/Plugin/Percussion/gm2PowerPercussionSet.hpp>
#include <Plugins/Smf/Plugin/Percussion/gm2RoomPercussionSet.hpp>
#include <Plugins/Smf/Plugin/Percussion/gm2SFXPercussionSet.hpp>
#include <Plugins/Smf/Plugin/Percussion/gm2StandardPercussionSet.hpp>
#include <Plugins/Smf/Plugin/Percussion/gmPercussionSet.hpp>
#include <Plugins/Smf/Plugin/Percussion/gs808909PercussionSet.hpp>
#include <Plugins/Smf/Plugin/Percussion/gsBrushPercussionSet.hpp>
#include <Plugins/Smf/Plugin/Percussion/gsElectronicPercussionSet.hpp>
#include <Plugins/Smf/Plugin/Percussion/gsJazzPercussionSet.hpp>
#include <Plugins/Smf/Plugin/Percussion/gsOrchestraPercussionSet.hpp>
#include <Plugins/Smf/Plugin/Percussion/gsPowerPercussionSet.hpp>
#include <Plugins/Smf/Plugin/Percussion/gsRoomPercussionSet.hpp>
#include <Plugins/Smf/Plugin/Percussion/gsStandard1PercussionSet.hpp>
#include <Plugins/Smf/Plugin/Percussion/gsSFXPercussionSet.hpp>
#include <Plugins/Smf/Plugin/Percussion/xgAnalogPercussionSet.hpp>
#include <Plugins/Smf/Plugin/Percussion/xgBrushPercussionSet.hpp>
#include <Plugins/Smf/Plugin/Percussion/xgClassicPercussionSet.hpp>
#include <Plugins/Smf/Plugin/Percussion/xgElectroPercussionSet.hpp>
#include <Plugins/Smf/Plugin/Percussion/xgJazzPercussionSet.hpp>
#include <Plugins/Smf/Plugin/Percussion/xgRockPercussionSet.hpp>
#include <Plugins/Smf/Plugin/Percussion/xgRoomPercussionSet.hpp>
#include <Plugins/Smf/Plugin/Percussion/xgSFX1PercussionSet.hpp>
#include <Plugins/Smf/Plugin/Percussion/xgSFX2PercussionSet.hpp>
#include <Plugins/Smf/Plugin/Percussion/xgStandard1PercussionSet.hpp>
#include <Plugins/Smf/Plugin/gmSpec.hpp>
#include <Plugins/Smf/Plugin/smfFormat.hpp>

void smf::registerLib(babelwires::ProjectContext& context) {
    // Formats
    context.m_sourceFileFormatReg.addEntry(std::make_unique<smf::SmfSourceFormat>());
    context.m_targetFileFormatReg.addEntry(std::make_unique<smf::SmfTargetFormat>());

    // Types
    context.m_typeSystem.addEntry<smf::GMSpecType>();

    // Percussion types
    const seqwires::BuiltInPercussionInstruments& builtInPercussion =
        context.m_typeSystem.getEntryByType<seqwires::BuiltInPercussionInstruments>();
    context.m_typeSystem.addEntry<GMPercussionSet>(builtInPercussion);
    context.m_typeSystem.addEntry<GM2StandardPercussionSet>(builtInPercussion);
    context.m_typeSystem.addEntry<GM2RoomPercussionSet>(builtInPercussion);
    context.m_typeSystem.addEntry<GM2PowerPercussionSet>(builtInPercussion);
    context.m_typeSystem.addEntry<GM2ElectronicPercussionSet>(builtInPercussion);
    context.m_typeSystem.addEntry<GM2AnalogPercussionSet>(builtInPercussion);
    context.m_typeSystem.addEntry<GM2JazzPercussionSet>(builtInPercussion);
    context.m_typeSystem.addEntry<GM2BrushPercussionSet>(builtInPercussion);
    context.m_typeSystem.addEntry<GM2OrchestraPercussionSet>(builtInPercussion);
    context.m_typeSystem.addEntry<GM2SFXPercussionSet>(builtInPercussion);
    context.m_typeSystem.addEntry<GsStandard1PercussionSet>(builtInPercussion);
    context.m_typeSystem.addEntry<GsRoomPercussionSet>(builtInPercussion);
    context.m_typeSystem.addEntry<GsPowerPercussionSet>(builtInPercussion);
    context.m_typeSystem.addEntry<GsElectronicPercussionSet>(builtInPercussion);
    context.m_typeSystem.addEntry<Gs808909PercussionSet>(builtInPercussion);
    context.m_typeSystem.addEntry<GsJazzPercussionSet>(builtInPercussion);
    context.m_typeSystem.addEntry<GsBrushPercussionSet>(builtInPercussion);
    context.m_typeSystem.addEntry<GsOrchestraPercussionSet>(builtInPercussion);
    context.m_typeSystem.addEntry<GsSFXPercussionSet>(builtInPercussion);
    context.m_typeSystem.addEntry<XgStandard1PercussionSet>(builtInPercussion);
    context.m_typeSystem.addEntry<XgRoomPercussionSet>(builtInPercussion);
    context.m_typeSystem.addEntry<XgRockPercussionSet>(builtInPercussion);
    context.m_typeSystem.addEntry<XgElectroPercussionSet>(builtInPercussion);
    context.m_typeSystem.addEntry<XgAnalogPercussionSet>(builtInPercussion);
    context.m_typeSystem.addEntry<XgJazzPercussionSet>(builtInPercussion);
    context.m_typeSystem.addEntry<XgBrushPercussionSet>(builtInPercussion);
    context.m_typeSystem.addEntry<XgClassicPercussionSet>(builtInPercussion);
    context.m_typeSystem.addEntry<XgSFX1PercussionSet>(builtInPercussion);
    context.m_typeSystem.addEntry<XgSFX2PercussionSet>(builtInPercussion);

    // Subtype relationships.
    context.m_typeSystem.addRelatedTypes(GM2StandardPercussionSet::getThisIdentifier(),
                                         {{seqwires::BuiltInPercussionInstruments::getThisIdentifier()}, {seqwires::AbstractPercussionSet::getThisIdentifier()}});
    context.m_typeSystem.addRelatedTypes(GM2RoomPercussionSet::getThisIdentifier(),
                                         {{seqwires::BuiltInPercussionInstruments::getThisIdentifier()}, {seqwires::AbstractPercussionSet::getThisIdentifier()}});
    context.m_typeSystem.addRelatedTypes(GM2PowerPercussionSet::getThisIdentifier(),
                                         {{seqwires::BuiltInPercussionInstruments::getThisIdentifier()}, {seqwires::AbstractPercussionSet::getThisIdentifier()}});
    context.m_typeSystem.addRelatedTypes(GM2ElectronicPercussionSet::getThisIdentifier(),
                                         {{seqwires::BuiltInPercussionInstruments::getThisIdentifier()}, {seqwires::AbstractPercussionSet::getThisIdentifier()}});
    context.m_typeSystem.addRelatedTypes(GM2AnalogPercussionSet::getThisIdentifier(),
                                         {{seqwires::BuiltInPercussionInstruments::getThisIdentifier()}, {seqwires::AbstractPercussionSet::getThisIdentifier()}});
    context.m_typeSystem.addRelatedTypes(GM2JazzPercussionSet::getThisIdentifier(),
                                         {{seqwires::BuiltInPercussionInstruments::getThisIdentifier()}, {seqwires::AbstractPercussionSet::getThisIdentifier()}});
    context.m_typeSystem.addRelatedTypes(GM2BrushPercussionSet::getThisIdentifier(),
                                         {{seqwires::BuiltInPercussionInstruments::getThisIdentifier()}, {seqwires::AbstractPercussionSet::getThisIdentifier()}});
    context.m_typeSystem.addRelatedTypes(GM2OrchestraPercussionSet::getThisIdentifier(),
                                         {{seqwires::BuiltInPercussionInstruments::getThisIdentifier()}, {seqwires::AbstractPercussionSet::getThisIdentifier()}});
    context.m_typeSystem.addRelatedTypes(GM2SFXPercussionSet::getThisIdentifier(),
                                         {{seqwires::BuiltInPercussionInstruments::getThisIdentifier()}, {seqwires::AbstractPercussionSet::getThisIdentifier()}});
    context.m_typeSystem.addRelatedTypes(GsStandard1PercussionSet::getThisIdentifier(),
                                         {{seqwires::BuiltInPercussionInstruments::getThisIdentifier()},
                                          {GM2StandardPercussionSet::getThisIdentifier()}});
    context.m_typeSystem.addRelatedTypes(
        GsRoomPercussionSet::getThisIdentifier(),
        {{seqwires::BuiltInPercussionInstruments::getThisIdentifier()}, {GM2RoomPercussionSet::getThisIdentifier()}});
    context.m_typeSystem.addRelatedTypes(
        GsPowerPercussionSet::getThisIdentifier(),
        {{seqwires::BuiltInPercussionInstruments::getThisIdentifier()}, {GM2PowerPercussionSet::getThisIdentifier()}});
    context.m_typeSystem.addRelatedTypes(GsElectronicPercussionSet::getThisIdentifier(),
                                         {{seqwires::BuiltInPercussionInstruments::getThisIdentifier()},
                                          {GM2ElectronicPercussionSet::getThisIdentifier()}});
    context.m_typeSystem.addRelatedTypes(Gs808909PercussionSet::getThisIdentifier(),
                                         {{seqwires::BuiltInPercussionInstruments::getThisIdentifier()}, {seqwires::AbstractPercussionSet::getThisIdentifier()}});
    context.m_typeSystem.addRelatedTypes(
        GsJazzPercussionSet::getThisIdentifier(),
        {{seqwires::BuiltInPercussionInstruments::getThisIdentifier()}, {GM2JazzPercussionSet::getThisIdentifier()}});
    context.m_typeSystem.addRelatedTypes(
        GsBrushPercussionSet::getThisIdentifier(),
        {{seqwires::BuiltInPercussionInstruments::getThisIdentifier()}, {GM2BrushPercussionSet::getThisIdentifier()}});
    context.m_typeSystem.addRelatedTypes(GsOrchestraPercussionSet::getThisIdentifier(),
                                         {{seqwires::BuiltInPercussionInstruments::getThisIdentifier()}, {seqwires::AbstractPercussionSet::getThisIdentifier()}});
    context.m_typeSystem.addRelatedTypes(GsSFXPercussionSet::getThisIdentifier(),
                                         {{seqwires::BuiltInPercussionInstruments::getThisIdentifier()}, {seqwires::AbstractPercussionSet::getThisIdentifier()}});
    context.m_typeSystem.addRelatedTypes(XgStandard1PercussionSet::getThisIdentifier(),
                                         {{seqwires::BuiltInPercussionInstruments::getThisIdentifier()}, {seqwires::AbstractPercussionSet::getThisIdentifier()}});
    context.m_typeSystem.addRelatedTypes(XgRoomPercussionSet::getThisIdentifier(),
                                         {{seqwires::BuiltInPercussionInstruments::getThisIdentifier()}, {seqwires::AbstractPercussionSet::getThisIdentifier()}});
    context.m_typeSystem.addRelatedTypes(XgRockPercussionSet::getThisIdentifier(),
                                         {{seqwires::BuiltInPercussionInstruments::getThisIdentifier()}, {seqwires::AbstractPercussionSet::getThisIdentifier()}});
    context.m_typeSystem.addRelatedTypes(XgElectroPercussionSet::getThisIdentifier(),
                                         {{seqwires::BuiltInPercussionInstruments::getThisIdentifier()}, {seqwires::AbstractPercussionSet::getThisIdentifier()}});
    context.m_typeSystem.addRelatedTypes(XgAnalogPercussionSet::getThisIdentifier(),
                                         {{seqwires::BuiltInPercussionInstruments::getThisIdentifier()}, {seqwires::AbstractPercussionSet::getThisIdentifier()}});
    context.m_typeSystem.addRelatedTypes(XgJazzPercussionSet::getThisIdentifier(),
                                         {{seqwires::BuiltInPercussionInstruments::getThisIdentifier()}, {seqwires::AbstractPercussionSet::getThisIdentifier()}});
    context.m_typeSystem.addRelatedTypes(XgBrushPercussionSet::getThisIdentifier(),
                                         {{seqwires::BuiltInPercussionInstruments::getThisIdentifier()}, {seqwires::AbstractPercussionSet::getThisIdentifier()}});
    context.m_typeSystem.addRelatedTypes(XgClassicPercussionSet::getThisIdentifier(),
                                         {{seqwires::BuiltInPercussionInstruments::getThisIdentifier()}, {seqwires::AbstractPercussionSet::getThisIdentifier()}});
    context.m_typeSystem.addRelatedTypes(XgSFX1PercussionSet::getThisIdentifier(),
                                         {{}, {seqwires::AbstractPercussionSet::getThisIdentifier()}});
    context.m_typeSystem.addRelatedTypes(XgSFX2PercussionSet::getThisIdentifier(),
                                         {{}, {seqwires::AbstractPercussionSet::getThisIdentifier()}});
    context.m_typeSystem.addRelatedTypes(GMPercussionSet::getThisIdentifier(),
                                         {{GM2StandardPercussionSet::getThisIdentifier()}, {seqwires::AbstractPercussionSet::getThisIdentifier()}});


}

/**
 * Registration of factories for the Standard MIDI File support.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <BabelWiresPlugins/Smf/Plugin/libRegistration.hpp>

#include <SeqWiresLib/builtInPercussionInstruments.hpp>

#include <BabelWiresLib/Project/projectContext.hpp>
#include <BabelWiresLib/TypeSystem/typeSystem.hpp>

#include <BabelWiresPlugins/Smf/Plugin/Percussion/gm2AnalogPercussionSet.hpp>
#include <BabelWiresPlugins/Smf/Plugin/Percussion/gm2BrushPercussionSet.hpp>
#include <BabelWiresPlugins/Smf/Plugin/Percussion/gm2ElectronicPercussionSet.hpp>
#include <BabelWiresPlugins/Smf/Plugin/Percussion/gm2JazzPercussionSet.hpp>
#include <BabelWiresPlugins/Smf/Plugin/Percussion/gm2OrchestraPercussionSet.hpp>
#include <BabelWiresPlugins/Smf/Plugin/Percussion/gm2PowerPercussionSet.hpp>
#include <BabelWiresPlugins/Smf/Plugin/Percussion/gm2RoomPercussionSet.hpp>
#include <BabelWiresPlugins/Smf/Plugin/Percussion/gm2StandardPercussionSet.hpp>
#include <BabelWiresPlugins/Smf/Plugin/Percussion/gm2SFXPercussionSet.hpp>
#include <BabelWiresPlugins/Smf/Plugin/Percussion/gmPercussionSet.hpp>
#include <BabelWiresPlugins/Smf/Plugin/Percussion/gsStandard1PercussionSet.hpp>
#include <BabelWiresPlugins/Smf/Plugin/Percussion/xgStandard1PercussionSet.hpp>
#include <BabelWiresPlugins/Smf/Plugin/Percussion/xgRoomPercussionSet.hpp>
#include <BabelWiresPlugins/Smf/Plugin/Percussion/xgRockPercussionSet.hpp>
#include <BabelWiresPlugins/Smf/Plugin/Percussion/xgElectroPercussionSet.hpp>
#include <BabelWiresPlugins/Smf/Plugin/gmSpec.hpp>
#include <BabelWiresPlugins/Smf/Plugin/smfFormat.hpp>

void smf::registerLib(babelwires::ProjectContext& context) {
    // Formats
    context.m_sourceFileFormatReg.addEntry(std::make_unique<smf::SmfSourceFormat>());
    context.m_targetFileFormatReg.addEntry(std::make_unique<smf::SmfTargetFormat>());

    // Types
    context.m_typeSystem.addEntry(std::make_unique<smf::GMSpecType>());

    // Percussion types
    const seqwires::BuiltInPercussionInstruments& builtInPercussion =
        context.m_typeSystem.getRegisteredEntry(seqwires::BuiltInPercussionInstruments::getThisIdentifier())
            .is<seqwires::BuiltInPercussionInstruments>();
    context.m_typeSystem.addEntry(std::make_unique<GMPercussionSet>(builtInPercussion));
    context.m_typeSystem.addEntry(std::make_unique<GM2StandardPercussionSet>(builtInPercussion));
    context.m_typeSystem.addEntry(std::make_unique<GM2RoomPercussionSet>(builtInPercussion));
    context.m_typeSystem.addEntry(std::make_unique<GM2PowerPercussionSet>(builtInPercussion));
    context.m_typeSystem.addEntry(std::make_unique<GM2ElectronicPercussionSet>(builtInPercussion));
    context.m_typeSystem.addEntry(std::make_unique<GM2AnalogPercussionSet>(builtInPercussion));
    context.m_typeSystem.addEntry(std::make_unique<GM2JazzPercussionSet>(builtInPercussion));
    context.m_typeSystem.addEntry(std::make_unique<GM2BrushPercussionSet>(builtInPercussion));
    context.m_typeSystem.addEntry(std::make_unique<GM2OrchestraPercussionSet>(builtInPercussion));
    context.m_typeSystem.addEntry(std::make_unique<GM2SFXPercussionSet>(builtInPercussion));
    context.m_typeSystem.addEntry(std::make_unique<GsStandard1PercussionSet>(builtInPercussion));
    context.m_typeSystem.addEntry(std::make_unique<XgStandard1PercussionSet>(builtInPercussion));
    context.m_typeSystem.addEntry(std::make_unique<XgRoomPercussionSet>(builtInPercussion));
    context.m_typeSystem.addEntry(std::make_unique<XgRockPercussionSet>(builtInPercussion));
    context.m_typeSystem.addEntry(std::make_unique<XgElectroPercussionSet>(builtInPercussion));

    // Subtype relationships.
    context.m_typeSystem.addRelatedTypes(GM2StandardPercussionSet::getThisIdentifier(),
                                         {{seqwires::BuiltInPercussionInstruments::getThisIdentifier()}, {}});
    context.m_typeSystem.addRelatedTypes(GM2RoomPercussionSet::getThisIdentifier(),
                                         {{seqwires::BuiltInPercussionInstruments::getThisIdentifier()}, {}});
    context.m_typeSystem.addRelatedTypes(GM2PowerPercussionSet::getThisIdentifier(),
                                         {{seqwires::BuiltInPercussionInstruments::getThisIdentifier()}, {}});
    context.m_typeSystem.addRelatedTypes(GM2ElectronicPercussionSet::getThisIdentifier(),
                                         {{seqwires::BuiltInPercussionInstruments::getThisIdentifier()}, {}});
    context.m_typeSystem.addRelatedTypes(GM2AnalogPercussionSet::getThisIdentifier(),
                                         {{seqwires::BuiltInPercussionInstruments::getThisIdentifier()}, {}});
    context.m_typeSystem.addRelatedTypes(GM2JazzPercussionSet::getThisIdentifier(),
                                         {{seqwires::BuiltInPercussionInstruments::getThisIdentifier()}, {}});
    context.m_typeSystem.addRelatedTypes(GM2BrushPercussionSet::getThisIdentifier(),
                                         {{seqwires::BuiltInPercussionInstruments::getThisIdentifier()}, {}});
    context.m_typeSystem.addRelatedTypes(GM2OrchestraPercussionSet::getThisIdentifier(),
                                         {{seqwires::BuiltInPercussionInstruments::getThisIdentifier()}, {}});
    context.m_typeSystem.addRelatedTypes(GM2SFXPercussionSet::getThisIdentifier(),
                                         {{seqwires::BuiltInPercussionInstruments::getThisIdentifier()}, {}});
    context.m_typeSystem.addRelatedTypes(GsStandard1PercussionSet::getThisIdentifier(),
                                         {{seqwires::BuiltInPercussionInstruments::getThisIdentifier()}, {GM2StandardPercussionSet::getThisIdentifier()}});
    context.m_typeSystem.addRelatedTypes(XgStandard1PercussionSet::getThisIdentifier(),
                                         {{seqwires::BuiltInPercussionInstruments::getThisIdentifier()}, {}});
    context.m_typeSystem.addRelatedTypes(XgRoomPercussionSet::getThisIdentifier(),
                                         {{seqwires::BuiltInPercussionInstruments::getThisIdentifier()}, {}});
    context.m_typeSystem.addRelatedTypes(XgRockPercussionSet::getThisIdentifier(),
                                         {{seqwires::BuiltInPercussionInstruments::getThisIdentifier()}, {}});
    context.m_typeSystem.addRelatedTypes(XgElectroPercussionSet::getThisIdentifier(),
                                         {{seqwires::BuiltInPercussionInstruments::getThisIdentifier()}, {}});
    context.m_typeSystem.addRelatedTypes(GMPercussionSet::getThisIdentifier(),
                                         {{GM2StandardPercussionSet::getThisIdentifier()}, {}});
}

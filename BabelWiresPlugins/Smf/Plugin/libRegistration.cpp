/**
 * Registration of factories for the Standard MIDI File support.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <BabelWiresPlugins/Smf/Plugin/libRegistration.hpp>

#include <SeqWiresLib/percussion.hpp>

#include <BabelWiresLib/Project/projectContext.hpp>
#include <BabelWiresLib/TypeSystem/typeSystem.hpp>

#include <BabelWiresPlugins/Smf/Plugin/gmSpec.hpp>
#include <BabelWiresPlugins/Smf/Plugin/Percussion/smfPercussion.hpp>
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
    context.m_typeSystem.addEntry(std::make_unique<GMPercussionKit>(builtInPercussion));
    context.m_typeSystem.addEntry(std::make_unique<GM2StandardPercussionKit>(builtInPercussion));

    // Subtype relationships.
    context.m_typeSystem.addRelatedTypes(GM2StandardPercussionKit::getThisIdentifier(), {{seqwires::BuiltInPercussionInstruments::getThisIdentifier()}, {}});
    context.m_typeSystem.addRelatedTypes(GMPercussionKit::getThisIdentifier(), {{GM2StandardPercussionKit::getThisIdentifier()}, {}});
}

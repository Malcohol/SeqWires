/**
 * Some definitions for use across the plugin.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <Plugins/Smf/Plugin/smfCommon.hpp>

#include <Plugins/Smf/Plugin/smfSequence.hpp>

#include <BabelWiresLib/Types/File/fileTypeConstructor.hpp>

babelwires::TypeRef smf::getSmfFileType() {
    return babelwires::FileTypeConstructor::makeTypeRef(SmfSequence::getThisIdentifier());
}

/**
 * Some definitions for use across the plugin.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <SeqWiresLib/Types/Track/trackType.hpp>

namespace smf {
    using TypeOfTracks = seqwires::DefaultTrackType;

    /// Get the FileType subclass which contains SMF data.
    babelwires::TypeRef getSmfFileType();
}

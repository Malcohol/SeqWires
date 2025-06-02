/**
 * Initialize and deinitialize support for ALSA (Advanced Linux Sound System).
 *
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/

#pragma once

namespace babelwires {
    class AudioInterfaceRegistry;
}

namespace babelwires_alsa {

    void init_audio(babelwires::AudioInterfaceRegistry& registry);

    void shutdown_audio(babelwires::AudioInterfaceRegistry& registry);

} // namespace babelwires_alsa

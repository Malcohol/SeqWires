/**
 * Initialize and deinitialize support for audio.
 *
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/

#pragma once

namespace babelwires {
    class AudioInterfaceRegistry;

    // TODO: Could this use the same shape as the "libRegistration" files in other libs?

    void init_audio(AudioInterfaceRegistry& registry);

    void shutdown_audio(AudioInterfaceRegistry& registry);

} // namespace babelwires_alsa

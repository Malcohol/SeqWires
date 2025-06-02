/**
 * Some common functionality used for ALSA (Advanced Linux Sound System).
 *
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/

#pragma once

#include <alsa/asoundlib.h>

namespace babelwires_alsa {

    // Safe version of a snd_pcm_hw_params_t*.
    struct HardwareParameters {
        HardwareParameters();

        ~HardwareParameters();

        operator snd_pcm_hw_params_t*();

        snd_pcm_hw_params_t* m_params;
    };

    void checkForError(const char* description, int retCode);
} // namespace babelwires_alsa

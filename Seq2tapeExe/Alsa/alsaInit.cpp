/**
 * Initialize and deinitialize support for ALSA (Advanced Linux Sound System).
 *
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/

#include <Seq2tapeExe/Alsa/alsaInit.hpp>

#include <Seq2tapeExe/Alsa/alsaAudioInterface.hpp>

#include <Seq2tapeLib/Audio/audioInterface.hpp>

void babelwires_alsa::init_audio(babelwires::AudioInterfaceRegistry& registry) {
    registry.addEntry<AlsaAudioInterface>();
}

void babelwires_alsa::shutdown_audio(babelwires::AudioInterfaceRegistry& registry) {}

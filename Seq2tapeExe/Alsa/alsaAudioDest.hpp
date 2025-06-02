/**
 * AlsaAudioDest is an AudioDest for ALSA (Advanced Linux Sound System).
 *
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/

#pragma once

#include <Seq2tapeLib/Audio/audioDest.hpp>

#include <memory>
#include <string>

namespace babelwires_alsa {

     /// AlsaAudioDest is an AudioDest for ALSA (Advanced Linux Sound System).
     class AlsaAudioDest : public babelwires::AudioDest {
     public:
       AlsaAudioDest(const char* pcmHandleName);

       ~AlsaAudioDest();

       virtual int getNumChannels() const override;

       virtual babelwires::Duration getFrequency() const override;

       virtual unsigned long writeMoreAudioData(const babelwires::AudioSample* buffer, unsigned long bufSize) override;

     protected:
       struct Impl;

     private:
       std::unique_ptr<Impl> m_impl;
   };

} // namespace babelwires_alsa

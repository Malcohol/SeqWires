/**
 * Factories for creating AudioSources corresponding to sound formats.
 *
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <Seq2tapeLib/Audio/audioSource.hpp>
#include <memory>
#include <string>

namespace babelwires {

    class FileAudioSource : public AudioSource {
      public:
        FileAudioSource(const char* fileName);
        virtual ~FileAudioSource();

        virtual int getNumChannels() const override;

        virtual Duration getFrequency() const override;

        virtual unsigned long getMoreAudioData(AudioSample* buffer, unsigned long bufSize) override;

      private:
        struct Impl;
        std::unique_ptr<Impl> m_impl;
    };

} // namespace babelwires

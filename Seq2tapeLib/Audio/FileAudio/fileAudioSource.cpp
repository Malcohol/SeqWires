/**
 * Factories for creating AudioSources corresponding to sound formats.
 *
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <Seq2tapeLib/Audio/FileAudio/fileAudioSource.hpp>

#include <sndfile.h>

#include <assert.h>
#include <stdexcept>

struct babelwires::FileAudioSource::Impl {
    Impl(const char* fileName) {
        m_info.format = 0;
        m_sndFile = sf_open(fileName, SFM_READ, &m_info);

        if (m_sndFile) {

        } else {
            throw FileIoException() << getErrorString();
        }
    }

    ~Impl() {
        if (m_sndFile) {
            sf_close(m_sndFile);
        }
    }

    std::string getErrorString() {
        assert(sf_error(m_sndFile) != SF_ERR_NO_ERROR);
        return sf_strerror(m_sndFile);
    }

    SF_INFO m_info;
    SNDFILE* m_sndFile;
};

babelwires::FileAudioSource::FileAudioSource(const char* fileName)
    : m_impl(new Impl(fileName)) {}

babelwires::FileAudioSource::~FileAudioSource() {
    // Required out-of-line, because of the unique_ptr.
}

int babelwires::FileAudioSource::getNumChannels() const {
    return m_impl->m_info.channels;
}

babelwires::Duration babelwires::FileAudioSource::getFrequency() const {
    return m_impl->m_info.samplerate;
}

unsigned long babelwires::FileAudioSource::getMoreAudioData(AudioSample* buffer, unsigned long bufSize) {
    assert(sf_error(m_impl->m_sndFile) == SF_ERR_NO_ERROR);
    return sf_read_float(m_impl->m_sndFile, buffer, bufSize);
}

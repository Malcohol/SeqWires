/**
 * A SampleReader manages the buffered reading of samples from an AudioSource.
 * 
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <Seq2tapeLib/sampleReader.hpp>

#include <Common/Audio/audioSource.hpp>

#include <cassert>

seq2tape::SampleReader::SampleReader(babelwires::AudioSource& audioSource)
    : m_audioSource(audioSource)
    , m_numChannels(audioSource.getNumChannels())
    , m_numSamplesInBuffer(0)
    , m_cursorInBuffer(0)
    , m_numSamplesRead(0)
    , m_samplePeriod(1.0 / audioSource.getFrequency()) {}

seq2tape::SampleReader::SampleResult seq2tape::SampleReader::getNextSample(babelwires::AudioSample& sample) {
    if (m_cursorInBuffer >= m_numSamplesInBuffer) {
        m_numSamplesInBuffer = m_audioSource.getMoreAudioData(m_buffer, BUFSIZE);
        m_cursorInBuffer = 0;
        if (m_numSamplesInBuffer == 0) {
            return SAMPLE_EOF;
        } else {
            assert(m_numSamplesInBuffer % m_numChannels == 0);
            assert(m_numSamplesInBuffer <= BUFSIZE);
        }
    }
    sample = 0;
    for (unsigned long j = 0; j < m_numChannels; ++j) {
        // TODO This looks wrong.
        sample += m_buffer[(m_cursorInBuffer * m_numChannels) + j];
    }
    sample = sample / m_numChannels;
    m_cursorInBuffer += m_numChannels;
    ++m_numSamplesRead;
    return SAMPLE_OK;
}

babelwires::Duration seq2tape::SampleReader::getPositionInStream() const {
    return m_numSamplesRead * m_samplePeriod;
}

unsigned long seq2tape::SampleReader::getNumSamplesRead() const {
    return m_numSamplesRead;
}

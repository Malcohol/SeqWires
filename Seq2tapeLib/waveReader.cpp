/**
 * A WaveReader tries to read waves of specific lengths from an audio file.
 * 
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#define _USE_MATH_DEFINES
#include <Seq2tapeLib/waveReader.hpp>

#include <Common/Audio/audioSource.hpp>

#include <algorithm>
#include <assert.h>
#include <memory>

#include <cmath>


// TODO
// Remember time at last zero crossing.

namespace {
    constexpr babelwires::Duration c_pi = 3.14159265358979323846;

    /// When a wave of an expected type is read, we allow the referenceDuration to be adjusted.
    /// This allows some compensation for the recording speeding up or slowing down.
    /// This weight determines the influence of the new wave's duration against the existing one.
    constexpr babelwires::Duration c_newDurationWeight = 0.25;

    // These two thresholds ensure there is hysteresis in the system, i.e. prevents wobbles around 0
    // being considered very short waves. Note: We draw a line between the samples either side of the
    // two thresholds to determine when the pulse switched between negative and positive.

    /// The absolute value of a sample has to pass this threshold to trigger a switch from the negative
    /// to positive pulses or vice-versa.
    constexpr babelwires::AudioSample c_switchThreshold = 0.05f;

    /// When the absolute value of a sample in a pulse falls below this value, we consider that it is
    /// at the end of the pulse.
    constexpr babelwires::AudioSample c_returnThreshold = 0.02f;

    /// The proportion of the space between two wave types which is regarded as ambiguous.
    constexpr double c_interWaveGap = 0.15;

    /// When we successfully read a wave, we estimate the current bias by comparing the length of the
    /// negative and positive pulses. This factor controls how much influence the latest estimate has
    /// on the estimate used.
    constexpr double c_newBiasWeight = 0.15;
} // namespace

seq2tape::WaveReader::WaveReader(babelwires::AudioSource& audioSource, Polarity polarity,
                                 const std::vector<babelwires::Duration>& waveLengths)
    : m_sampleReader(audioSource)
    , m_samplePeriod(1.0 / audioSource.getFrequency())
    , m_negativeDurationFromPreviousStep(0.0)
    , m_polarityMultiplier((polarity == Polarity::negativeThenPositive) ? 1.0f : -1.0f) {
    assert((waveLengths.size() > 1) && "It doesn't make sense to have a waveReader with only one kind of wave");

    m_perWaveInfo.reserve(waveLengths.size());
    for (int i = 0; i < waveLengths.size(); ++i) {
        assert(waveLengths[i] > 1.e-8);

        PerWaveInfo& info = m_perWaveInfo.emplace_back();
        // We will temporarily put absolute durations in the proportions. It will be fixed later.
        info.m_proportionOfReferenceDuration = waveLengths[i];
        info.m_originalIndex = i;
    }

    std::sort(m_perWaveInfo.begin(), m_perWaveInfo.end(), [](const PerWaveInfo& a, const PerWaveInfo& b) {
        return a.m_proportionOfReferenceDuration < b.m_proportionOfReferenceDuration;
    });

    // Use the absolute wave length of the longest wave.
    m_postReturnTimeOut = m_perWaveInfo[m_perWaveInfo.size() - 1].m_proportionOfReferenceDuration;

    for (int i = 0; i < m_perWaveInfo.size() - 1; ++i) {
        PerWaveInfo& infoL = m_perWaveInfo[i];
        PerWaveInfo& infoR = m_perWaveInfo[i + 1];
        const Proportion diff = infoR.m_proportionOfReferenceDuration - infoL.m_proportionOfReferenceDuration;
        const Proportion halfInterval = (diff * (1 - c_interWaveGap)) / 2.0;
        infoL.m_upperBoundProportion = infoL.m_proportionOfReferenceDuration + halfInterval;
        infoR.m_lowerBoundProportion = infoR.m_proportionOfReferenceDuration - halfInterval;
    }

    {
        PerWaveInfo& info = m_perWaveInfo[0];
        info.m_lowerBoundProportion =
            std::max(0.0, info.m_proportionOfReferenceDuration -
                              (info.m_upperBoundProportion - info.m_proportionOfReferenceDuration));
    }
    {
        PerWaveInfo& info = m_perWaveInfo[m_perWaveInfo.size() - 1];
        info.m_upperBoundProportion =
            info.m_proportionOfReferenceDuration + (info.m_proportionOfReferenceDuration - info.m_lowerBoundProportion);
    }

    m_referenceDuration = m_perWaveInfo[0].m_proportionOfReferenceDuration;
    for (PerWaveInfo& info : m_perWaveInfo) {
        info.m_proportionOfReferenceDuration = info.m_proportionOfReferenceDuration / m_referenceDuration;
        info.m_lowerBoundProportion = info.m_lowerBoundProportion / m_referenceDuration;
        info.m_upperBoundProportion = info.m_upperBoundProportion / m_referenceDuration;
    }

    m_originalToSortedIndex.resize(m_perWaveInfo.size());
    for (int i = 0; i < m_perWaveInfo.size(); ++i) {
        m_originalToSortedIndex[m_perWaveInfo[i].m_originalIndex] = i;
    }
}

babelwires::Duration seq2tape::WaveReader::getPositionInStream() const {
    return m_sampleReader.getPositionInStream();
}

int seq2tape::WaveReader::getWaveType(babelwires::Duration waveDuration) const {
    const Proportion currentWaveProportion = waveDuration / m_referenceDuration;

    for (const PerWaveInfo& info : m_perWaveInfo) {
        if (currentWaveProportion <= info.m_lowerBoundProportion) {
            return WAVE_TYPE_UNKNOWN;
        } else if (currentWaveProportion <= info.m_upperBoundProportion) {
            return info.m_originalIndex;
        }
    }

    return WAVE_TYPE_UNKNOWN;
}

void seq2tape::WaveReader::updateReferenceDuration(int waveType, babelwires::Duration waveDuration) {
    assert((waveType >= 0) && (waveType < m_perWaveInfo.size()));
    const Proportion waveProportion = m_perWaveInfo[m_originalToSortedIndex[waveType]].m_proportionOfReferenceDuration;
    const babelwires::Duration expectedDuration = m_referenceDuration * waveProportion;
    const babelwires::Duration average =
        ((1.0 - c_newDurationWeight) * expectedDuration) + (c_newDurationWeight * waveDuration);
    m_referenceDuration = average / waveProportion;
}

void seq2tape::WaveReader::updateBiasEstimate(babelwires::Duration negativeDuration,
                                              babelwires::Duration positiveDuration) {
    /// The code is written assume an inverted sin wave (because that was the shape of the first source data I examined)
    const babelwires::Duration ratio = (negativeDuration - positiveDuration) / (negativeDuration + positiveDuration);
    const babelwires::Duration theta = 0.5 * c_pi * ratio;
    const babelwires::AudioSample estimatedBiasNow = -sin(theta);
    m_biasEstimate = ((1.0 - c_newBiasWeight) * m_biasEstimate) + (c_newBiasWeight * estimatedBiasNow);
}

seq2tape::SampleReader::SampleResult seq2tape::WaveReader::getNextSample(babelwires::AudioSample& sample) {
    seq2tape::SampleReader::SampleResult result = m_sampleReader.getNextSample(sample);
    if (result == SampleReader::SAMPLE_OK) {
        const double numSamplesRead = m_sampleReader.getNumSamplesRead();
        m_signalMean = (m_signalMean * ((numSamplesRead - 1.0) / numSamplesRead)) + (sample * (1.0 / numSamplesRead));
        sample -= m_signalMean;
        sample *= m_polarityMultiplier;
        sample -= m_biasEstimate;
    }
    return result;
}

seq2tape::WaveReader::PulseDurations seq2tape::WaveReader::getNextWaveDuration() {
    babelwires::Duration positiveDuration = 0.0;
    babelwires::Duration negativeDuration = m_negativeDurationFromPreviousStep;
    babelwires::Duration timeSinceFlip = 0.0;
    babelwires::AudioSample preFlipSample = 0.0f;

    enum State { NEGATIVE_PULSE, NEGATIVE_TO_POSITIVE, POSITIVE_PULSE, POSITIVE_TO_NEGATIVE } state = NEGATIVE_PULSE;

    babelwires::AudioSample sample;
    while (getNextSample(sample) == SampleReader::SAMPLE_OK) {
        switch (state) {
            case NEGATIVE_PULSE: {
                if (sample < -c_returnThreshold) {
                    negativeDuration += m_samplePeriod;
                    preFlipSample = sample;
                    break;
                }
                state = NEGATIVE_TO_POSITIVE;
                timeSinceFlip = 0.0;
                [[fallthrough]];
            }
            case NEGATIVE_TO_POSITIVE: {
                timeSinceFlip += m_samplePeriod;
                if (sample > c_switchThreshold) {
                    // enter positive state
                    state = POSITIVE_PULSE;
                    assert(preFlipSample <= 0);
                    const babelwires::Duration preFlipProportion = -preFlipSample / (-preFlipSample + sample);
                    const babelwires::Duration preFlipDuration = timeSinceFlip * preFlipProportion;
                    const babelwires::Duration postFlipDuration = timeSinceFlip * (1 - preFlipProportion);
                    negativeDuration += preFlipDuration;
                    positiveDuration = postFlipDuration;
                    preFlipSample = sample;
                }
                break;
            }
            case POSITIVE_PULSE: {
                if (sample > c_returnThreshold) {
                    positiveDuration += m_samplePeriod;
                    preFlipSample = sample;
                    break;
                }
                state = POSITIVE_TO_NEGATIVE;
                timeSinceFlip = 0.0;
                [[fallthrough]];
            }
            case POSITIVE_TO_NEGATIVE: {
                timeSinceFlip += m_samplePeriod;
                if (timeSinceFlip > m_postReturnTimeOut) {
                    state = POSITIVE_PULSE;
                    positiveDuration += m_samplePeriod;
                    // No point in carrying the excess forward.
                    m_negativeDurationFromPreviousStep = 0.0;
                    return {negativeDuration, positiveDuration};
                }
                if (sample < -c_switchThreshold) {
                    state = NEGATIVE_PULSE;
                    assert(preFlipSample >= 0);
                    const babelwires::Duration preFlipProportion = preFlipSample / (preFlipSample - sample);
                    const babelwires::Duration preFlipDuration = timeSinceFlip * preFlipProportion;
                    const babelwires::Duration postFlipDuration = timeSinceFlip * (1 - preFlipProportion);
                    positiveDuration += preFlipDuration;
                    m_negativeDurationFromPreviousStep = postFlipDuration;

                    return {negativeDuration, positiveDuration};
                }

                break;
            }
        }
    }
    switch (state) {
        case POSITIVE_PULSE: {
            return {negativeDuration, positiveDuration};
        }
        default:
            return {0.0, 0.0};
    }
}

int seq2tape::WaveReader::getNextWave(int expectedWave) {
    const auto [negativeDuration, positiveDuration] = getNextWaveDuration();
    const babelwires::Duration waveDuration = negativeDuration + positiveDuration;

    if (waveDuration == 0.0) {
        return WAVE_TYPE_EOF;
    }

    const int waveType = getWaveType(waveDuration);
    if (expectedWave != WAVE_TYPE_UNKNOWN) {
        updateBiasEstimate(negativeDuration, positiveDuration);
        if (waveType == expectedWave) {
            updateReferenceDuration(waveType, waveDuration);
        }
    }
    return waveType;
}

int seq2tape::WaveReader::seekPilot(int waveType, int minimumNumWaves) {
    int shortCount = 0;
    do {
        const auto [negativeDuration, positiveDuration] = getNextWaveDuration();
        const babelwires::Duration waveDuration = negativeDuration + positiveDuration;
        updateReferenceDuration(waveType, waveDuration);
        if (waveDuration == 0.0) {
            return WAVE_TYPE_EOF;
        }
        if (getWaveType(waveDuration) == waveType) {
            ++shortCount;
        } else {
            shortCount = 0;
        }
    } while (shortCount < minimumNumWaves);
    int type = WAVE_TYPE_UNKNOWN;
    do {
        type = getNextWave(waveType);
    } while (type == waveType);
    return type;
}

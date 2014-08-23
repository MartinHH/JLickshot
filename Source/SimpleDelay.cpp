/*
 * Copyright 2014 Martin Hansen
 *
 * This file is part of JLickshot.
 *
 * JLickshot is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * JLickshot is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with JLickshot.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "SimpleDelay.h"

SimpleDelay::SimpleDelay(int noChannels, int maxLength, int sampleRate):
    delayBuffer_(noChannels, maxLength * sampleRate),
    maxLength_(maxLength),
    length_(0.5),
    feedBack_(0.0),
    sampleRate_(sampleRate),
    delayBufferIdx_(0)
{
}

SimpleDelay::~SimpleDelay()
{
}

void SimpleDelay::setSampleRate(int sampleRate)
{
    if(sampleRate != sampleRate_){
        const ScopedLock sl (lock);
        delayBuffer_.setSize(delayBuffer_.getNumChannels(), maxLength_ * sampleRate);
        sampleRate_ = sampleRate;
    }
}

void SimpleDelay::setLength(const float length)
{
    length_ = SATURATE(0.0, 1.0, length);
}

float SimpleDelay::getLength() const
{
    return length_;
}

float SimpleDelay::getLengthInSeconds() const
{
    return maxLength_ * length_;
}

void SimpleDelay::setFeedback(float feedback)
{
    feedBack_ = SATURATE(0.0, 1.0, feedback);
}

void SimpleDelay::processBlock(juce::AudioSampleBuffer &buffer)
{
    const ScopedLock sl(lock);
    
    int dIdx;
    
    const int nChannels = jmin(buffer.getNumChannels(), delayBuffer_.getNumChannels());
    const int dLength = (int)(delayBuffer_.getNumSamples() * length_);
    const int nSamples = buffer.getNumSamples();
    
    // for each channel:
    for (int channel = 0; channel < nChannels; channel++){
        float* channelData = buffer.getWritePointer (channel);
        float* delayData = delayBuffer_.getWritePointer(channel);
        dIdx = delayBufferIdx_;

        // for each sample:
        for (int i = 0; i < nSamples; i++){
            const float in = channelData[i];
            channelData[i] += delayData[dIdx];
            delayData[dIdx] = (delayData[dIdx] + in) * feedBack_;
            if (++dIdx >= dLength)
                dIdx = 0;
        }
    }
    delayBufferIdx_ = dIdx;
}
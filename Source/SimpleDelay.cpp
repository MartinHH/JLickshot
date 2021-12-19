/*
 * Copyright 2014-2021 Martin Hansen
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

SimpleDelay::SimpleDelay(int noChannels, int maxTime, int sampleRate):
    delayBuffer_(noChannels, maxTime * sampleRate),
    maxTime_(maxTime),
    time_(0.5),
    feedBack_(0.0),
    sampleRate_(sampleRate),
    delayBufferIdx_(0),
    lp_(),
    lowPass_(false)
{
    for(int i=0; i < noChannels; i++){
        lp_.push_back(OnePoleLowpass(5000, sampleRate_));
    }
}

SimpleDelay::~SimpleDelay()
{
}

void SimpleDelay::setSampleRate(int sampleRate)
{
    if(sampleRate != sampleRate_){
        const ScopedLock sl (lock);
        delayBuffer_.setSize(delayBuffer_.getNumChannels(), maxTime_ * sampleRate);
        sampleRate_ = sampleRate;
        for(std::vector<OnePoleLowpass>::iterator it = lp_.begin(); it != lp_.end(); ++it){
            it->setSampleRate(sampleRate);
        }
    }
}

void SimpleDelay::setRelativeTime(const float time)
{
    time_ = SATURATE(0.0, 1.0, time);
}

float SimpleDelay::getRelativeTime() const
{
    return time_;
}

float SimpleDelay::getTimeInSeconds() const
{
    return maxTime_ * time_;
}

void SimpleDelay::setFeedback(float feedback)
{
    feedBack_ = SATURATE(0.0, 1.0, feedback);
}

float SimpleDelay::getFeedback() const
{
    return feedBack_;
}

void SimpleDelay::setLowpassIsActive(bool lowPassActive)
{
    lowPass_ = lowPassActive;
}

bool SimpleDelay::getLowpassIsActive() const
{
    return lowPass_;
}

void SimpleDelay::setLowpassFrequency(double frequency)
{
    const double f = SATURATE(LP_MIN_FREQ, LP_MAX_FREQ, frequency);
    for(std::vector<OnePoleLowpass>::iterator it = lp_.begin(); it != lp_.end(); ++it){
        it->setFrequency(f);
    }
}

double SimpleDelay::getLowpassFrequency() const
{
    return lp_.size() > 0 ? lp_[0].getFrequency() : INFINITY;
}

int SimpleDelay::getNumChannels() const
{
    return delayBuffer_.getNumChannels();
}

void SimpleDelay::processBlock(juce::AudioSampleBuffer &buffer)
{
    const ScopedLock sl(lock);
    
    int dIdx = 0;
    
    const int nChannels = jmin(buffer.getNumChannels(), delayBuffer_.getNumChannels());
    const int dLength = (int)(delayBuffer_.getNumSamples() * time_);
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
            const float delayDry = (delayData[dIdx] + in) * feedBack_;
            if (lowPass_) {
                delayData[dIdx] = lp_[channel].process(delayDry);
            } else {
                delayData[dIdx] = delayDry;
            }
            if (++dIdx >= dLength)
                dIdx = 0;
        }
    }
    delayBufferIdx_ = dIdx;
}

XmlElement* SimpleDelay::getStateXml() const
{
    XmlElement* rv = new XmlElement("DELAYSETTINGS");
    
    rv->setAttribute("length", time_);
    rv->setAttribute("feedback", feedBack_);
    rv->setAttribute("lp_active", lowPass_);
    rv->setAttribute("lp_freq", getLowpassFrequency());
    
    return rv;
}

void SimpleDelay::updateFromXml(XmlElement *stateXml)
{
    if (stateXml != nullptr && stateXml->hasTagName("DELAYSETTINGS")) {
        time_ = stateXml->getDoubleAttribute("length", time_);
        feedBack_ = stateXml->getDoubleAttribute("feedback", feedBack_);
        lowPass_ = stateXml->getBoolAttribute("lp_active", lowPass_);
        setLowpassFrequency(stateXml->getDoubleAttribute("lp_freq",
                                                         getLowpassFrequency()));
    }
}

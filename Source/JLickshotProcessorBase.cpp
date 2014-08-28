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

#include "JLickshotProcessorBase.h"

JLickshotProcessorBase::JLickshotProcessorBase(int noOfVoices):
    synth_(noOfVoices),
    delayIsActive_(false),
    reverbIsActive_(false)
{
}

JLickshotProcessorBase::~JLickshotProcessorBase()
{
}

MidiKeyboardState& JLickshotProcessorBase::getKeyState()
{
    return keyState_;
}

SampleSynth& JLickshotProcessorBase::getSynth()
{
    return synth_;
}

SimpleDelay& JLickshotProcessorBase::getDelayUnit()
{
    return delay_;
}

MVerbPlus& JLickshotProcessorBase::getMVerb()
{
    return mVerb_;
}

void JLickshotProcessorBase::process(juce::AudioSampleBuffer &buffer,
                                     juce::MidiBuffer &midiData,
                                     int numSamples)
{
    keyState_.processNextMidiBuffer (midiData, 0, numSamples, true);
    
    synth_.renderNextBlock (buffer, midiData, 0, numSamples);
    if (delayIsActive_) {
        delay_.processBlock(buffer);
    }
    
    if(reverbIsActive_){
        mVerb_.process(buffer.getArrayOfReadPointers(),
                       buffer.getArrayOfWritePointers(),
                       numSamples);
    }
}

void JLickshotProcessorBase::setSampleRate(double sampleRate)
{
    synth_.setCurrentPlaybackSampleRate (sampleRate);
    delay_.setSampleRate((int) sampleRate);
    mVerb_.setSampleRate(sampleRate);
}

void JLickshotProcessorBase::setDelayIsActive(bool delayIsActive)
{
    delayIsActive_ = delayIsActive;
}

bool JLickshotProcessorBase::getDelayIsActive() const
{
    return delayIsActive_;
}

void JLickshotProcessorBase::setReverbIsActive(bool reverbIsActive)
{
    reverbIsActive_ = reverbIsActive;
}

bool JLickshotProcessorBase::getReverbIsActive() const
{
    return reverbIsActive_;
}
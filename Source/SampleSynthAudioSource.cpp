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


#include "SampleSynthAudioSource.h"

SampleSynthAudioSource::SampleSynthAudioSource(MidiKeyboardState& keyState,
                                               int noOfVoices):
    keyState_(keyState),
    synth_(noOfVoices),
    delayIsActive_(false),
    reverbIsActive_(false)
{
}

SampleSynthAudioSource::~SampleSynthAudioSource()
{
}

SampleSynth& SampleSynthAudioSource::getSynth()
{
    return synth_;
}

MidiMessageCollector* SampleSynthAudioSource::getMidiCollector()
{
    return &midiCollector_;
}

SimpleDelay& SampleSynthAudioSource::getDelayUnit()
{
    return delay_;
}

MVerbPlus& SampleSynthAudioSource::getMVerb()
{
    return mVerb_;
}

void SampleSynthAudioSource::prepareToPlay (int /* samplesPerBlockExpected */,
                                            double sampleRate)
{
    midiCollector_.reset (sampleRate);
    synth_.setCurrentPlaybackSampleRate (sampleRate);
    delay_.setSampleRate((int) sampleRate);
    mVerb_.setSampleRate(sampleRate);
}

void SampleSynthAudioSource::releaseResources()
{
}

void SampleSynthAudioSource::getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill)
{
    bufferToFill.clearActiveBufferRegion();
    
    MidiBuffer incomingMidi;
    midiCollector_.removeNextBlockOfMessages (incomingMidi, bufferToFill.numSamples);
    
    keyState_.processNextMidiBuffer (incomingMidi, 0, bufferToFill.numSamples, true);
    
    synth_.renderNextBlock (*bufferToFill.buffer, incomingMidi, 0,
                            bufferToFill.numSamples);
    if (delayIsActive_) {
        delay_.processBlock(*bufferToFill.buffer);
    }
    
    if(reverbIsActive_){
        mVerb_.process(bufferToFill.buffer->getArrayOfReadPointers(),
                       bufferToFill.buffer->getArrayOfWritePointers(),
                       bufferToFill.numSamples);
    }
}

void SampleSynthAudioSource::setDelayIsActive(bool delayIsActive)
{
    delayIsActive_ = delayIsActive;
}

bool SampleSynthAudioSource::getDelayIsActive() const
{
    return delayIsActive_;
}

void SampleSynthAudioSource::setReverbIsActive(bool reverbIsActive)
{
    reverbIsActive_ = reverbIsActive;
}

bool SampleSynthAudioSource::getReverbIsActive() const
{
    return reverbIsActive_;
}
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


#include "SampleSynthAudioSource.h"

SampleSynthAudioSource::SampleSynthAudioSource(int noOfVoices):
    JLickshotProcessorBase(noOfVoices)
{
}

SampleSynthAudioSource::~SampleSynthAudioSource()
{
}

MidiMessageCollector* SampleSynthAudioSource::getMidiCollector()
{
    return &midiCollector_;
}

void SampleSynthAudioSource::prepareToPlay (int /* samplesPerBlockExpected */,
                                            double sampleRate)
{
    midiCollector_.reset (sampleRate);
    setSampleRate(sampleRate);
}

void SampleSynthAudioSource::releaseResources()
{
}

void SampleSynthAudioSource::getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill)
{
    bufferToFill.clearActiveBufferRegion();
    
    MidiBuffer incomingMidi;
    midiCollector_.removeNextBlockOfMessages (incomingMidi, bufferToFill.numSamples);

    process(*bufferToFill.buffer, incomingMidi, bufferToFill.numSamples);
}

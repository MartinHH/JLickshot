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


#ifndef SAMPLESYNTHAUDIOSOURCE_H_INCLUDED
#define SAMPLESYNTHAUDIOSOURCE_H_INCLUDED

#include "JuceHeader.h"
#include "SampleSynth.h"

/**
 *  An AudioSource that plays back a SampleSynth (which is controlled via MIDI).
 */
class SampleSynthAudioSource  : public AudioSource
{
public:
    SampleSynthAudioSource (MidiKeyboardState& keyState, int noOfVoices = 8);
    ~SampleSynthAudioSource();
    
    SampleSynth& getSynth();
    
    MidiMessageCollector* getMidiCollector();
    
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    
    void releaseResources() override;
    
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;

private:
    
    MidiMessageCollector midiCollector_;
    MidiKeyboardState& keyState_;
    SampleSynth synth_;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SampleSynthAudioSource)

};


#endif  // SAMPLESYNTHAUDIOSOURCE_H_INCLUDED

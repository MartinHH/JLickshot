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

#include "SampleSynth.h"

FixedVelocitySound::FixedVelocitySound(const File& audioFile,
                                       AudioFormatReader& source,
                                       const BigInteger& midiNotes,
                                       int midiNoteForNormalPitch,
                                       double attackTimeSecs,
                                       double releaseTimeSecs,
                                       double maxSampleLengthSeconds,
                                       float velocity):
    SamplerSound::SamplerSound(audioFile.getFileName(), source, midiNotes,
                               midiNoteForNormalPitch, attackTimeSecs,
                               releaseTimeSecs, maxSampleLengthSeconds),
    velocity_(velocity),
    af_(audioFile)
{
}

FixedVelocitySound::~FixedVelocitySound()
{
}

void FixedVelocitySound::setVelocity(float velocity)
{
    if (velocity < 0.0) {
        velocity_ = 0.0;
    } else if(velocity > 1.0) {
        velocity_ = 1.0;
    } else {
        velocity_ = velocity;
    }
}

float FixedVelocitySound::getVelocity() const
{
    return velocity_;
}

const File& FixedVelocitySound::getAudioFile() const
{
    return af_;
}

//==============================================================================

FixedVelocityVoice::FixedVelocityVoice()
{
}

FixedVelocityVoice::~FixedVelocityVoice()
{
}

bool FixedVelocityVoice::canPlaySound (SynthesiserSound* sound)
{
    return dynamic_cast<const FixedVelocitySound*> (sound) != nullptr;
}

void FixedVelocityVoice::startNote(int midiNoteNumber, float velocity,
                                   SynthesiserSound* sound, int pitchWheel)
{
    const FixedVelocitySound* s =
        dynamic_cast<const FixedVelocitySound*> (sound);
    
    if (s != nullptr) {
        SamplerVoice::startNote(midiNoteNumber, s->getVelocity(), sound, pitchWheel);
    } else {
        SamplerVoice::startNote(midiNoteNumber, velocity, sound, pitchWheel);
    }
}

//==============================================================================

SampleSynth::SampleSynth(int numberOfVoices)
{
    // add specified number of voices:
    for(int i=0; i<numberOfVoices; i++){
        addVoice (new FixedVelocityVoice());
    }
    
    // add 128 SilentSounds so that assigning of sounds by note number will work
    // (128 sounds in notes array -> array index = note number)
    for(int i=0; i<NUMBER_OF_NOTES; i++){
        addSound(new SilentSound());
    }
    
    // intialise format manager that is used for loading audio files:
    formatManager_.registerBasicFormats();
}

SampleSynth::~SampleSynth()
{
}

bool SampleSynth::setSample(int noteNo, const juce::File& audioFile, float velocity)
{
    if(noteNo < 0 || noteNo >= NUMBER_OF_NOTES){
        return false;
    }
    
    ScopedPointer<AudioFormatReader> reader =
        formatManager_.createReaderFor(audioFile);
    
    if (reader == nullptr) {
        return false;
    }
    
    BigInteger note;
    note.setBit(noteNo);
        
    const SynthesiserSound::Ptr& newSound =
        new FixedVelocitySound (audioFile, *reader, note, noteNo,
                                0.001, 0.001, 60.0, velocity);
    setSound(noteNo, newSound);
    
    return true;
}

const String& SampleSynth::getFilePath(int noteNo) const
{
    if(noteNo < 0 || noteNo >= NUMBER_OF_NOTES){
        return String::empty;
    }
    
    SynthesiserSound::Ptr s = sounds[noteNo];
    FixedVelocitySound* fixedS = dynamic_cast<FixedVelocitySound*>(s.get());
    
    if(fixedS == nullptr){
        return String::empty;
    }

    return fixedS->getAudioFile().getFullPathName();
}

const String SampleSynth::getFileName(int noteNo) const
{
    if(noteNo < 0 || noteNo >= NUMBER_OF_NOTES){
        return String::empty;
    }
    
    SynthesiserSound::Ptr s = sounds[noteNo];
    FixedVelocitySound* fixedS = dynamic_cast<FixedVelocitySound*>(s.get());
    
    if(fixedS == nullptr){
        return String::empty;
    }
    
    return fixedS->getAudioFile().getFileName();
}

bool SampleSynth::setVelocity(int noteNo, float velocity)
{
    if(noteNo < 0 || noteNo >= NUMBER_OF_NOTES){
        return false;
    }
    
    SynthesiserSound::Ptr s = sounds[noteNo];
    FixedVelocitySound* fixedS = dynamic_cast<FixedVelocitySound*>(s.get());
    
    if(fixedS == nullptr){
        return false;
    }
    
    fixedS->setVelocity(velocity);
    
    return true;
}

float SampleSynth::getVelocity(int noteNo) const
{
    if(noteNo < 0 || noteNo >= NUMBER_OF_NOTES){
        return 0.0;
    }
    
    SynthesiserSound::Ptr s = sounds[noteNo];
    FixedVelocitySound* fixedS = dynamic_cast<FixedVelocitySound*>(s.get());
    
    if(fixedS == nullptr){
        return 0.0;
    }
    
    return fixedS->getVelocity();
}

bool SampleSynth::sampleIsLoaded(int noteNo) const
{
    if(noteNo < 0 || noteNo >= NUMBER_OF_NOTES){
        return false;
    }
    
    // if no sample is loaded, a SilentSound is loaded and this cast will fail:
    return (dynamic_cast<FixedVelocitySound*>(sounds[noteNo].get()) != nullptr);
}

void SampleSynth::noteOff(const int midiChannel, const int midiNoteNumber,
                          const bool allowTailOff)
{
    // for one-shot-behaviour, noteOff is ignored...
}

void SampleSynth::setSound(int index, const SynthesiserSound::Ptr &newSound)
{
    const ScopedLock sl (lock);
    sounds.set(index, newSound);
}
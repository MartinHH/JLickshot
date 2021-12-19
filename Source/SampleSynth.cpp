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
    SamplerSound(audioFile.getFileName(), source, midiNotes,
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
    
    // create AudioFormatReader:
    ScopedPointer<AudioFormatReader> reader(formatManager_.createReaderFor(audioFile));
    
    if (reader == nullptr) {
        return false;
    }
    
    // convert noteNo to BigInteger (required by upcoming constructor call):
    BigInteger note;
    note.setBit(noteNo);
    
    // create new SynthesizerSound object:
    const SynthesiserSound::Ptr& newSound =
        new FixedVelocitySound (audioFile, *reader, note, noteNo,
                                0.001, 0.001, 60.0, velocity);
    // add it to Synth:
    setSound(noteNo, newSound);
    
    return true;
}

void SampleSynth::clearSamples()
{
    const ScopedLock sl (lock);
    
    // clear all samples:
    sounds.clear();
    
    // add new dummy sounds:
    for(int i=0; i<NUMBER_OF_NOTES; i++){
        addSound(new SilentSound());
    }
}

const File& SampleSynth::getAudioFile(int noteNo) const
{
    const FixedVelocitySound* s = getFixedVelocitySound(noteNo);
    
    if(s == nullptr){
        return File();
    }
    
    return s->getAudioFile();
}

const String& SampleSynth::getFilePath(int noteNo) const
{
    return getAudioFile(noteNo).getFullPathName();
}

const String SampleSynth::getFileName(int noteNo) const
{
    return getAudioFile(noteNo).getFileName();
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
    const FixedVelocitySound* s = getFixedVelocitySound(noteNo);
    
    if(s == nullptr){
        return 0.0;
    }
    
    return s->getVelocity();
}

bool SampleSynth::sampleIsLoaded(int noteNo) const
{
    return getFixedVelocitySound(noteNo) != nullptr;
}

void SampleSynth::noteOff(const int /* midiChannel */,
                          const int /* midiNoteNumber */,
                          const float /* velocity */,
                          const bool /* allowTailOff */)
{
    // for one-shot-behaviour, noteOff is ignored...
}

XmlElement* SampleSynth::getStateXml(bool oneDir, const File& dir) const
{
    XmlElement* rv = new XmlElement("SAMPLES");

    for(int i=0; i<NUMBER_OF_NOTES; i++){
        if (sampleIsLoaded(i)) {
            XmlElement* sample = new XmlElement("SAMPLE");
            
            const File f = getAudioFile(i);
            if(oneDir){
                if(f.getParentDirectory() != dir){
                    const File dest = dir.getChildFile(f.getFileName());
                    f.copyFileTo(dest);
                }
                sample->setAttribute("path", f.getFileName());
            } else {
                sample->setAttribute("path", f.getFullPathName());
            }
            
            sample->setAttribute("note", i);
            sample->setAttribute("velocity", getVelocity(i));
            rv->addChildElement(sample);
        }
    }
    
    return rv;
}

SampleSynth::LoadResult SampleSynth::updateFromXml(juce::XmlElement *stateXml,
                                                   bool fromDir, const File& dir)
{
    LoadResult rv = LoadResult();
    
    if(stateXml != nullptr && stateXml->hasTagName("SAMPLES")){
        // iterate over sample elements:
        XmlElement* sample = stateXml->getChildByName("SAMPLE");
        while (sample != nullptr) {
            
            // extract note number:
            const int noteNo = sample->getIntAttribute("note", -1);
            
            if (noteNo >=0 && noteNo < NUMBER_OF_NOTES) {
                
                // extract file path and velocity:
                String path = sample->getStringAttribute("path", String());
                float velocity =
                    (float) sample->getDoubleAttribute("velocity", getVelocity(noteNo));
                
                const File file = fromDir ? dir.getChildFile(path) : File(path);
                
                // set sample and count result:
                if(setSample(noteNo, file, velocity)){
                    rv.loaded++;
                } else {
                    rv.failed++;
                }
            }
            
            // next xml element to keep the iteration going:
            sample = sample->getNextElementWithTagName("SAMPLE");
        }
        
        rv.success = true;
    }
    
    return rv;
}

void SampleSynth::setSound(int index, const SynthesiserSound::Ptr &newSound)
{
    const ScopedLock sl (lock);
    sounds.set(index, newSound);
}

const FixedVelocitySound* SampleSynth::getFixedVelocitySound(int noteNo) const
{
    if(noteNo < 0 || noteNo >= NUMBER_OF_NOTES){
        return nullptr;
    }
    
    SynthesiserSound::Ptr s = sounds[noteNo];
    return dynamic_cast<FixedVelocitySound*>(s.get());
}

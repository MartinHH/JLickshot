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

#include "JLickshotProcessorBase.h"

JLickshotProcessorBase::JLickshotProcessorBase(int noOfVoices):
    synth_(noOfVoices),
    delayIsActive_(false),
    reverbIsActive_(false),
    gain_(1.0),
    lastGain_(gain_),
    samplesChanged_(1) // 1 makes sure the gui syncs itself at construction
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
    
    const int channels = jmin(buffer.getNumChannels(), delay_.getNumChannels());
    for(int i = 0; i < channels; i++){
        buffer.applyGainRamp (i, 0, numSamples, lastGain_, gain_);
    }
    lastGain_ = gain_;
}

void JLickshotProcessorBase::setSampleRate(double sampleRate)
{
    synth_.setCurrentPlaybackSampleRate (sampleRate);
    delay_.setSampleRate((int) sampleRate);
    mVerb_.setSampleRate((float) sampleRate);
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

void JLickshotProcessorBase::setMasterGain(float gain)
{
    gain_ = gain;
}

float JLickshotProcessorBase::getMasterGain() const
{
    return gain_;
}

XmlElement* JLickshotProcessorBase::addStateXmlElements(XmlElement *xml,
                                                        bool oneDir,
                                                        const File& dir)
{
    // add general settings:
    XmlElement* generalSettings = new XmlElement("GENERAL");
    generalSettings->setAttribute("delay_active", delayIsActive_);
    generalSettings->setAttribute("reverb_active", reverbIsActive_);
    generalSettings->setAttribute("gain", gain_);
    xml->addChildElement(generalSettings);
    
    // add sample settings:
    xml->addChildElement(synth_.getStateXml(oneDir, dir));
    
    // add delay setting:
    xml->addChildElement(delay_.getStateXml());
    
    // add reverb settings:
    xml->addChildElement(mVerb_.getStateXml());
    
    return xml;
}

SampleSynth::LoadResult JLickshotProcessorBase::updateFromXml(const XmlElement *stateXml,
                                                              bool fromDir,
                                                              const File& dir)
{
    // this should have been cheked by the caller, but anyway:
    if (stateXml == nullptr || !stateXml->hasTagName("JLICKSHOTSETTINGS")) {
        return SampleSynth::LoadResult();
    }
    
    const XmlElement* generalSettings = stateXml->getChildByName("GENERAL");
    if(generalSettings != nullptr){
        delayIsActive_ = generalSettings->getBoolAttribute("delay_active", delayIsActive_);
        reverbIsActive_ = generalSettings->getBoolAttribute("reverb_active",reverbIsActive_);
        gain_ = (float) generalSettings->getDoubleAttribute("gain", gain_);
    }
    
    delay_.updateFromXml(stateXml->getChildByName("DELAYSETTINGS"));
    mVerb_.updateFromXml(stateXml->getChildByName("MVERBSETTINGS"));
    
    // clear synth:
    synth_.clearSamples();
    
    SampleSynth::LoadResult rv = synth_.updateFromXml(stateXml->getChildByName("SAMPLES"),
                                                      fromDir, dir);
    
    if(rv.success){
        samplesChanged_.set(1);
    }
    
    return rv;
}

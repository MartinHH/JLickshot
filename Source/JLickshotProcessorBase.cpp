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
    reverbIsActive_(false),
    gain_(1.0),
    lastGain_(gain_)
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

void JLickshotProcessorBase::setMasterGain(float gain)
{
    gain_ = gain;
}

float JLickshotProcessorBase::getMasterGain() const
{
    return gain_;
}

XmlElement* JLickshotProcessorBase::addXmlOfSynthAndFx(XmlElement *xml)
{
    // add sample settings:
    xml->addChildElement(synth_.getStateXml());
    
    // add delay setting:
    xml->addChildElement(delay_.getStateXml());
    
    // add reverb settings:
    xml->addChildElement(mVerb_.getStateXml());
    
    return xml;
}

SampleSynth::LoadResult JLickshotProcessorBase::updateSynthAndFxFromXml(const XmlElement *stateXml)
{
    // this should have been cheked by the caller, but anyway:
    if (stateXml == nullptr || !stateXml->hasTagName("JLICKSHOTSETTINGS")) {
        return SampleSynth::LoadResult();
    }
    
    delay_.updateFromXml(stateXml->getChildByName("DELAYSETTINGS"));
    mVerb_.updateFromXml(stateXml->getChildByName("MVERBSETTINGS"));
    
    // clear synth:
    synth_.clearSamples();
    
    return synth_.updateFromXml(stateXml->getChildByName("SAMPLES"));
}

XmlElement* JLickshotProcessorBase::createGeneralSettingsXml() const
{
    XmlElement* generalSettings = new XmlElement("GENERAL");
    generalSettings->setAttribute("delay_active", delayIsActive_);
    generalSettings->setAttribute("reverb_active", reverbIsActive_);
    return generalSettings;
}

void JLickshotProcessorBase::updateGeneralSettingsFromXml(const XmlElement *stateXml)
{
    if(stateXml != nullptr  && stateXml->hasTagName("GENERAL")){
        delayIsActive_ = stateXml->getBoolAttribute("delay_active", delayIsActive_);
        reverbIsActive_ = stateXml->getBoolAttribute("reverb_active",reverbIsActive_);
    }
}
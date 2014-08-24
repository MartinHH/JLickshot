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

#include "MainController.h"

MainController::MainController():
    aSource_(keyState_)
{
    // initialise audio device manager:
    adm_.initialise (2, 2, nullptr, true, String::empty, 0);

    // set SampleSynthAudioSource as source of the AudioSourcePlayer:
    aSourcePlayer_.setSource(&aSource_);
    
    // add callbacks to audio device manager:
    adm_.addAudioCallback(&aSourcePlayer_);
    adm_.addMidiInputCallback(String::empty, aSource_.getMidiCollector());
}

MainController::~MainController()
{
    adm_.removeAudioCallback(&aSourcePlayer_);
    adm_.removeMidiInputCallback(String::empty, aSource_.getMidiCollector());
}

AudioDeviceManager& MainController::getAudioDeviceManager()
{
    return adm_;
}

MidiKeyboardState& MainController::getKeyState()
{
    return keyState_;
}

SampleSynthAudioSource& MainController::getAudioSource()
{
    return aSource_;
}

const SampleSynth& MainController::getSynth()
{
    return aSource_.getSynth();
}

void MainController::velocityChanged(SampleChoiceComponent *sampleChoiceComponent)
{
    // set the sample's velocity for the corresponding note number:
    aSource_.getSynth().setVelocity(sampleChoiceComponent->getNoteNumber(),
                                    sampleChoiceComponent->getVelocity());
}

void MainController::fileChosen(SampleChoiceComponent *sampleChoiceComponent,
                                const File& audioFile)
{
    // get noteNo and velocity:
    const int note = sampleChoiceComponent->getNoteNumber();
    const float v = sampleChoiceComponent->getVelocity();
    
    // set new sample:
    if(aSource_.getSynth().setSample(note, audioFile, v)){
        // on success, update GUI:
        sampleChoiceComponent->setFileName(audioFile.getFileName());
    }
}

void MainController::activationStatusChanged(bool delaySetActive)
{
    aSource_.setDelayIsActive(delaySetActive);
}

void MainController::delayTimeChanged(float delay)
{
    aSource_.getDelayUnit().setLength(delay);
}

void MainController::feedbackChanged(float feedback)
{
    aSource_.getDelayUnit().setFeedback(feedback);
}

void MainController::setMasterGain(float gain)
{
    aSourcePlayer_.setGain(gain);
}

float MainController::getMasterGain() const
{
    return aSourcePlayer_.getGain();
}

bool MainController::saveState(const File& xmlDest)
{
    // create main xml element:
    XmlElement xml ("JLICKSHOTSETTINGS");
    
    // add state of audio device manager:
    xml.addChildElement(adm_.createStateXml());
    
    // add main settings:
    XmlElement* generalSettings = new XmlElement("GENERAL");
    generalSettings->setAttribute("gain", aSourcePlayer_.getGain());
    generalSettings->setAttribute("delay_active", aSource_.getDelayIsActive());
    xml.addChildElement(generalSettings);
    
    // add sample settings:
    xml.addChildElement(aSource_.getSynth().getStateXml());
    
    // add delay setting:
    xml.addChildElement(aSource_.getDelayUnit().getStateXml());
    
    return xml.writeToFile(xmlDest, String::empty); // TODO
}

SampleSynth::LoadResult MainController::loadState(const File& xmlSource)
{
    ScopedPointer<const XmlElement> xml = XmlDocument::parse(xmlSource);
    
    if (xml == nullptr || !xml->hasTagName("JLICKSHOTSETTINGS")) {
        return SampleSynth::LoadResult();
    }
    
    const XmlElement* admSettings = xml->getChildByName("DEVICESETUP");
    if(admSettings != nullptr){
        adm_.initialise(0, 2, admSettings, true, String::empty, 0);
    }
    
    const XmlElement* generalSettings = xml->getChildByName("GENERAL");
    if(generalSettings != nullptr){
        const double gain =
            generalSettings->getDoubleAttribute("gain", aSourcePlayer_.getGain());
        aSourcePlayer_.setGain(gain);
        
        const bool delActive =
            generalSettings->getBoolAttribute("delay_active", aSource_.getDelayIsActive());
        aSource_.setDelayIsActive(delActive);
    }
    
    aSource_.getDelayUnit().updateFromXml(xml->getChildByName("DELAYSETTINGS"));
    
    SampleSynth& synth = aSource_.getSynth();
    
    // clear synth:
    synth.clearSamples();
    
    return synth.updateFromXml(xml->getChildByName("SAMPLES"));
}
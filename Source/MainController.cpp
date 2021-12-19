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

#include "MainController.h"

MainController::MainController()
{
    // initialise audio device manager:
    adm_.initialise (2, 2, nullptr, true, String(), nullptr);

    // set SampleSynthAudioSource as source of the AudioSourcePlayer:
    aSourcePlayer_.setSource(&aSource_);
    
    // add callbacks to audio device manager:
    adm_.addAudioCallback(&aSourcePlayer_);
    adm_.addMidiInputCallback(String(), aSource_.getMidiCollector());
}

MainController::~MainController()
{
    adm_.removeAudioCallback(&aSourcePlayer_);
    adm_.removeMidiInputCallback(String(), aSource_.getMidiCollector());
}

AudioDeviceManager& MainController::getAudioDeviceManager()
{
    return adm_;
}

JLickshotProcessorBase& MainController::getProcessor(){
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

void MainController::delayActivationChanged(bool delaySetActive)
{
    aSource_.setDelayIsActive(delaySetActive);
}

void MainController::delayTimeChanged(float delay)
{
    aSource_.getDelayUnit().setRelativeTime(delay);
}

void MainController::feedbackChanged(float feedback)
{
    aSource_.getDelayUnit().setFeedback(feedback);
}

void MainController::lowpassActivatedChanged(bool lpSetActive)
{
    aSource_.getDelayUnit().setLowpassIsActive(lpSetActive);
}

void MainController::lowpassFrequencyChanged(float frequency)
{
    aSource_.getDelayUnit().setLowpassFrequency(frequency);
}

void MainController::reverbActivationChanged(bool delaySetActive)
{
    aSource_.setReverbIsActive(delaySetActive);
}

void MainController::dampingChanged(float newValue)
{
    aSource_.getMVerb().setParameter(MVerb<float>::DAMPINGFREQ, newValue);
}

void MainController::densityChanged(float newValue)
{
    aSource_.getMVerb().setParameter(MVerb<float>::DENSITY, newValue);
}

void MainController::bandwidthChanged(float newValue)
{
    aSource_.getMVerb().setParameter(MVerb<float>::BANDWIDTHFREQ, newValue);
}

void MainController::predelayChanged(float newValue)
{
    aSource_.getMVerb().setParameter(MVerb<float>::PREDELAY, newValue);
}

void MainController::sizeChanged(float newValue)
{
    aSource_.getMVerb().setParameter(MVerb<float>::SIZE, newValue);
}

void MainController::decayChanged(float newValue)
{
    aSource_.getMVerb().setParameter(MVerb<float>::DECAY, newValue);
}

void MainController::gainChanged(float newValue)
{
    aSource_.getMVerb().setParameter(MVerb<float>::GAIN, newValue);
}

void MainController::mixChanged(float newValue)
{
    aSource_.getMVerb().setParameter(MVerb<float>::MIX, newValue);
}

void MainController::earlyLateMixChanged(float newValue)
{
    aSource_.getMVerb().setParameter(MVerb<float>::EARLYMIX, newValue);
}

void MainController::setMasterGain(float gain)
{
    aSource_.setMasterGain(gain);
}

float MainController::getMasterGain() const
{
    return aSource_.getMasterGain();
}

bool MainController::saveState(const File& xmlDest, bool toOneDir)
{
    // create main xml element:
    XmlElement xml ("JLICKSHOTSETTINGS");
    
    xml.setAttribute(translate("one_dir"), toOneDir);
    
    // add state of audio device manager:
    const std::unique_ptr<XmlElement> stateXml = adm_.createStateXml();
    xml.addChildElement(stateXml.get());
    
    // add all other settings:
    if(toOneDir){
        aSource_.addStateXmlElements(&xml, toOneDir, xmlDest.getParentDirectory());
    } else {
        aSource_.addStateXmlElements(&xml);
    }
    
    return xml.writeToFile(xmlDest, String()); // TODO
}

SampleSynth::LoadResult MainController::loadState(const File& xmlSource)
{
    const std::unique_ptr<XmlElement> xml = XmlDocument::parse(xmlSource);
    
    if (xml == nullptr || !xml->hasTagName("JLICKSHOTSETTINGS")) {
        return SampleSynth::LoadResult();
    }
    
    const bool fromDir = xml->getBoolAttribute("one_dir", false);
    
    const XmlElement* admSettings = xml->getChildByName("DEVICESETUP");
    if(admSettings != nullptr){
        adm_.initialise(0, 2, admSettings, true, String(), nullptr);
    }
        
    return aSource_.updateFromXml(xml.get(), fromDir, xmlSource.getParentDirectory());
}

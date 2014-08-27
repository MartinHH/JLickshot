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

#include "MainComponent.h"

MainContentComponent::MainContentComponent(MainController* controller):
    controller_(controller)
{
    aDevSelector_ =
        new AudioDeviceSelectorComponent(controller->getAudioDeviceManager(),
                                         0, 0, 0, 2,
                                         true, false, true, false);
    
    sampleComponent_ = new SampleCollectionComponent();
    sampleComponent_->addListener(controller);

    viewport_ = new Viewport();
    viewport_->setViewedComponent(sampleComponent_, false);
    addAndMakeVisible(viewport_);
    
    loadButton_ = new TextButton();
    loadButton_->setButtonText(translate("LOAD"));
    loadButton_->addListener(this);
    loadButton_->setTooltip(translate("Load a previously saved setup"));
    addAndMakeVisible(loadButton_);
    
    saveButton_ = new TextButton();
    saveButton_->setButtonText(translate("SAVE"));
    saveButton_->addListener(this);
    saveButton_->setTooltip(translate("Save a setup"));
    addAndMakeVisible(saveButton_);
    
    settingsButton_ = new TextButton();
    settingsButton_->setButtonText(translate("SETTINGS"));
    settingsButton_->addListener(this);
    settingsButton_->setTooltip(translate("Audio and Midi setting"));
    addAndMakeVisible(settingsButton_);
    
    addAndMakeVisible (gainSlider_ = new Slider ("gain slider"));
    gainSlider_->setRange (0, 1.0, 0.01);
    gainSlider_->setValue(0.75);
    gainSlider_->setSliderStyle (Slider::LinearBar);
    gainSlider_->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    gainSlider_->setColour(Slider::backgroundColourId , Colours::white);
    gainSlider_->setTooltip(translate("Master gain"));
    gainSlider_->addListener (this);
    
    delayComponent_ = new DelayComponent();
    delayComponent_->addListener(controller);
    addAndMakeVisible(delayComponent_);
    
    mVerbComponent_ = new MVerbComponent();
    mVerbComponent_->addListener(controller);
    addAndMakeVisible(mVerbComponent_);
    
    keyboard_ = new MidiKeyboardComponent(controller->getKeyState(),
                                          MidiKeyboardComponent::horizontalKeyboard);
    addAndMakeVisible(keyboard_);
    
    laf_ = new LookAndFeel_V3();
    setLookAndFeel(laf_);
    
    setSize (970, 410);
}

MainContentComponent::~MainContentComponent()
{
}

void MainContentComponent::paint (Graphics& g)
{
    g.fillAll (Colours::lightgrey);

    g.setFont (Font (16.0f));
    g.setColour (Colours::black);
}

void MainContentComponent::resized()
{
    // positioned from top:
    loadButton_->setBounds(10, 5, 140, 24);
    saveButton_->setBounds(155, 5, 140, 24);
    settingsButton_->setBounds(305, 5, 140, 24);
    gainSlider_->setBounds(455, 5, 60, 24);
    delayComponent_->setBounds(530, 10, 430, 124);
    mVerbComponent_->setBounds(530, 10 + delayComponent_->getHeight(),
                               430, 214);
    
    // positioned from bottom:
    keyboard_->setBounds(10, getHeight()-64, getWidth()-20, 64);

    
    // takes up the remaining space on the left:
    viewport_->setBounds(10, 34, 520,
                         getHeight() - 34 - 10 - keyboard_->getHeight());

}

void MainContentComponent::sliderValueChanged(juce::Slider *sliderThatWasMoved)
{
    if (sliderThatWasMoved == gainSlider_){
        controller_->setMasterGain(gainSlider_->getValue());
    }
}

void MainContentComponent::buttonClicked (Button* buttonThatWasClicked)
{
    if(buttonThatWasClicked == loadButton_){
        // pop up a file chooser dialog:
        FileChooser fc ("Choose a file to load...",
                        File::getCurrentWorkingDirectory(),
                        "*.xml",
                        true);
        
        if(fc.browseForFileToOpen()){
            // file has been chosen:
            File chosenFile = fc.getResult();
            // make controller load the settings:
            SampleSynth::LoadResult rv = controller_->loadState(chosenFile);
            // update the gui to the new state:
            updateFromController();
            // show result report:
            popUpLoadResult(rv);
        }
    } else if(buttonThatWasClicked == saveButton_){
        // pop up a file chooser dialog:
        FileChooser fc ("Choose a file to save...",
                        File::getCurrentWorkingDirectory(),
                        "*.xml",
                        true);
        
        if (fc.browseForFileToSave (true)){
            // file has been chosen:
            File chosenFile = fc.getResult();
            // make controller save its state:
            controller_->saveState(chosenFile);
        }
    } else if (buttonThatWasClicked == settingsButton_){
        // create an alert window:
        AlertWindow w ("Audio Settings",
                       String::empty,
                       AlertWindow::NoIcon);
        aDevSelector_->setSize(300, 250);
        w.setLookAndFeel(laf_);
        
        // add the AudioDeviceSelectorComponent to it:
        w.addCustomComponent(aDevSelector_);
        // add a close button
        w.addButton ("CLOSE", 1, KeyPress (KeyPress::returnKey, 0, 0));
        // pop it up:
        w.runModalLoop();
    }
}

void MainContentComponent::updateFromController()
{
    // update the gain slider:
    gainSlider_->setValue(controller_->getMasterGain());
    // update the SampleCollectionComponent:
    sampleComponent_->updateFromSynth(controller_->getSynth());
    // update the DelayComponent:
    delayComponent_->updateFromAudioSource(controller_->getAudioSource());
}

void MainContentComponent::popUpLoadResult(SampleSynth::LoadResult result)
{
    if(result.success){
        AlertWindow::showMessageBoxAsync(AlertWindow::InfoIcon,
                                         "Setup loaded successfully",
                                         String(result.loaded)
                                         + " samples loaded.\n"
                                         + (result.failed ? String(result.failed)
                                            + " samples could not be loaded."
                                            : String::empty),
                                         "ok");
    } else {
        AlertWindow::showMessageBoxAsync(AlertWindow::WarningIcon,
                                         "Loading failed",
                                         "An error occured loading the setup file."
                                         "ok");
    }
}
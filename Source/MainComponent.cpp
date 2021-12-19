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

MainContentComponent::MainContentComponent(JLickshotControllerBase* controller,
                                           AudioDeviceManager* adm):
    controller_(controller)
{
    if(adm != nullptr){
        aDevSelector_ = new AudioDeviceSelectorComponent(*adm, 0, 0, 0, 2,
                                                         true, false,
                                                         true, false);
        
        settingsButton_ = new TextButton();
        settingsButton_->setButtonText(translate("SETTINGS"));
        settingsButton_->addListener(this);
        settingsButton_->setTooltip(translate("Audio and Midi setting"));
        addAndMakeVisible(settingsButton_);
    }
    
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
    
    keyboard_ = new MidiKeyboardComponent(controller->getProcessor().getKeyState(),
                                          MidiKeyboardComponent::horizontalKeyboard);
    addAndMakeVisible(keyboard_);
    
    updateFromController();
    
    laf_ = new LookAndFeel_V3();
    setLookAndFeel(laf_);
    
    setSize (910, 410);
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
    const int leftColWidth = 460; // TODO: sensible calculation for this
    
    // starting from top
    if (settingsButton_ == nullptr) {
        const int buttonWidth = (leftColWidth - 90) / 2;
        loadButton_->setBounds(10, 5, buttonWidth, 24);
        saveButton_->setBounds(loadButton_->getX() + buttonWidth + 5,
                               5, buttonWidth, 24);
    } else {
        const int buttonWidth = (leftColWidth - 100) / 3;
        loadButton_->setBounds(10, 5, buttonWidth, 24);
        saveButton_->setBounds(loadButton_->getX() + buttonWidth + 5,
                               5, buttonWidth, 24);
        settingsButton_->setBounds(saveButton_->getX() + buttonWidth + 5,
                                   5, buttonWidth, 24);
    }
    gainSlider_->setBounds(leftColWidth - 70, 5, 65, 24);
    delayComponent_->setBounds(leftColWidth+10, 10, 430, 124);
    mVerbComponent_->setBounds(leftColWidth+10, 10 + delayComponent_->getHeight(),
                               430, 214);
    
    // now continuing from bottom:
    keyboard_->setBounds(10, getHeight()-64, getWidth()-20, 64);

    
    // takes up the remaining space on the left:
    viewport_->setBounds(10, 34, leftColWidth,
                         getHeight() - 34 - 10 - keyboard_->getHeight());
    // adjust width of contained component:
    sampleComponent_->setSize(viewport_->getWidth()-18, sampleComponent_->getHeight());

}

void MainContentComponent::sliderValueChanged(juce::Slider *sliderThatWasMoved)
{
    if (sliderThatWasMoved == gainSlider_){
        controller_->getProcessor().setMasterGain((float) gainSlider_->getValue());
    }
}

void MainContentComponent::buttonClicked (Button* buttonThatWasClicked)
{
    if(buttonThatWasClicked == loadButton_){
        // pop up a file chooser dialog:
        fileCooser_ = std::make_unique<FileChooser>("Choose a file to load...",
                                                    File::getCurrentWorkingDirectory(),
                                                    "*.xml",
                                                    true);
        auto folderChooserFlags = FileBrowserComponent::openMode | FileBrowserComponent::canSelectFiles;
        
        fileCooser_->launchAsync(folderChooserFlags, [this] (const FileChooser& chooser)
        {
            // file has been chosen:
            File chosenFile = chooser.getResult();
            if(!chosenFile.getFullPathName().isEmpty()) {
                // make controller load the settings:
                SampleSynth::LoadResult rv = controller_->loadState(chosenFile);
                // update the gui to the new state:
                updateFromController();
                // show result report:
                popUpLoadResult(rv);
            }
        }
        );

    } else if (buttonThatWasClicked == saveButton_){
    
        // TODO: askAboutSavingToDir() now is async, returns immediately:
        launchSaveDialog();
        /*
        // pop up a file chooser dialog:
        fileCooser_ = std::make_unique<FileChooser>("Choose a file to save...",
                                                    File::getCurrentWorkingDirectory(),
                                                    "*.xml",
                                                    true);
        
        auto folderChooserFlags = FileBrowserComponent::openMode | FileBrowserComponent::canSelectFiles;
        
        fileCooser_->launchAsync(folderChooserFlags, [this, td=toDir] (const FileChooser& chooser)
        {
            // file has been chosen:
            File chosenFile = chooser.getResult();
            if(!chosenFile.getFullPathName().isEmpty()) {
                // make controller save its state:
                controller_->saveState(chosenFile, td);
            }
        }
        );*/
    } else if (buttonThatWasClicked == settingsButton_){
        aDevSelector_->setSize(500, 500);
        DialogWindow::showDialog("Audio Settings", aDevSelector_, this, Colours::darkgrey, true, false, false);
    }
}

void MainContentComponent::updateFromController()
{
    JLickshotProcessorBase& proc = controller_->getProcessor();
    
    // update the gain slider:
    gainSlider_->setValue(proc.getMasterGain());
    if(proc.samplesChanged()){
        // update the SampleCollectionComponent:
        sampleComponent_->updateFromSynth(proc.getSynth());
    }
    // update the DelayComponent:
    delayComponent_->updateFromProcessor(proc);
    // update the MVerbComponent
    mVerbComponent_->updateFromProcessor(proc);
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
                                            : String()),
                                         "ok");
    } else {
        AlertWindow::showMessageBoxAsync(AlertWindow::WarningIcon,
                                         "Loading failed",
                                         "An error occured loading the setup file."
                                         "ok");
    }
}

void MainContentComponent::launchSaveDialog()
{
    // callback that will launch the filebwrowser after the user has chosen whether to save presets
    // alone or to also copy the samples
    ModalComponentManager::Callback* callback = ModalCallbackFunction::create([this] (int toFile)
        {
            // pop up a file chooser dialog:
            fileCooser_ = std::make_unique<FileChooser>("Choose a file to save...",
                                                        File::getCurrentWorkingDirectory(),
                                                        "*.xml",
                                                        true);
        
            auto folderChooserFlags = FileBrowserComponent::saveMode | FileBrowserComponent::canSelectFiles;
        
            fileCooser_->launchAsync(folderChooserFlags, [this, tf=toFile] (const FileChooser& chooser)
            {
                // file has been chosen:
                File chosenFile = chooser.getResult();
                if(!chosenFile.getFullPathName().isEmpty()) {
                    // make controller save its state:
                    controller_->saveState(chosenFile, !tf);
                }
            });
            
        });
    AlertWindow::showOkCancelBox (AlertWindow::QuestionIcon,
                                  "Save a simple preset or all files to one directory?",
                                  translate("Do you want to save just the preset and keep all files where they are, or\n")
                                    + "do you want to save everything (preset and audiofiles) into one directory?\n\n"
                                    + "(Saving everything to one directory will enable you to move your preset\n"
                                    + "around and even load it on other machines.)",
                                    "Just the preset",
                                    "  Everything   ",
                                    nullptr,
                                    callback);
}

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

#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "JuceHeader.h"
#include "MainController.h"
#include "SampleCollectionComponent.h"
#include "DelayComponent.h"
#include "MVerbComponent.h"
#include "JLickshotControllerBase.h"

/**
 *  This is the main GUI component that contains any subcomponents. It holds
 *  a pointer to the main JLickshotControllerBase controller.
 */
class MainContentComponent   :  public Component,
                                public Slider::Listener,
                                public Button::Listener
{
public:

    MainContentComponent(JLickshotControllerBase* controller,
                         AudioDeviceManager* adm = nullptr);
    ~MainContentComponent();
    
    void paint (Graphics&);
    void resized();
    
    void sliderValueChanged (Slider* sliderThatWasMoved);
    void buttonClicked (Button* buttonThatWasClicked);

private:
    /** Updates all GUI elements to the state of the MainController. */
    void updateFromController();
    
    /** Opens a popup window displaying the results of loading a preset. */
    void popUpLoadResult(SampleSynth::LoadResult result);
    
    void launchSaveDialog();
    
    std::unique_ptr<LookAndFeel> laf_;
    std::unique_ptr<AudioDeviceSelectorComponent> aDevSelector_;
    std::unique_ptr<SampleCollectionComponent> sampleComponent_;
    std::unique_ptr<Viewport> viewport_;
    std::unique_ptr<MidiKeyboardComponent> keyboard_;
    std::unique_ptr<TextButton> loadButton_;
    std::unique_ptr<TextButton> saveButton_;
    std::unique_ptr<TextButton> settingsButton_;
    std::unique_ptr<Slider> gainSlider_;
    std::unique_ptr<DelayComponent> delayComponent_;
    std::unique_ptr<MVerbComponent> mVerbComponent_;
    std::unique_ptr<FileChooser> fileCooser_;
    JLickshotControllerBase* controller_;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


#endif  // MAINCOMPONENT_H_INCLUDED

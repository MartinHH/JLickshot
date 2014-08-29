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

#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "JuceHeader.h"
#include "MainController.h"
#include "SampleCollectionComponent.h"
#include "DelayComponent.h"
#include "MVerbComponent.h"
#include "JLickshotProcessorBase.h"

/**
 *  This is the main GUI component that contains any subcomponents. It holds
 *  a pointer to the MainController.
 */
class MainContentComponent   :  public Component,
                                public SliderListener,
                                public ButtonListener
{
public:

    MainContentComponent(MainController* controller);
    ~MainContentComponent();
    
    void paint (Graphics&);
    void resized();
    
    void sliderValueChanged (Slider* sliderThatWasMoved);
    void buttonClicked (Button* buttonThatWasClicked);

private:
    /** Updates all GUI elements to the state of the MainController. */
    void updateFromController();
    
    void popUpLoadResult(SampleSynth::LoadResult result);
    
    ScopedPointer<LookAndFeel> laf_;
    ScopedPointer<AudioDeviceSelectorComponent> aDevSelector_;
    ScopedPointer<SampleCollectionComponent> sampleComponent_;
    ScopedPointer<Viewport> viewport_;
    ScopedPointer<MidiKeyboardComponent> keyboard_;
    ScopedPointer<TextButton> loadButton_;
    ScopedPointer<TextButton> saveButton_;
    ScopedPointer<TextButton> settingsButton_;
    ScopedPointer<Slider> gainSlider_;
    ScopedPointer<DelayComponent> delayComponent_;
    ScopedPointer<MVerbComponent> mVerbComponent_;
    MainController* controller_;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


#endif  // MAINCOMPONENT_H_INCLUDED

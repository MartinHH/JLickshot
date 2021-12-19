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

#ifndef DELAYCOMPONENT_H_INCLUDED
#define DELAYCOMPONENT_H_INCLUDED

#include "JuceHeader.h"
#include "Defines.h"
#include "JLickshotProcessorBase.h"
#include "JLickshotComponent.h"

class DelayComponent  : public JLickshotComponent,
                        public Button::Listener
{
public:
    DelayComponent ();
    ~DelayComponent();
    
    class Listener
    {
    public:
        virtual ~Listener(){}
        
        virtual void delayActivationChanged(bool delaySetActive) = 0;
        
        virtual void delayTimeChanged(float delay) = 0;
        
        virtual void feedbackChanged(float feedback) = 0;
        
        virtual void lowpassActivatedChanged(bool lpSetActive) = 0;
        
        virtual void lowpassFrequencyChanged(float frequency) = 0;
    };
    
    /** Adds a listener. */
    void addListener (Listener* listener);
    
    /** Removes a previously-registered listener. */
    void removeListener (Listener* listener);
    
    void paint (Graphics& g);
    void resized();
    void buttonClicked (Button* buttonThatWasClicked);
    void sliderValueChanged (Slider* sliderThatWasMoved);
    
    void updateFromProcessor(JLickshotProcessorBase& processor);
    
private:    
    ScopedPointer<GroupComponent> groupComponent_;
    ScopedPointer<ToggleButton> activateButton_;
    ScopedPointer<Slider> delaySlider_;
    ScopedPointer<Label> delayLabel;
    ScopedPointer<Slider> feedbackSlider_;
    ScopedPointer<Label> feedbackLabel_;
    ScopedPointer<ToggleButton> lowpassButton_;
    ScopedPointer<Slider> frequencySlider_;
    ScopedPointer<Label> frequencyLabel_;
    ListenerList<Listener> listeners_;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DelayComponent)
};

#endif  // DELAYCOMPONENT_H_INCLUDED

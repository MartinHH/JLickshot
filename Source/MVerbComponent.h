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

#ifndef MVERBCOMPONENT_H_INCLUDED
#define MVERBCOMPONENT_H_INCLUDED

#include "JuceHeader.h"
#include "JLickshotComponent.h"
#include "JLickshotProcessorBase.h"

class MVerbComponent    : public JLickshotComponent,
                          public ButtonListener
{
public:
    MVerbComponent();
    ~MVerbComponent();
    
    class Listener
    {
    public:
        virtual ~Listener(){};
        
        virtual void reverbActivationChanged(bool delaySetActive) = 0;
        
        virtual void dampingChanged(float newValue) = 0;
        
        virtual void densityChanged(float newValue) = 0;
        
        virtual void bandwidthChanged(float newValue) = 0;
        
        virtual void predelayChanged(float newValue) = 0;
        
        virtual void sizeChanged(float newValue) = 0;
        
        virtual void decayChanged(float newValue) = 0;
        
        virtual void gainChanged(float newValue) = 0;
        
        virtual void mixChanged(float newValue) = 0;
        
        virtual void earlyLateMixChanged(float newValue) = 0;

    };
    
    /** Adds a listener. */
    void addListener (Listener* listener);
    
    /** Removes a previously-registered listener. */
    void removeListener (Listener* listener);

    void paint (Graphics&);
    void resized();
    void buttonClicked (Button* buttonThatWasClicked);
    void sliderValueChanged (Slider* sliderThatWasMoved);
    
    void updateFromProcessor(JLickshotProcessorBase& processor);

private:
    ScopedPointer<GroupComponent> groupComponent_;
    ScopedPointer<ToggleButton> activateButton_;
    ScopedPointer<Label> gainLabel_;
    ScopedPointer<Slider> gainSlider_;
    ScopedPointer<Label> mixLabel_;
    ScopedPointer<Slider> mixSlider_;
    ScopedPointer<Label> earlyLateMixLabel_;
    ScopedPointer<Slider> earlyLateMixSlider_;
    ScopedPointer<Label> dampingLabel_;
    ScopedPointer<Slider> dampingSlider_;
    ScopedPointer<Label> densityLabel_;
    ScopedPointer<Slider> densitySlider_;
    ScopedPointer<Label> bandwidthLabel_;
    ScopedPointer<Slider> bandwidthSlider_;
    ScopedPointer<Label> predelayLabel_;
    ScopedPointer<Slider> predelaySlider_;
    ScopedPointer<Label> sizeLabel_;
    ScopedPointer<Slider> sizeSlider_;
    ScopedPointer<Label> decayLabel_;
    ScopedPointer<Slider> decaySlider_;
    ListenerList<Listener> listeners_;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MVerbComponent)
};


#endif  // MVERBCOMPONENT_H_INCLUDED

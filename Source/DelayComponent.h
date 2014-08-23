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

#ifndef DELAYCOMPONENT_H_INCLUDED
#define DELAYCOMPONENT_H_INCLUDED

#include "JuceHeader.h"

class DelayComponent  : public Component,
                        public ButtonListener,
                        public SliderListener
{
public:
    DelayComponent ();
    ~DelayComponent();
    
    void paint (Graphics& g);
    void resized();
    void buttonClicked (Button* buttonThatWasClicked);
    void sliderValueChanged (Slider* sliderThatWasMoved);
    
private:
    ScopedPointer<GroupComponent> groupComponent_;
    ScopedPointer<ToggleButton> activateButton_;
    ScopedPointer<Slider> delaySlider_;
    ScopedPointer<Label> delayLabel;
    ScopedPointer<Slider> feedbackSlider_;
    ScopedPointer<Label> feedbackLabel_;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DelayComponent)
};

#endif  // DELAYCOMPONENT_H_INCLUDED

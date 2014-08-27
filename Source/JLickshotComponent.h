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

#ifndef JLICKSHOTCOMPONENT_H_INCLUDED
#define JLICKSHOTCOMPONENT_H_INCLUDED

#include "JuceHeader.h"
#include "Defines.h"

/**
 *  Base class for components containing parameter sliders. It
 *  offers methods to create sliders and labels with standard settings
 *  and to convert slider values to [0.0 , 1.0]-normalized values.
 */
class JLickshotComponent    :   public Component,
                                public SliderListener
{
public:
    JLickshotComponent(const String& name = String::empty):
        Component(name)
    {}
    
    virtual ~JLickshotComponent(){}

protected:

    Slider* createAndAddDefaultslider(const String& name);
    
    Label* createAndAddDefaultLabel(const String& name, const String& text);
    
    static float normalizeSlider(const Slider* slider);
    
    static void updateSliderFromNormalized(Slider* slider, float normalized);
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (JLickshotComponent)
};

inline Slider* JLickshotComponent::createAndAddDefaultslider(const String &name)
{
    Slider* s = new Slider(name);
    addAndMakeVisible(s);
    s->setRange (0, 1.0, 0.01);
    s->setSliderStyle (Slider::Rotary);
    s->setTextBoxStyle (Slider::TextBoxBelow, false, 50, 20);
    s->addListener (this);
    return s;
}

inline Label* JLickshotComponent::createAndAddDefaultLabel(const String &name, const String &text)
{
    Label* l = new Label(name, text);
    addAndMakeVisible(l);
    l->setFont (Font (15.00f, Font::plain));
    l->setJustificationType (Justification::centred);
    l->setEditable (false, false, false);
    l->setColour (TextEditor::textColourId, Colours::black);
    l->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    return l;
}

inline float JLickshotComponent::normalizeSlider(const Slider *slider)
{
    return NORMALIZE(slider->getMinimum(),
                     slider->getMaximum(),
                     slider->getValue());
}

inline void JLickshotComponent::updateSliderFromNormalized(juce::Slider *slider, float normalized)
{
    slider->setValue(DENORMALIZE(slider->getMinimum(),
                                 slider->getMaximum(),
                                 normalized), dontSendNotification);
}

#endif  // JLICKSHOTCOMPONENT_H_INCLUDED

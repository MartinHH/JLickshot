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

#ifndef JLICKSHOTCOMPONENT_H_INCLUDED
#define JLICKSHOTCOMPONENT_H_INCLUDED

#include "JuceHeader.h"
#include "Defines.h"

#ifndef ROW1_LABEL_Y
#define ROW1_LABEL_Y 20
#endif 

#ifndef ROW1_SLIDER_Y
#define ROW1_SLIDER_Y (ROW1_LABEL_Y + 20)
#endif

#ifndef ROW_DISTANCE
#define ROW_DISTANCE 86
#endif

#define ROW2_LABEL_Y (ROW1_LABEL1 + ROW_DISTANCE)
#define ROW2_SLIDER_Y (ROW2_LABEL_Y + 20)

#ifndef COLUMN1_LABEL_X
#define COLUM1_LABEL_X 17
#endif

#ifndef COLUMN1_LABEL_X
#define COLUM1_SLIDER_X 23
#endif

#ifndef COLUMN_DISTANCE
#define COLUMN_DISTANCE 80
#endif

/**
 *  Base class for components containing parameter sliders. It
 *  offers methods to create sliders and labels with standard settings
 *  and to convert slider values to [0.0 , 1.0]-normalized values.
 */
class JLickshotComponent    :   public Component,
                                public Slider::Listener
{
public:
    JLickshotComponent(const String& name = String()):
        Component(name)
    {}
    
    virtual ~JLickshotComponent(){}

protected:

    std::unique_ptr<Slider> createAndAddDefaultslider(const String& name, double min = 0.0,
                                                      double max = 100.0, double interval = 0.1);
    
    std::unique_ptr<Label> createAndAddDefaultLabel(const String& name, const String& text);
    
    void setDefaultSliderBounds(Slider* slider, int row, int coloumn);
    
    void setDefaultLabelBounds(Label* label, int row, int coloumn);
    
    static float normalizeSlider(const Slider* slider);
    
    static void updateSliderFromNormalized(Slider* slider, float normalized);
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (JLickshotComponent)
};

inline std::unique_ptr<Slider> JLickshotComponent::createAndAddDefaultslider(const String &name, double min,
                                                                             double max, double interval)
{
    std::unique_ptr<Slider> s = std::make_unique<Slider>(name);
    addAndMakeVisible(s.get());
    s->setRange (min, max, interval);
    s->setSliderStyle (Slider::Rotary);
    s->setTextBoxStyle (Slider::TextBoxBelow, false, 50, 20);
    s->addListener (this);
    return s;
}

inline std::unique_ptr<Label> JLickshotComponent::createAndAddDefaultLabel(const String &name, const String &text)
{
    std::unique_ptr<Label> l = std::make_unique<Label>(name, text);
    addAndMakeVisible(l.get());
    l->setFont (Font (15.00f, Font::plain));
    l->setJustificationType (Justification::centred);
    l->setEditable (false, false, false);
    l->setColour (TextEditor::textColourId, Colours::black);
    l->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    return l;
}

inline void JLickshotComponent::setDefaultSliderBounds(Slider* slider, int row, int column)
{
    slider->setBounds(COLUM1_SLIDER_X + column * COLUMN_DISTANCE,
                      ROW1_SLIDER_Y + row * ROW_DISTANCE,
                      64, 56);
}

inline void JLickshotComponent::setDefaultLabelBounds(Label* label, int row, int column)
{
    label->setBounds(COLUM1_LABEL_X + column * COLUMN_DISTANCE,
                     ROW1_LABEL_Y + row * ROW_DISTANCE,
                     76, 16);
}

inline float JLickshotComponent::normalizeSlider(const Slider *slider)
{
    return (float) (NORMALIZE(slider->getMinimum(),
                              slider->getMaximum(),
                              slider->getValue()));
}

inline void JLickshotComponent::updateSliderFromNormalized(juce::Slider *slider, float normalized)
{
    slider->setValue(DENORMALIZE(slider->getMinimum(),
                                 slider->getMaximum(),
                                 normalized), dontSendNotification);
}

#endif  // JLICKSHOTCOMPONENT_H_INCLUDED

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

#include "DelayComponent.h"

DelayComponent::DelayComponent ()
{
    addAndMakeVisible(groupComponent_ = new GroupComponent ("parameters group",
                                                            TRANS("Delay")));
    
    addAndMakeVisible (activateButton_ = new ToggleButton ("activate button"));
    activateButton_->setButtonText (TRANS("activate"));
    activateButton_->addListener (this);
    
    addAndMakeVisible (delaySlider_ = new Slider ("new slider"));
    delaySlider_->setRange (0, 1.0, 0.001);
    delaySlider_->setValue(0.5);
    delaySlider_->setSliderStyle (Slider::Rotary);
    delaySlider_->setTextBoxStyle (Slider::TextBoxLeft, false, 50, 20);
    delaySlider_->addListener (this);
    
    addAndMakeVisible (delayLabel = new Label ("delay label",
                                               TRANS("Delay:")));
    delayLabel->setFont (Font (15.00f, Font::plain));
    delayLabel->setJustificationType (Justification::centredLeft);
    delayLabel->setEditable (false, false, false);
    delayLabel->setColour (TextEditor::textColourId, Colours::black);
    delayLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    
    addAndMakeVisible (feedbackSlider_ = new Slider ("feedback slider"));
    feedbackSlider_->setRange (0, 100, 0.1);
    feedbackSlider_->setValue(0.0);
    feedbackSlider_->setSliderStyle (Slider::Rotary);
    feedbackSlider_->setTextBoxStyle (Slider::TextBoxLeft, false, 50, 20);
    feedbackSlider_->addListener (this);
    
    addAndMakeVisible (feedbackLabel_ = new Label ("feedback label",
                                                   TRANS("Feedback:")));
    feedbackLabel_->setFont (Font (15.00f, Font::plain));
    feedbackLabel_->setJustificationType (Justification::centredLeft);
    feedbackLabel_->setEditable (false, false, false);
    feedbackLabel_->setColour (TextEditor::textColourId, Colours::black);
    feedbackLabel_->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    
    setSize (510, 96);
    
}

DelayComponent::~DelayComponent()
{
    activateButton_ = nullptr;
    delaySlider_ = nullptr;
    delayLabel = nullptr;
    feedbackSlider_ = nullptr;
    feedbackLabel_ = nullptr;
}

void DelayComponent::addListener(DelayComponent::Listener *listener)
{
    listeners_.add(listener);
}

void DelayComponent::removeListener(DelayComponent::Listener *listener)
{
    listeners_.remove(listener);
}

void DelayComponent::paint (Graphics& g)
{
    g.fillAll (Colours::lightgrey);
}

void DelayComponent::resized()
{
    groupComponent_->setBounds(0, 0, getWidth(), getHeight()-12);
    activateButton_->setBounds (24, 32, 71, 32);
    delaySlider_->setBounds (168, 24, 112, 48);
    delayLabel->setBounds (112, 32, 56, 32);
    feedbackSlider_->setBounds (376, 24, 112, 48);
    feedbackLabel_->setBounds (296, 32, 72, 32);
}

void DelayComponent::buttonClicked (Button* buttonThatWasClicked)
{
    if (buttonThatWasClicked == activateButton_){
        Component::BailOutChecker checker(this);
        listeners_.callChecked(checker,
                               &DelayComponent::Listener::activationStatusChanged,
                               activateButton_->getToggleState());
    }
}

void DelayComponent::sliderValueChanged (Slider* sliderThatWasMoved)
{
    if (sliderThatWasMoved == delaySlider_){
        Component::BailOutChecker checker(this);
        listeners_.callChecked(checker,
                               &DelayComponent::Listener::delayTimeChanged,
                               normalizeSlider(delaySlider_));
    } else if (sliderThatWasMoved == feedbackSlider_) {
        Component::BailOutChecker checker(this);
        listeners_.callChecked(checker,
                               &DelayComponent::Listener::feedbackChanged,
                               normalizeSlider(feedbackSlider_));
    }
}

float DelayComponent::normalizeSlider(Slider *slider)
{
    return NORMALIZE(slider->getMinimum(),
                     slider->getMaximum(),
                     slider->getValue());
}

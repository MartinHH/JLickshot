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
    
    addAndMakeVisible (delaySlider_ = new Slider ("delay slider"));
    delaySlider_->setRange (0, 1.0, 0.001);
    delaySlider_->setValue(0.5);
    delaySlider_->setSliderStyle (Slider::Rotary);
    delaySlider_->setTextBoxStyle (Slider::TextBoxBelow, false, 50, 20);
    delaySlider_->addListener (this);
    
    addAndMakeVisible (delayLabel = new Label ("delay label",
                                               TRANS("Delay:")));
    delayLabel->setFont (Font (15.00f, Font::plain));
    delayLabel->setJustificationType (Justification::centred);
    delayLabel->setEditable (false, false, false);
    delayLabel->setColour (TextEditor::textColourId, Colours::black);
    delayLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    
    addAndMakeVisible (feedbackSlider_ = new Slider ("feedback slider"));
    feedbackSlider_->setRange (0, 100, 0.1);
    feedbackSlider_->setValue(0.0);
    feedbackSlider_->setSliderStyle (Slider::Rotary);
    feedbackSlider_->setTextBoxStyle (Slider::TextBoxBelow, false, 50, 20);
    feedbackSlider_->addListener (this);
    
    addAndMakeVisible (feedbackLabel_ = new Label ("feedback label",
                                                   TRANS("Feedback:")));
    feedbackLabel_->setFont (Font (15.00f, Font::plain));
    feedbackLabel_->setJustificationType (Justification::centred);
    feedbackLabel_->setEditable (false, false, false);
    feedbackLabel_->setColour (TextEditor::textColourId, Colours::black);
    feedbackLabel_->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    
    addAndMakeVisible (lowpassButton_ = new ToggleButton ("lowpass button"));
    lowpassButton_->setButtonText (TRANS("lowpass"));
    lowpassButton_->addListener (this);
    
    addAndMakeVisible (frequencySlider_ = new Slider ("frequency slider"));
    frequencySlider_->setRange (50, 20000, 0.1);
    frequencySlider_->setValue(5000);
    frequencySlider_->setSkewFactor(0.5);
    frequencySlider_->setSliderStyle (Slider::Rotary);
    frequencySlider_->setTextBoxStyle (Slider::TextBoxBelow, false, 50, 20);
    frequencySlider_->addListener (this);
    
    addAndMakeVisible (frequencyLabel_ = new Label ("frequency label",
                                                    TRANS("Frequency:")));
    frequencyLabel_->setFont (Font (15.00f, Font::plain));
    frequencyLabel_->setJustificationType (Justification::centred);
    frequencyLabel_->setEditable (false, false, false);
    frequencyLabel_->setColour (TextEditor::textColourId, Colours::black);
    frequencyLabel_->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    
    setSize (430, 124);
    
}

DelayComponent::~DelayComponent()
{
    activateButton_ = nullptr;
    delaySlider_ = nullptr;
    delayLabel = nullptr;
    feedbackSlider_ = nullptr;
    feedbackLabel_ = nullptr;
    lowpassButton_ = nullptr;
    frequencySlider_ = nullptr;
    frequencyLabel_ = nullptr;
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
    activateButton_->setBounds (16, 32, 88, 24);
    lowpassButton_->setBounds (16, 64, 88, 24);
    
    setDefaultLabelBounds(delayLabel, 0, 1);
    setDefaultSliderBounds(delaySlider_, 0, 1);
    setDefaultLabelBounds(feedbackLabel_, 0, 2);
    setDefaultSliderBounds(feedbackSlider_, 0, 2);
    setDefaultLabelBounds(frequencyLabel_, 0, 3);
    setDefaultSliderBounds(frequencySlider_, 0, 3);
}

void DelayComponent::buttonClicked (Button* buttonThatWasClicked)
{
    if (buttonThatWasClicked == activateButton_){
        Component::BailOutChecker checker(this);
        listeners_.callChecked(checker,
                               &DelayComponent::Listener::delayActivationChanged,
                               activateButton_->getToggleState());
    } else if (buttonThatWasClicked == lowpassButton_){
        Component::BailOutChecker checker(this);
        listeners_.callChecked(checker,
                               &DelayComponent::Listener::lowpassActivatedChanged,
                               lowpassButton_->getToggleState());
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
    } else if (sliderThatWasMoved == frequencySlider_){
        Component::BailOutChecker checker(this);
        listeners_.callChecked(checker,
                               &DelayComponent::Listener::lowpassFrequencyChanged,
                               frequencySlider_->getValue());
    }
}

void DelayComponent::updateFromAudioSource(SampleSynthAudioSource& aSource)
{
    activateButton_->setToggleState(aSource.getDelayIsActive(), dontSendNotification);
    
    SimpleDelay& d = aSource.getDelayUnit();
    updateSliderFromNormalized(delaySlider_, d.getRelativeTime());
    updateSliderFromNormalized(feedbackSlider_, d.getFeedback());
    
    lowpassButton_->setToggleState(d.getLowpassIsActive(),
                                   dontSendNotification);
    frequencySlider_->setValue(d.getLowpassFrequency(),
                               dontSendNotification);
}

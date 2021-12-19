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

#include "DelayComponent.h"

DelayComponent::DelayComponent ()
{
    addAndMakeVisible(groupComponent_ = new GroupComponent ("parameters group",
                                                            TRANS("Delay")));
    
    addAndMakeVisible (activateButton_ = new ToggleButton ("activate button"));
    activateButton_->setButtonText (translate("activate"));
    activateButton_->addListener (this);
    
    addAndMakeVisible (lowpassButton_ = new ToggleButton ("lowpass button"));
    lowpassButton_->setButtonText (translate("lowpass"));
    lowpassButton_->addListener (this);
    
    delayLabel = createAndAddDefaultLabel(translate("delay label"), translate("Delay:"));
    delaySlider_ = createAndAddDefaultslider(translate("delay slider"), 0, 1.0, 0.001);
    delaySlider_->setValue(0.5);

    feedbackLabel_ = createAndAddDefaultLabel(translate("feedback label"), translate("Feedback:"));
    feedbackSlider_ = createAndAddDefaultslider(translate("delay slider"), 0, 100, 0.1);
    feedbackSlider_->setValue(0.0);
    
    frequencyLabel_ = createAndAddDefaultLabel(translate("frequency label"), translate("Frequency:"));
    frequencySlider_ = createAndAddDefaultslider(translate("frequency slider"), 50, 20000, 0.1);
    frequencySlider_->setValue(5000);
    frequencySlider_->setSkewFactor(0.5);
    
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

void DelayComponent::updateFromProcessor(JLickshotProcessorBase& processor)
{
    activateButton_->setToggleState(processor.getDelayIsActive(), dontSendNotification);
    
    SimpleDelay& d = processor.getDelayUnit();
    updateSliderFromNormalized(delaySlider_, d.getRelativeTime());
    updateSliderFromNormalized(feedbackSlider_, d.getFeedback());
    
    lowpassButton_->setToggleState(d.getLowpassIsActive(),
                                   dontSendNotification);
    frequencySlider_->setValue(d.getLowpassFrequency(),
                               dontSendNotification);
}

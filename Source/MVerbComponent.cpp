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

#include "MVerbComponent.h"

MVerbComponent::MVerbComponent():
    JLickshotComponent(translate("jlickshot component"))
{
    addAndMakeVisible(groupComponent_ = new GroupComponent ("parameters group",
                                                            TRANS("MVerb")));
    
    addAndMakeVisible (activateButton_ = new ToggleButton ("activate button"));
    activateButton_->setButtonText (TRANS("activate"));
    activateButton_->addListener (this);
    
    gainLabel_ = createAndAddDefaultLabel(translate("gain Label"), translate("Gain:"));
    gainSlider_= createAndAddDefaultslider(translate("gain slider"));
    gainSlider_->setValue(1.0);

    mixLabel_ = createAndAddDefaultLabel(translate("mix Label"), translate("Mix:"));
    mixSlider_= createAndAddDefaultslider(translate("mix slider"));
    mixSlider_->setValue(0.15);

    earlyLateMixLabel_ = createAndAddDefaultLabel(translate("early late Label"), translate("Early/Late:"));
    earlyLateMixSlider_= createAndAddDefaultslider(translate("early late slider"));
    earlyLateMixSlider_->setValue(0.75);
    
    dampingLabel_ = createAndAddDefaultLabel(translate("damping Label"), translate("Damping:"));
    dampingSlider_= createAndAddDefaultslider(translate("damping slider"));
    dampingSlider_->setValue(0.0);

    densityLabel_ = createAndAddDefaultLabel(translate("density Label"), translate("Density:"));
    densitySlider_= createAndAddDefaultslider(translate("density slider"));
    densitySlider_->setValue(0.5);
    
    bandwidthLabel_ = createAndAddDefaultLabel(translate("bandwidth Label"), translate("Bandwidth:"));
    bandwidthSlider_= createAndAddDefaultslider(translate("bandwidth slider"));
    bandwidthSlider_->setValue(1.0);
    
    predelayLabel_ = createAndAddDefaultLabel(translate("predelay Label"), translate("Predelay:"));
    predelaySlider_= createAndAddDefaultslider(translate("predelay slider"));
    predelaySlider_->setValue(0.0);
    
    sizeLabel_ = createAndAddDefaultLabel(translate("size Label"), translate("Size:"));
    sizeSlider_= createAndAddDefaultslider(translate("size slider"));
    sizeSlider_->setValue(0.5);
    
    decayLabel_ = createAndAddDefaultLabel(translate("decay Label"), translate("Decay:"));
    decaySlider_= createAndAddDefaultslider(translate("decay slider"));
    decaySlider_->setValue(0.5);
    
    setSize (430, 214);
}

MVerbComponent::~MVerbComponent()
{
    groupComponent_ = nullptr;
    activateButton_ = nullptr;
    gainLabel_ = nullptr;
    gainSlider_ = nullptr;
    mixLabel_ = nullptr;
    mixSlider_ = nullptr;
    earlyLateMixLabel_ = nullptr;
    earlyLateMixSlider_ = nullptr;
    dampingLabel_ = nullptr;
    dampingSlider_ = nullptr;
    densityLabel_ = nullptr;
    densitySlider_ = nullptr;
    bandwidthLabel_ = nullptr;
    bandwidthSlider_ = nullptr;
    predelayLabel_ = nullptr;
    predelaySlider_ = nullptr;
    sizeLabel_ = nullptr;
    sizeSlider_ = nullptr;
    decayLabel_ = nullptr;
    decaySlider_ = nullptr;

}

void MVerbComponent::addListener(MVerbComponent::Listener *listener)
{
    listeners_.add(listener);
}

void MVerbComponent::removeListener(MVerbComponent::Listener *listener)
{
    listeners_.remove(listener);
}

void MVerbComponent::paint (Graphics& g)
{
    g.fillAll (Colours::lightgrey);
}

void MVerbComponent::resized()
{
    groupComponent_->setBounds(0, 0, getWidth(), getHeight()-12);
    activateButton_->setBounds (16, 32, 88, 24);
    setDefaultLabelBounds(gainLabel_, 0, 1);
    setDefaultSliderBounds(gainSlider_, 0, 1);
    setDefaultLabelBounds(mixLabel_, 0, 2);
    setDefaultSliderBounds(mixSlider_, 0, 2);
    setDefaultLabelBounds(earlyLateMixLabel_, 0, 3);
    setDefaultSliderBounds(earlyLateMixSlider_, 0, 3);
    setDefaultLabelBounds(dampingLabel_, 0, 4);
    setDefaultSliderBounds(dampingSlider_, 0, 4);
    setDefaultLabelBounds(densityLabel_, 1, 0);
    setDefaultSliderBounds(densitySlider_, 1, 0);
    setDefaultLabelBounds(bandwidthLabel_, 1, 1);
    setDefaultSliderBounds(bandwidthSlider_, 1, 1);
    setDefaultLabelBounds(predelayLabel_, 1, 2);
    setDefaultSliderBounds(predelaySlider_, 1, 2);
    setDefaultLabelBounds(sizeLabel_, 1, 3);
    setDefaultSliderBounds(sizeSlider_, 1, 3);
    setDefaultLabelBounds(decayLabel_, 1, 4);
    setDefaultSliderBounds(decaySlider_, 1, 4);
}

void MVerbComponent::buttonClicked(Button *buttonThatWasClicked)
{
    if (buttonThatWasClicked == activateButton_){
        Component::BailOutChecker checker(this);
        listeners_.callChecked(checker,
                               &MVerbComponent::Listener::reverbActivationChanged,
                               activateButton_->getToggleState());
    }
}

void MVerbComponent::sliderValueChanged(Slider *sliderThatWasMoved)
{
    if (sliderThatWasMoved == gainSlider_){
        Component::BailOutChecker checker(this);
        listeners_.callChecked(checker,
                               &MVerbComponent::Listener::gainChanged,
                               normalizeSlider(gainSlider_));
    } else if (sliderThatWasMoved == mixSlider_){
        Component::BailOutChecker checker(this);
        listeners_.callChecked(checker,
                               &MVerbComponent::Listener::mixChanged,
                               normalizeSlider(mixSlider_));
    } else if (sliderThatWasMoved == earlyLateMixSlider_){
        Component::BailOutChecker checker(this);
        listeners_.callChecked(checker,
                               &MVerbComponent::Listener::earlyLateMixChanged,
                               normalizeSlider(earlyLateMixSlider_));
    } else if (sliderThatWasMoved == dampingSlider_){
        Component::BailOutChecker checker(this);
        listeners_.callChecked(checker,
                               &MVerbComponent::Listener::dampingChanged,
                               normalizeSlider(dampingSlider_));
    } else if (sliderThatWasMoved == densitySlider_){
        Component::BailOutChecker checker(this);
        listeners_.callChecked(checker,
                               &MVerbComponent::Listener::densityChanged,
                               normalizeSlider(densitySlider_));
    } else if (sliderThatWasMoved == bandwidthSlider_){
        Component::BailOutChecker checker(this);
        listeners_.callChecked(checker,
                               &MVerbComponent::Listener::bandwidthChanged,
                               normalizeSlider(bandwidthSlider_));
    } else if (sliderThatWasMoved == predelaySlider_){
        Component::BailOutChecker checker(this);
        listeners_.callChecked(checker,
                               &MVerbComponent::Listener::predelayChanged,
                               normalizeSlider(predelaySlider_));
    } else if (sliderThatWasMoved == sizeSlider_){
        Component::BailOutChecker checker(this);
        listeners_.callChecked(checker,
                               &MVerbComponent::Listener::sizeChanged,
                               normalizeSlider(sizeSlider_));
    } else if (sliderThatWasMoved == decaySlider_){
        Component::BailOutChecker checker(this);
        listeners_.callChecked(checker,
                               &MVerbComponent::Listener::decayChanged,
                               normalizeSlider(decaySlider_));
    }

}

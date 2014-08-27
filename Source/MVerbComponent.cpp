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

#define ROW1_LABEL_Y 20
#define ROW1_SLIDER_Y (ROW1_LABEL_Y + 20)
#define ROW2_LABEL_Y 106
#define ROW2_SLIDER_Y (ROW2_LABEL_Y + 20)


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
    
    setSize (510, 214);
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
    gainLabel_->setBounds (117, ROW1_LABEL_Y, 76, 16);
    gainSlider_->setBounds (123, ROW1_SLIDER_Y, 64, 56);
    mixLabel_->setBounds (217, ROW1_LABEL_Y, 76, 16);
    mixSlider_->setBounds (223, ROW1_SLIDER_Y, 64, 56);
    earlyLateMixLabel_->setBounds (317, ROW1_LABEL_Y, 76, 16);
    earlyLateMixSlider_->setBounds (323, ROW1_SLIDER_Y, 64, 56);
    dampingLabel_->setBounds (417, ROW1_LABEL_Y, 76, 16);
    dampingSlider_->setBounds (423, ROW1_SLIDER_Y, 64, 56);
    densityLabel_->setBounds ( 17, ROW2_LABEL_Y, 76, 16);
    densitySlider_->setBounds ( 23, ROW2_SLIDER_Y, 64, 56);
    bandwidthLabel_->setBounds (117, ROW2_LABEL_Y, 76, 16);
    bandwidthSlider_->setBounds (123, ROW2_SLIDER_Y, 64, 56);
    predelayLabel_->setBounds (217, ROW2_LABEL_Y, 76, 16);
    predelaySlider_->setBounds (223, ROW2_SLIDER_Y, 64, 56);
    sizeLabel_->setBounds (317, ROW2_LABEL_Y, 76, 16);
    sizeSlider_->setBounds (323, ROW2_SLIDER_Y, 64, 56);
    decayLabel_->setBounds (417, ROW2_LABEL_Y, 76, 16);
    decaySlider_->setBounds (423, ROW2_SLIDER_Y, 64, 56);

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

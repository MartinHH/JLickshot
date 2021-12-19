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

#include "MVerbComponent.h"

MVerbComponent::MVerbComponent():
    JLickshotComponent(translate("jlickshot component"))
{
    groupComponent_ = std::make_unique<GroupComponent>("parameters group",
                                                       TRANS("MVerb"));
    addAndMakeVisible(groupComponent_.get());
    
    activateButton_ = std::make_unique<ToggleButton> ("activate button");
    addAndMakeVisible (activateButton_.get());
    activateButton_->setButtonText (TRANS("activate"));
    activateButton_->addListener (this);
    
    gainLabel_ = createAndAddDefaultLabel(translate("gain Label"), translate("Gain:"));
    gainSlider_= createAndAddDefaultslider(translate("gain slider"));

    mixLabel_ = createAndAddDefaultLabel(translate("mix Label"), translate("Mix:"));
    mixSlider_= createAndAddDefaultslider(translate("mix slider"));

    earlyLateMixLabel_ = createAndAddDefaultLabel(translate("early late Label"), translate("Early/Late:"));
    earlyLateMixSlider_= createAndAddDefaultslider(translate("early late slider"));
    
    dampingLabel_ = createAndAddDefaultLabel(translate("damping Label"), translate("Damping:"));
    dampingSlider_= createAndAddDefaultslider(translate("damping slider"));

    densityLabel_ = createAndAddDefaultLabel(translate("density Label"), translate("Density:"));
    densitySlider_= createAndAddDefaultslider(translate("density slider"));
    
    bandwidthLabel_ = createAndAddDefaultLabel(translate("bandwidth Label"), translate("Bandwidth:"));
    bandwidthSlider_= createAndAddDefaultslider(translate("bandwidth slider"));
    
    predelayLabel_ = createAndAddDefaultLabel(translate("predelay Label"), translate("Predelay:"));
    predelaySlider_= createAndAddDefaultslider(translate("predelay slider"));
    
    sizeLabel_ = createAndAddDefaultLabel(translate("size Label"), translate("Size:"));
    sizeSlider_= createAndAddDefaultslider(translate("size slider"));
    
    decayLabel_ = createAndAddDefaultLabel(translate("decay Label"), translate("Decay:"));
    decaySlider_= createAndAddDefaultslider(translate("decay slider"));
    
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
    setDefaultLabelBounds(gainLabel_.get(), 0, 1);
    setDefaultSliderBounds(gainSlider_.get(), 0, 1);
    setDefaultLabelBounds(mixLabel_.get(), 0, 2);
    setDefaultSliderBounds(mixSlider_.get(), 0, 2);
    setDefaultLabelBounds(earlyLateMixLabel_.get(), 0, 3);
    setDefaultSliderBounds(earlyLateMixSlider_.get(), 0, 3);
    setDefaultLabelBounds(dampingLabel_.get(), 0, 4);
    setDefaultSliderBounds(dampingSlider_.get(), 0, 4);
    setDefaultLabelBounds(densityLabel_.get(), 1, 0);
    setDefaultSliderBounds(densitySlider_.get(), 1, 0);
    setDefaultLabelBounds(bandwidthLabel_.get(), 1, 1);
    setDefaultSliderBounds(bandwidthSlider_.get(), 1, 1);
    setDefaultLabelBounds(predelayLabel_.get(), 1, 2);
    setDefaultSliderBounds(predelaySlider_.get(), 1, 2);
    setDefaultLabelBounds(sizeLabel_.get(), 1, 3);
    setDefaultSliderBounds(sizeSlider_.get(), 1, 3);
    setDefaultLabelBounds(decayLabel_.get(), 1, 4);
    setDefaultSliderBounds(decaySlider_.get(), 1, 4);
}

void MVerbComponent::buttonClicked(Button *buttonThatWasClicked)
{
    if (buttonThatWasClicked == activateButton_.get()){
        Component::BailOutChecker checker(this);
        listeners_.callChecked(checker,
                               &MVerbComponent::Listener::reverbActivationChanged,
                               activateButton_->getToggleState());
    }
}

void MVerbComponent::sliderValueChanged(Slider *sliderThatWasMoved)
{
    if (sliderThatWasMoved == gainSlider_.get()){
        Component::BailOutChecker checker(this);
        listeners_.callChecked(checker,
                               &MVerbComponent::Listener::gainChanged,
                               normalizeSlider(gainSlider_.get()));
    } else if (sliderThatWasMoved == mixSlider_.get()){
        Component::BailOutChecker checker(this);
        listeners_.callChecked(checker,
                               &MVerbComponent::Listener::mixChanged,
                               normalizeSlider(mixSlider_.get()));
    } else if (sliderThatWasMoved == earlyLateMixSlider_.get()){
        Component::BailOutChecker checker(this);
        listeners_.callChecked(checker,
                               &MVerbComponent::Listener::earlyLateMixChanged,
                               normalizeSlider(earlyLateMixSlider_.get()));
    } else if (sliderThatWasMoved == dampingSlider_.get()){
        Component::BailOutChecker checker(this);
        listeners_.callChecked(checker,
                               &MVerbComponent::Listener::dampingChanged,
                               normalizeSlider(dampingSlider_.get()));
    } else if (sliderThatWasMoved == densitySlider_.get()){
        Component::BailOutChecker checker(this);
        listeners_.callChecked(checker,
                               &MVerbComponent::Listener::densityChanged,
                               normalizeSlider(densitySlider_.get()));
    } else if (sliderThatWasMoved == bandwidthSlider_.get()){
        Component::BailOutChecker checker(this);
        listeners_.callChecked(checker,
                               &MVerbComponent::Listener::bandwidthChanged,
                               normalizeSlider(bandwidthSlider_.get()));
    } else if (sliderThatWasMoved == predelaySlider_.get()){
        Component::BailOutChecker checker(this);
        listeners_.callChecked(checker,
                               &MVerbComponent::Listener::predelayChanged,
                               normalizeSlider(predelaySlider_.get()));
    } else if (sliderThatWasMoved == sizeSlider_.get()){
        Component::BailOutChecker checker(this);
        listeners_.callChecked(checker,
                               &MVerbComponent::Listener::sizeChanged,
                               normalizeSlider(sizeSlider_.get()));
    } else if (sliderThatWasMoved == decaySlider_.get()){
        Component::BailOutChecker checker(this);
        listeners_.callChecked(checker,
                               &MVerbComponent::Listener::decayChanged,
                               normalizeSlider(decaySlider_.get()));
    }
}

void MVerbComponent::updateFromProcessor(JLickshotProcessorBase& processor)
{
    activateButton_->setToggleState(processor.getReverbIsActive(), dontSendNotification);
    
    MVerb<float>& mv = processor.getMVerb();
    updateSliderFromNormalized(gainSlider_.get(), mv.getParameter(MVerb<float>::GAIN));
    updateSliderFromNormalized(mixSlider_.get(), mv.getParameter(MVerb<float>::MIX));
    updateSliderFromNormalized(earlyLateMixSlider_.get(), mv.getParameter(MVerb<float>::EARLYMIX));
    updateSliderFromNormalized(dampingSlider_.get(), mv.getParameter(MVerb<float>::DAMPINGFREQ));
    updateSliderFromNormalized(densitySlider_.get(), mv.getParameter(MVerb<float>::DENSITY));
    updateSliderFromNormalized(bandwidthSlider_.get(), mv.getParameter(MVerb<float>::BANDWIDTHFREQ));
    updateSliderFromNormalized(predelaySlider_.get(), mv.getParameter(MVerb<float>::PREDELAY));
    updateSliderFromNormalized(sizeSlider_.get(), mv.getParameter(MVerb<float>::SIZE));
    updateSliderFromNormalized(decaySlider_.get(), mv.getParameter(MVerb<float>::DECAY));
}

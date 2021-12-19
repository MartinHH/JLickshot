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


#include "SampleCollectionComponent.h"

SampleCollectionComponent::SampleCollectionComponent()
{
    for(int i=0; i<NUMBER_OF_NOTES; i++){
        choosers_[i] = new SampleChoiceComponent(i);
        addAndMakeVisible(choosers_[i]);
    }

    setSize(500, 26 * NUMBER_OF_NOTES);
}

SampleCollectionComponent::~SampleCollectionComponent()
{
    for(int i=0; i<NUMBER_OF_NOTES; i++){
        choosers_[i] = nullptr;
    }
}

void SampleCollectionComponent::addListener(SampleChoiceComponent::Listener *listener)
{
    for(int i=0; i<NUMBER_OF_NOTES; i++){
        choosers_[i]->addListener(listener);
    }
}

void SampleCollectionComponent::removeListener(SampleChoiceComponent::Listener *listener)
{
    for(int i=0; i<NUMBER_OF_NOTES; i++){
        choosers_[i]->removeListener(listener);
    }
}

void SampleCollectionComponent::paint (Graphics& g)
{
    g.fillAll (Colours::grey);   // clear the background
}

void SampleCollectionComponent::resized()
{
    for(int i=0; i<NUMBER_OF_NOTES; i++){
        choosers_[i]->setBounds(0, i*26+1, getWidth(), 24);
    }
}

void SampleCollectionComponent::updateFromSynth(const SampleSynth& synth)
{
    for(int i=0; i<NUMBER_OF_NOTES; i++){
        if(synth.sampleIsLoaded(i)){
            choosers_[i]->setFileName(synth.getFileName(i));
            choosers_[i]->setVelocity(synth.getVelocity(i));
        } else {
            choosers_[i]->setFileName(translate("empty"));
            choosers_[i]->setVelocity(0.75);
        }
    }
}

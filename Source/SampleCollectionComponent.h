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

#ifndef SAMPLECOLLECTIONCOMPONENT_H_INCLUDED
#define SAMPLECOLLECTIONCOMPONENT_H_INCLUDED

#include "JuceHeader.h"
#include "SampleChoiceComponent.h"
#include "SampleSynth.h"
#include "Defines.h"

/**
 *  A GUI component holding 128 SampleChoiceComponents (1 for each note).
 */
class SampleCollectionComponent    : public Component
{
public:
    SampleCollectionComponent();
    ~SampleCollectionComponent();

    /** Adds a listener. */
    void addListener (SampleChoiceComponent::Listener* listener);
    
    /** Removes a previously-registered listener. */
    void removeListener (SampleChoiceComponent::Listener* listener);
    
    void paint (Graphics&);
    void resized();

    /** Updates all components to the state of a SampleSynth. */
    void updateFromSynth(const SampleSynth& synth);
    
private:
    std::unique_ptr<SampleChoiceComponent> choosers_[NUMBER_OF_NOTES];
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SampleCollectionComponent)
};


#endif  // SAMPLECOLLECTIONCOMPONENT_H_INCLUDED

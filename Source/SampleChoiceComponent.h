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

#ifndef SAMPLECHOICECOMPONENT_H_INCLUDED
#define SAMPLECHOICECOMPONENT_H_INCLUDED

#include "JuceHeader.h"

/**
 *  A GUI component to control a single sample slot (=note) of a
 *  SampleSynth.
 */
class SampleChoiceComponent    :    public Component,
                                    public SliderListener,
                                    public ButtonListener,
                                    public FileDragAndDropTarget
{
public:
    /** Constructor.
     *
     *  @param noteNumber The number of the note this SampleChoiceComponent
     *      is responsible for.
     */
    SampleChoiceComponent(int noteNumber);
    ~SampleChoiceComponent();
    
    /** A listener interface for callbacks by this component. */
    class Listener
    {
    public:
        virtual ~Listener(){}
        
        /** Called when the velocity slider was changed. */
        virtual void velocityChanged(SampleChoiceComponent* sampleChoiceComponent) = 0;
        
        /** Called when a file was chosen to be loaded. */
        virtual void fileChosen(SampleChoiceComponent* sampleChoiceComponent,
                                const File& audioFile) = 0;
    };
    
    /** Adds a listener. */
    void addListener (Listener* listener);
    
    /** Removes a previously-registered listener. */
    void removeListener (Listener* listener);
    
    void paint (Graphics&);
    void resized();
    
    void sliderValueChanged (Slider* sliderThatWasMoved);
    void buttonClicked (Button* buttonThatWasClicked);

    int getNoteNumber() const;

    void setFileName(const String& name);
    
    float getVelocity() const;
    void setVelocity(float velocity);
    
    bool isInterestedInFileDrag (const StringArray& files) override;
    void filesDropped (const StringArray& files, int x, int y) override;
    
private:
    int noteNo_;
    ScopedPointer<Label> noteLabel_;
    ScopedPointer<TextEditor> nameDisplay_;
    ScopedPointer<Slider> velocitySlider_;
    ScopedPointer<TextButton> fileButton_;
    ListenerList <Listener> listeners_;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SampleChoiceComponent)
};


#endif  // SAMPLECHOICECOMPONENT_H_INCLUDED

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

#ifndef SIMPLEDELAY_H_INCLUDED
#define SIMPLEDELAY_H_INCLUDED

#include "JuceHeader.h"
#include "Defines.h"

class SimpleDelay
{
public:
    SimpleDelay(int noChannels=2, int maxLength=1, int sampleRate=44100);
    virtual ~SimpleDelay();
    
    void setSampleRate(int sampleRate);
    
    void setLength(float length);
    
    float getLength() const;
    
    float getLengthInSeconds() const;
    
    void setFeedback(float feedback);
    
    float getFeedback() const;
    
    void processBlock (AudioSampleBuffer& buffer);
    
    XmlElement* getStateXml() const;
    
    void updateFromXml(XmlElement* stateXml);
    
protected:
    CriticalSection lock;
    
private:
    AudioSampleBuffer delayBuffer_;
    int maxLength_;     // in seconds
    float length_;      // relative length in samples
    float feedBack_;
    int sampleRate_;
    int delayBufferIdx_;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SimpleDelay)
};





#endif  // SIMPLEDELAY_H_INCLUDED

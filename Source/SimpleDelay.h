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
#include "OnePoleLowpass.h"

#ifndef LP_MIN_FREQ
#define LP_MIN_FREQ 0.0
#endif

#ifndef LP_MAX_FREQ
#define LP_MAX_FREQ 25000.0
#endif

class SimpleDelay
{
public:
    SimpleDelay(int noChannels=2, int maxTime=1, int sampleRate=44100);
    virtual ~SimpleDelay();
    
    /** Sets the sample rate.
     *
     *  @param sampleRate The current sample rate.
     */
    void setSampleRate(int sampleRate);
    
    /** Sets the delay time in relation to the maximum delay time.
     *
     *  @param time A value within [0.0 ; 1.0].
     */
    void setRelativeTime(float time);
    
    /** Returns the delay time in relation to the maximum delay time.
     *
     *  @return A value within [0.0 ; 1.0].
     */
    float getRelativeTime() const;
    
    /** Returns the (absolute) delay time in seconds.
     *
     *  @return The (absolute) delay time in seconds.
     */
    float getTimeInSeconds() const;
    
    /** Sets the delay feedback value.
     *
     *  @param feedback A value within [0.0 ; 1.0].
     */
    void setFeedback(float feedback);

    /** Returns the delay feedback value.
     *
     *  @return The delay feedback value (a value within [0.0 ; 1.0]).
     */
    float getFeedback() const;
    
    void setLowpassIsActive(bool lowPassActive);
    
    bool getLowpassIsActive() const;
    
    void setLowpassFrequency(double frequency);
    
    double getLowpassFrequency() const;
    
    double getLowpassFrequencyNormalized() const
        { return NORMALIZE(LP_MIN_FREQ, LP_MAX_FREQ, getLowpassFrequency()); }
    
    int getNumChannels() const;
    
    /** Processes a block auf audio data, adding the delay effect to it.
     *
     *  @param buffer A block of audio data that shall have this delay effect
     *      added.
     */
    void processBlock (AudioSampleBuffer& buffer);
    
    XmlElement* getStateXml() const;
    
    void updateFromXml(XmlElement* stateXml);
    
protected:
    CriticalSection lock;
    
private:
    AudioSampleBuffer delayBuffer_;
    int maxTime_;     // in seconds
    float time_;      // time setting in relation to maxTime_
    float feedBack_;
    int sampleRate_;
    int delayBufferIdx_;
    std::vector<OnePoleLowpass> lp_;
    bool lowPass_;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SimpleDelay)
};





#endif  // SIMPLEDELAY_H_INCLUDED

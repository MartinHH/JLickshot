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

#ifndef JLICKSHOTPROCESSORBASE_H_INCLUDED
#define JLICKSHOTPROCESSORBASE_H_INCLUDED

#include "JuceHeader.h"
#include "SampleSynth.h"
#include "SimpleDelay.h"
#include "MVerbPlus.h"

/**
 *  Base class for the audio-processing parts the stand-alone version
 *  and the plugin have in common. It also contains the corresponding
 *  xml-handling methods.
 */
class JLickshotProcessorBase
{
public:
    JLickshotProcessorBase(int noOfVoices=8);
    virtual ~JLickshotProcessorBase();
    
    MidiKeyboardState& getKeyState();
    
    SampleSynth& getSynth();
    
    SimpleDelay& getDelayUnit();
    
    MVerbPlus& getMVerb();
    
    void process(AudioSampleBuffer& buffer, MidiBuffer& midiData,
                 int numSamples);
    
    void setSampleRate(double sampleRate);
    
    void setDelayIsActive(bool delayIsActive);
    
    bool getDelayIsActive() const;
    
    void setReverbIsActive(bool reverbIsActive);
    
    bool getReverbIsActive() const;
    
    void setMasterGain(float gain);
    
    float getMasterGain() const;
    
    /** Calls the getStateXml methods of the synth and the effects
     *  and adds the returned XmlElements as childs to the XmlElement
     *  passed as parameter. Also adds the "GENERALSETTINGS" element.
     *
     *  @param An XmlElement that those XmlElements shall be added to.
     *
     *  @return A pointer to the same XmlElement that was passed as
     *      parameter.
     */
    XmlElement* addStateXmlElements(XmlElement* xml);
    
    SampleSynth::LoadResult updateFromXml(const XmlElement* stateXml);
    
    /** An atomic flag indicating that a preset has been loaded so the GUI
     *  must update the filenames of the samples. Once checked, the flag is
     *  set to false, so this must only be checked at one point and the 
     *  GUI then must be updated if true is returned.
     */
    bool samplesChanged(){ return samplesChanged_.compareAndSetBool(0, 1); }
    
protected:
    MidiKeyboardState keyState_;
    SampleSynth synth_;
    SimpleDelay delay_;
    MVerbPlus mVerb_;
    bool delayIsActive_;
    bool reverbIsActive_;
    float gain_;
    float lastGain_;
    Atomic<int> samplesChanged_;
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (JLickshotProcessorBase)
};


#endif  // JLICKSHOTPROCESSORBASE_H_INCLUDED

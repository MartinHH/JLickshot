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

#ifndef MAINCONTROLLER_H_INCLUDED
#define MAINCONTROLLER_H_INCLUDED

#include "JuceHeader.h"
#include "SampleSynthAudioSource.h"
#include "JLickshotControllerBase.h"

/**
 *  This class contains the main logic of the stand-alone version. It holds an
 *  AudioDeviceManager, a MidiKeyboardState, an AudioSourcePlayer and a
 *  SampleSynthAudioSource (which holds the SampleSynth) and connects them.
 *  Furthermore, it accepts callbacks from the GUI and offers any relevant
 *  accessors to the GUI. Also, it contains methods to store/restore its state
 *  to/from an XML file.
 */
class MainController    :   public JLickshotControllerBase
{
public:
    MainController();
    ~MainController();
    
    AudioDeviceManager& getAudioDeviceManager();
    
    JLickshotProcessorBase& getProcessor();
    
    const SampleSynth& getSynth();
    
    void velocityChanged(SampleChoiceComponent* sampleChoiceComponent);
    
    void fileChosen(SampleChoiceComponent* sampleChoiceComponent,
                    const File& audioFile);
    
    void delayActivationChanged(bool delaySetActive);
    
    void delayTimeChanged(float delay);
    
    void feedbackChanged(float feedback);
    
    void lowpassActivatedChanged(bool lpSetActive);
    
    void lowpassFrequencyChanged(float frequency);
    
    void reverbActivationChanged(bool delaySetActive);
    
    void dampingChanged(float newValue);
    
    void densityChanged(float newValue);
    
    void bandwidthChanged(float newValue);
    
    void predelayChanged(float newValue);
    
    void sizeChanged(float newValue);
    
    void decayChanged(float newValue);
    
    void gainChanged(float newValue);
    
    void mixChanged(float newValue);
    
    void earlyLateMixChanged(float newValue);
    
    /** Sets the main playback gain. Sets the gain setting for
     *  the AudioSourcePlayer that is used internally.
     *
     *  @param gain
     */
    void setMasterGain(float gain);
    
    /** Returns the main playback gain. Returns the gain setting of
     *  the AudioSourcePlayer that is used internally.
     *
     *  @return the gain setting of the AudioSourcePlayer that is used
     *      internally.
     */
    float getMasterGain() const;
    
    /** Saves the state of the controller in XML format to a specified
     *  file. Saves the filepaths and velocity settings of the loaded
     *  samples, the master gain and the settings of the AudioDeviceManager
     *  (if those differ from its default settings).
     *
     *  @return true on success, false on any error.
     *
     *  @see loadState
     */
    bool saveState(const File& xmlDest, bool toOneDir = false);
    
    /** Restores the state of the controller from an XML file generated
     *  using saveState.
     *
     *  @return a LoadResult struct containing details of the operation.
     *
     *  @see saveState
     */
    SampleSynth::LoadResult loadState(const File& xmlSource);
    
private:
    AudioDeviceManager adm_;
    SampleSynthAudioSource aSource_;
    AudioSourcePlayer aSourcePlayer_;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainController)
};


#endif  // MAINCONTROLLER_H_INCLUDED

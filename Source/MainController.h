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
#include "SampleChoiceComponent.h"

/**
 *  This class contains the main logic. It holds an AudioDeviceManager,
 *  a MidiKeyboardState, an AudioSourcePlayer and a SampleSynthAudioSource
 *  (which holds the SampleSynth) and connects them. Furthermore, it
 *  accepts callbacks from the GUI and offers any relevant accessors to the GUI.
 *  Also, it contains methods to store/restore its state to/from an XML file.
 */
class MainController    :   public SampleChoiceComponent::Listener
{
public:
    MainController();
    ~MainController();
    
    AudioDeviceManager& getAudioDeviceManager();
    
    MidiKeyboardState& getKeyState();
    
    const SampleSynth& getSynth();
    
    void velocityChanged(SampleChoiceComponent* sampleChoiceComponent);
    
    void fileChosen(SampleChoiceComponent* sampleChoiceComponent,
                    const File& audioFile);
    
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
    bool saveState(const File& xmlDest);
    
    /** A struct describing the results of a call to loadState. */
    struct LoadResult
    {
        bool success;   /**< Load operation was generally successfull. */
        int loaded;     /**< How many samples were loaded successfully. */
        int failed;     /**< How many samples could not be loaded (only
                             specified in case success is set to true). */
    };
    
    /** Restores the state of the controller from an XML file generated
     *  using saveState.
     *
     *  @return a LoadResult struct containing details of the operation.
     *
     *  @see saveState
     */
    LoadResult loadState(const File& xmlSource);
    
private:
    AudioDeviceManager adm_;
    MidiKeyboardState keyState_;
    SampleSynthAudioSource aSource_;
    AudioSourcePlayer aSourcePlayer_;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainController)
};


#endif  // MAINCONTROLLER_H_INCLUDED

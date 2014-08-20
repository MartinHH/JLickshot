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

#ifndef SAMPLESYNTH_H_INCLUDED
#define SAMPLESYNTH_H_INCLUDED

#include "JuceHeader.h"
#include "Defines.h"

/**
 *  A SamplerSound class that contains additional information about
 *  the File and the velocity to be used for playback.
 */
class FixedVelocitySound    :   public SamplerSound
{
public:
    FixedVelocitySound(const File& audioFile,
                       AudioFormatReader& source,
                       const BigInteger& midiNotes,
                       int midiNoteForNormalPitch,
                       double attackTimeSecs,
                       double releaseTimeSecs,
                       double maxSampleLengthSeconds,
                       float velocity = 1.0);
    
    ~FixedVelocitySound();
    
    void setVelocity(float velocity);
    
    float getVelocity() const;
    
    const File& getAudioFile() const;
    
private:
    float velocity_;
    const File af_;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FixedVelocitySound)
};

//==============================================================================

/**
 *  A SamplerVoice class that plays sounds with a fixed velocity. The velocity
 *  used for playback is taken from the FixedVelocitySound objects that are to
 *  be used with this voice.
 */
class FixedVelocityVoice    :   public SamplerVoice
{
public:
    FixedVelocityVoice();
    ~FixedVelocityVoice();
    
    bool canPlaySound (SynthesiserSound* sound) override;
    
    void startNote (int midiNoteNumber, float velocity, SynthesiserSound* sound,
                    int pitchWheel) override;
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FixedVelocityVoice)
};

//==============================================================================

/**
 *  A Synthesiser playing back FixedVelocitySounds using FixedVelocityVoices.
 *  For every note, a different audiofile and veolcity setting can be assigned.
 *
 *  Playback is one-shot-style, meaning once a sample's playback is triggered,
 *  it cannot be stopped again.
 */
class SampleSynth   :   public Synthesiser
{
public:
    SampleSynth(int numberOfVoices = 4);
    ~SampleSynth();
    
    /** Sets the audiofile to be assigned to be played back for a specified note.
     *
     *  @param noteNo The MIDI note number the file shall be assigned to (0-127).
     *  @param audioFile The file to be assigned.
     *  @param velocity The velocity to be used for playback.
     *
     *  @return true on success, false on any error.
     */
    bool setSample(int noteNo, const File& audioFile, float velocity);
    
    /** Returns the path of the audio file assigned to a certain note.
     *
     *  @param noteNo The note number for which the file path shall be returned.
     *
     *  @return The absolute path of the file assigned to that note or an empty
     *      string if no file is assigned.
     */
    const String& getFilePath(int noteNo) const;
    
    /** Returns the simple file name of the audio file assigned to a certain note.
     *
     *  @param noteNo The note number for which the file path shall be returned.
     *
     *  @return The simple file name of the file assigned to that note or an empty
     *      string if no file is assigned.
     */
    const String getFileName(int noteNo) const;
    
    /** Sets the velocity to be used for playback of a specified note. Only to
     *  be used if a sample is actually assigned to that note.
     *
     *  @param noteNo The note number for which the velocity shall be set.
     *  @param velocity The velocity to be set (0.0 - 1.0 inclusive).
     *
     *  @return true on success, false if no file is assigned to that note.
     */
    bool setVelocity(int noteNo, float velocity);
    
    /** Returns the velocity that is set for playback of a specified note.
     *
     *  @param noteNo The note number for which the velocity shall be returned.
     *
     *  @return The velocity set for that note number or 0.0 if no sample is
     *      assigned to that note (or noteNo is out of range).
     */
    float getVelocity(int noteNo) const;
    
    /** Returns true of a sample is assigned to the specified note.
     *
     *  @param noteNo The number of the note to be checked.
     *
     *  @return true if a sample is assigned to the specified note.
     */
    bool sampleIsLoaded(int noteNo) const;
    
    /** NoteOff messages are ignored for one-shot playback behaviour.
     *  Hence, this overrides Synthesiser::noteOff.
     */
    void noteOff (const int midiChannel, const int midiNoteNumber,
                  const bool allowTailOff) override;
    
private:
    
    AudioFormatManager formatManager_;
    
    /** Sets a new sound, replacing the sound previuosly at that index. */
    void setSound (int index, const SynthesiserSound::Ptr& newSound);

    /**
     *  Dummy sound needed to always have 128 sounds in the synth (which
     *  makes the management of "1 sound per note" much easier).
     */
    struct SilentSound   :   SynthesiserSound
    {
        bool appliesToNote(const int midiNoteNumber){ return false; }
        bool appliesToChannel (const int midiChannel){ return false; }
    };
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SampleSynth)
};


#endif  // SAMPLESYNTH_H_INCLUDED

# JLickshot

This is JLickshot, a simple sample player for OSX and Windows to be used for one-shot playback of audio samples (the name being both a tribute to the great Joe Lickshot and an indicator that this software uses the [JUCE library](http://www.juce.com/)). For the main output, it features a simple delay unit and a reverb unit based on [MVerb by Martin Eastwood](https://github.com/martineastwood/mverb).

Samples are triggered via MIDI. Each sample is assigned to one note. Since the main use case for this software is triggering short samples during DJ performances, the velocity value recieved via MIDI is ignored and replaced by a fixed value (which can be set for each sample as an invidiual gain per sample).

## Download runnable binaries

For downloads of the latest pre-compiled runnable binaries, visit the [releases](https://github.com/MartinHH/JLickshot/releases) section.

## Features

These are the main features of JLickshot:

* plays one audio file per MIDI note
* samples are played "one-shot"-style (once triggered, the samples plays until its end regardless of the key being released)
* individual gain settings for each note/sample
* ignores (keystroke) velocity
* simple delay effect
* reverb effect (based on [MVerb by Martin Eastwood](https://github.com/martineastwood/mverb))
* master gain
* save and restore presets
* save presets and audio files to one folder so presets can be transfered from machine to machine (along with their audio files)

## Limitations

A few limitations are in place for performance reasons:

* polyphony is limited to eight voices (meaning you can play a maximum of eight samples simultaneously)
* the maximum length of a sample is 60 seconds -- you can load longer files, but only the first 60 seconds will be played
## Disclaimer: Don't expect too much

This software was designed in very short time to suit personal needs - don't expect it to be anything else but a tiny (free) tool for a very specific use case.
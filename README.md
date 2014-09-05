# JLickshot

This is JLickshot, a simple sample player for OSX and Windows to be used for one-shot playback of audio samples (the name being both a tribute to the great Joe Lickshot and an indicator that this software uses the [JUCE library](http://www.juce.com/)). For the main output, it features a simple delay unit and a reverb unit based on [MVerb by Martin Eastwood](https://github.com/martineastwood/mverb).

Samples are triggered via MIDI. Each sample is assigned to one note. Since the main use case for this software is triggering short samples during DJ performances, the velocity value recieved via MIDI is ignored and replaced by a fixed value (which can be set for each sample as an invidiual gain per sample).

## Disclaimer: Don't expect too much

This software was designed in very short time to suit personal needs - don't expect it to be anything else but a tiny (free) tool for a very specific use case.
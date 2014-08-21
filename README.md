# JLickshot

This is JLickshot, a simple sample player for OSX and Windows to be used for one-shot playback of audio samples (the name being both a tribute to the great Joe Lickshot and an indicator that this software uses the [JUCE library](http://www.juce.com/)).

Samples are triggered via MIDI. Each sample is assigned to one note. Since the main use case for this software is triggering short samples during DJ performances, the velocity value recieved via MIDI is ignored and replaced by a fixed value (which can be set for each sample).

## Disclaimer: Don't expect too much

This software was designed int very short time to suit personal needs - don't expect it to be anyhting else but a tiny (free) tool for a very specific use case.
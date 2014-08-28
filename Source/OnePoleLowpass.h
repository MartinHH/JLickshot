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

#ifndef ONEPOLELOWPASS_H_INCLUDED
#define ONEPOLELOWPASS_H_INCLUDED

#define _USE_MATH_DEFINES
#include <math.h>

class OnePoleLowpass
{
public:
    OnePoleLowpass():
        a0(1.0), b1(0.0), z1(0.0),freq_(INFINITY), sampleRate_(44100)
    {};
    
    OnePoleLowpass(double freq, int sampleRate):
        z1(0.0), sampleRate_(sampleRate)
    {setFrequency(freq);}
    
    virtual ~OnePoleLowpass(){}
    
    void setFrequency(double freq);
    
    void setSampleRate(int sampleRate){ sampleRate_ = sampleRate; }
    
    double getFrequency() const {return freq_;}
    
    double process(double sample);
    
private:
    double a0, b1, z1;
    double freq_;
    int sampleRate_;
};

inline void OnePoleLowpass::setFrequency(double freq)
{
    freq_ = freq;
    b1 = exp(-2.0 * M_PI * freq_ / sampleRate_);
    a0 = 1.0 - b1;
}

inline double OnePoleLowpass::process(double sample)
{
    z1 = sample * a0 + z1 * b1;
    return z1;
}


#endif  // ONEPOLELOWPASS_H_INCLUDED

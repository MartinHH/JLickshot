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

#ifndef MVERBPLUS_H_INCLUDED
#define MVERBPLUS_H_INCLUDED

#include "JuceHeader.h"
#include "MVerb.h"

/** An MVerb<float> with added methods for storing and restoring its
 *  state to/from XML.
 */
class MVerbPlus :   public MVerb<float>
{
public:
    MVerbPlus();
    virtual ~MVerbPlus();
    
    XmlElement* getStateXml() const;
    
    void updateFromXml(const XmlElement* stateXml);
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MVerbPlus)
};



#endif  // MVERBPLUS_H_INCLUDED

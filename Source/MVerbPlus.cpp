/*
 * Copyright 2014-2021 Martin Hansen
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

#include "MVerbPlus.h"

MVerbPlus::MVerbPlus()
{
    // initialize with "subtle" preset (by Martin Eastwood)
    setParameter(DAMPINGFREQ, 0.0);
	setParameter(DENSITY, 0.5);
	setParameter(BANDWIDTHFREQ, 1.0);
	setParameter(DECAY, 0.5);
	setParameter(PREDELAY, 0.0);
	setParameter(SIZE, 0.5);
    setParameter(GAIN, 1.0);
	setParameter(MIX, 0.15);
	setParameter(EARLYMIX, 0.75);
}

MVerbPlus::~MVerbPlus()
{
}

XmlElement* MVerbPlus::getStateXml() const
{
    XmlElement* rv = new XmlElement("MVERBSETTINGS");
    
    for(int i = 0; i < NUM_PARAMS; i++){
        const String attrName = translate("ATTRIBUTE") + String(i);
        rv->setAttribute(attrName, getParameter(i));
    }
    
    return rv;
}

void MVerbPlus::updateFromXml(const XmlElement *stateXml)
{
    if (stateXml != nullptr && stateXml->hasTagName("MVERBSETTINGS")) {
        for(int i = 0; i < NUM_PARAMS; i++){
            const String attrName = translate("ATTRIBUTE") + String(i);
            setParameter(i, stateXml->getDoubleAttribute(attrName, getParameter(i)));
        }
    }
}

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

#ifndef JLICKSHOTCONTROLLERBASE_H_INCLUDED
#define JLICKSHOTCONTROLLERBASE_H_INCLUDED


#include "JuceHeader.h"
#include "SampleChoiceComponent.h"
#include "DelayComponent.h"
#include "MVerbComponent.h"
#include "JLickshotProcessorBase.h"

/**
 *  An interface describing the methods a controller must offer to the GUI.
 */
class JLickshotControllerBase   :   public SampleChoiceComponent::Listener,
                                    public DelayComponent::Listener,
                                    public MVerbComponent::Listener
{
public:
    virtual ~JLickshotControllerBase(){}
    
    virtual JLickshotProcessorBase& getProcessor() = 0;
    
    /** Saves the state of the controller in XML format to a specified
     *  file. Saves the filepaths and velocity settings of the loaded
     *  samples, the master gain and the settings of the AudioDeviceManager
     *  (if those differ from its default settings).
     *
     *  @return true on success, false on any error.
     *
     *  @see loadState
     */
    virtual bool saveState(const File& xmlDest, bool toOneDir = false) = 0;
    
    /** Restores the state of the controller from an XML file generated
     *  using saveState.
     *
     *  @return a LoadResult struct containing details of the operation.
     *
     *  @see saveState
     */
    virtual SampleSynth::LoadResult loadState(const File& xmlSource) = 0;
};


#endif  // JLICKSHOTCONTROLLERBASE_H_INCLUDED

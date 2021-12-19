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

#include "JuceHeader.h"
#include "MainComponent.h"
#include "MainController.h"


//==============================================================================
class JLickshotApplication  : public JUCEApplication
{
public:
    JLickshotApplication() {}

    const String getApplicationName() override       { return ProjectInfo::projectName; }
    const String getApplicationVersion() override    { return ProjectInfo::versionString; }
    bool moreThanOneInstanceAllowed() override       { return true; }

    void initialise (const String& /* commandLine */) override
    {
        mainController = new MainController();
        mainWindow = new MainWindow(mainController);
        toolTipWindow = new TooltipWindow(mainWindow, 2000);
    }

    void shutdown() override
    {
        toolTipWindow = nullptr;
        mainWindow = nullptr;
        mainController = mainController;
    }

    void systemRequestedQuit() override
    {
        quit();
    }

    //==============================================================================
    /**
     *  This class implements the desktop window that contains an instance of
     *  our MainContentComponent class.
     */
    class MainWindow    : public DocumentWindow
    {
    public:
        MainWindow(MainController* controller)
            : DocumentWindow (translate("JLickshot ") + translate(VERSION_STRING),
                              Colours::lightgrey,
                              DocumentWindow::allButtons)
        {
            setContentOwned (new MainContentComponent(controller, &controller->getAudioDeviceManager()),
                             true);

            centreWithSize (getWidth(), getHeight());
            setResizeLimits(472, 442, getWidth(), 800);
            setResizable(true, true);
            setVisible (true);
        }

        void closeButtonPressed()
        {
            JUCEApplication::getInstance()->systemRequestedQuit();
        }

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainWindow)
    };

private:
    ScopedPointer<MainController> mainController;
    ScopedPointer<MainWindow> mainWindow;
    ScopedPointer<TooltipWindow> toolTipWindow;
};

//==============================================================================
// This macro generates the main() routine that launches the app.
START_JUCE_APPLICATION (JLickshotApplication)

/*
  ==============================================================================

	This file was auto-generated by the Introjucer!

	It contains the basic startup code for a Juce application.

  ==============================================================================
*/
/*
  ==============================================================================
This file is part of MIDI2LR. Copyright 2015-2016 by Rory Jaffe.

MIDI2LR is free software: you can redistribute it and/or modify it under the
terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later version.

MIDI2LR is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
MIDI2LR.  If not, see <http://www.gnu.org/licenses/>.
  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "MainComponent.h"
#include "LR_IPC_OUT.h"
#include "LR_IPC_IN.h"
#include "VersionChecker.h"
#include "MainWindow.h"
#include "CommandMap.h"

#define SHUT_DOWN_STRING "--LRSHUTDOWN"

class MIDI2LRApplication : public JUCEApplication
{
public:
	MIDI2LRApplication() {}

	const String getApplicationName() override { return ProjectInfo::projectName; }
	const String getApplicationVersion() override { return ProjectInfo::versionString; }
	bool moreThanOneInstanceAllowed() override { return false; }

	//==============================================================================
	void initialise(const String& commandLine) override
	{
		if (commandLine != SHUT_DOWN_STRING)
		{
			
			//set the reference to the command map
			m_profileManager.SetCommandMap(&m_commandMap);

	        mainWindow = new MainWindow(getApplicationName());
		    mainWindow->Init();
		    // Check for latest version
		    _versionChecker.startThread();
		}
		else
	    {
	        // apparently the appication is already terminated
	        quit();
	    }
	        
	}

	void shutdown() override
	{
		// Save the current profile as default.xml
		File defaultProfile = File::getSpecialLocation(File::currentExecutableFile).getSiblingFile("default.xml");
		m_commandMap.toXMLDocument(defaultProfile);

		LR_IPC_OUT::getInstance().shutdown();
		m_lr_IPC_IN.shutdown();
		mainWindow = nullptr; // (deletes our window)
		quit();
	}

	//==============================================================================
	void systemRequestedQuit() override
	{
		// This is called when the app is being asked to quit: you can ignore this
		// request and let the app carry on running, or call quit() to allow the app to close.
		quit();
	}

	void anotherInstanceStarted(const String& commandLine) override
	{
		// When another instance of the app is launched while this one is running,
		// this method is invoked, and the commandLine parameter tells you what
		// the other instance's command-line arguments were.

		if (commandLine == SHUT_DOWN_STRING)
		{
			//shutting down
			this->shutdown();
		}

	}


private:
	ScopedPointer<MainWindow> mainWindow;
	VersionChecker _versionChecker;

	// the array with the commands
	CommandMap m_commandMap;
	LR_IPC_IN m_lr_IPC_IN;
	LR_IPC_OUT m_lr_IPC_OUT;
	ProfileManager m_profileManager;
};

//==============================================================================
// This macro generates the main() routine that launches the app.
START_JUCE_APPLICATION(MIDI2LRApplication)

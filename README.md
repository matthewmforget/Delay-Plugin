# Delay-Plugin

**Overview**
This is a delay audio plugin built using the JUCE framework. The plugin processes incoming audio and applies various delay effects, including customizable feedback, delay time, and wet/dry mix. Additionally, it features a Ping-Pong Delay mode, which creates an interesting stereo bouncing delay effect.

**Features**
Feedback Slider: Controls how much of the delayed signal is fed back into the delay line, affecting how long the echoes last.
Delay Time Slider: Adjusts the time between repeats of the delayed signal.
Wet Slider: Controls the level of the delayed signal in the mix.
Dry Slider: Controls the level of the original (unaffected) signal in the mix.
Ping Pong Button: Toggles a special delay mode where the echoes alternate between the left and right channels, creating a "ping-pong" effect.
**Files Included**
PluginProcessor.cpp
PluginEditor.cpp
PluginProcessor.h
PluginEditor.h
DelayA2.jucer
These files contain the source code and project configuration necessary to build and run the plugin.

**Installation & Usage**

**Building the Plugin (macOS - AU)**
Prerequisites:

JUCE Framework: Download JUCE from JUCE's official website.
Xcode: Install Xcode from the Mac App Store for compiling the project.
Steps:

Clone the Repository:
bash:

git clone https://github.com/<your-username>/<your-repo>.git

Open in Projucer:
Open Projucer from your JUCE installation.
Load the .jucer file by clicking Open Existing Project and navigating to DelayA2.jucer.
Make sure AU (Audio Unit) is enabled under Plugin Formats in the project settings.
Open in Xcode:
In Projucer, click Save Project and Open in IDE. This will generate Xcode project files and open them in Xcode.
Build in Xcode:
In Xcode, select the Audio Unit target from the scheme dropdown.
Hit Cmd + R or click Run to build the plugin.
Once the build is complete, you will find the .component file in the build folder.
Install the Plugin:
Copy the generated .component file to your Audio Unit plugin folder:
bash:

/Library/Audio/Plug-Ins/Components

Use the Plugin:
Open your DAW (e.g., Logic Pro, Ableton Live).
Rescan your plugins if needed, and insert the delay plugin on an audio track. Adjust the feedback, delay time, wet/dry mix, and experiment with the ping-pong effect for a wide range of delay sounds.

**Building the Plugin (VST - Windows/Linux/macOS)**
Prerequisites:

JUCE Framework: Download JUCE from JUCE's official website.
CMake (for Linux) or Xcode (for macOS) or Visual Studio (for Windows) for compiling the project.
Steps:

Clone the Repository:
bash

git clone https://github.com/<your-username>/<your-repo>.git

Open in Projucer:
Open Projucer and load the .jucer file (DelayPlugin.jucer).
In Projucer, make sure VST3 is enabled under Plugin Formats in the project settings.
Open in Your IDE:
In Projucer, click Save Project and Open in IDE. This will open the project in Xcode (macOS), Visual Studio (Windows), or generate build files for CMake (Linux).
Build the Plugin:
macOS: Follow the same steps as the AU build, but make sure the VST3 target is selected.
Windows: Use Visual Studio to compile the project.
Linux: Use CMake to build the project:
bash:

mkdir build
cd build
cmake ..
make
Install the Plugin:
Copy the generated .vst3 file to the appropriate VST3 plugin folder:
macOS: /Library/Audio/Plug-Ins/VST3
Windows: C:\Program Files\Common Files\VST3
Linux: Place the plugin in your DAW's VST3 folder, typically located under ~/.vst3.
Use the Plugin:
Open your DAW (e.g., Ableton Live, Reaper, Cubase).
Rescan your plugins if needed, and insert the delay plugin on an audio track. Adjust the feedback, delay time, wet/dry mix, and explore the ping-pong effect for a dynamic delay sound.

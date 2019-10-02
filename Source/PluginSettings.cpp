/*
	Permission is hereby granted, free of charge, to any person
	obtaining a copy of this software and associated documentation files
	(the "Software"), to deal in the Software without restriction,
	including without limitation the rights to use, copy, modify, merge,
	publish, distribute, sublicense, and/or sell copies of the Software,
	and to permit persons to whom the Software is furnished to do so,
	subject to the following conditions:
	The above copyright notice and this permission notice shall be
	included in all copies or substantial portions of the Software.
	Any person wishing to distribute modifications to the Software is
	asked to send the modifications to the original developer so that
	they can be incorporated into the canonical version.  This is,
	however, not a binding provision of this license.
	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
	EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
	MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
	IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR
	ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
	CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
	WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginSettings.h"

//==============================================================================
AutomationBridgeSettings::AutomationBridgeSettings (AutomationBridgeEditor& e)
    : DocumentWindow ("Automation Bridge Settings", DocumentWindow::backgroundColourId,
      DocumentWindow::minimiseButton | DocumentWindow::closeButton, false),
    editor (e)
{
    centreWithSize (300, 300);
    setVisible (true);
    setUsingNativeTitleBar (true);

    path = File::getSpecialLocation (File::userApplicationDataDirectory).getFullPathName();
    path += File::getSeparatorString();
#ifdef __APPLE__
	path += "Preferences/AutomationBridge.dat";
#else
    path += "AutomationBridge.dat";
#endif

    addAndMakeVisible (fadersSlider);
    fadersSlider.setSliderStyle (Slider::LinearBar);
    fadersSlider.setTextSuffix (" faders");

    addAndMakeVisible (cancelButton);
    cancelButton.setButtonText ("Cancel");
    cancelButton.onClick = [this] {
		load();
		editor.findParentComponentOfClass<DocumentWindow>()->exitModalState (0);
    };

    addAndMakeVisible (applyButton);
    applyButton.setButtonText ("Apply");
    applyButton.onClick = [this] {
		save();
    };

    addAndMakeVisible (saveButton);
    saveButton.setButtonText ("Save");
    saveButton.onClick = [this] {
		save();
		editor.findParentComponentOfClass<DocumentWindow>()->exitModalState (0);
    };

    load();
}

AutomationBridgeSettings::~AutomationBridgeSettings()
{
}

int AutomationBridgeSettings::getFaderCount() const
{
	return faders;
}

int AutomationBridgeSettings::getWidth() const
{
	return width;
}

int AutomationBridgeSettings::getHeight() const
{
	return height;
}

MidiDeviceInfo* AutomationBridgeSettings::getInput(int i) const
{
	return inputs[i];
}

MidiDeviceInfo* AutomationBridgeSettings::getOutput(int i) const
{
	return outputs[i];
}

void AutomationBridgeSettings::save() const
{
	FileOutputStream fs(path);

	if (fs.openedOk())
	{
		fs.writeInt (1); // format version for future revisions
		fs.writeInt (faders);
		fs.writeInt (width);
		fs.writeInt (height);
		fs.writeInt (inputs.size());
		fs.writeInt (outputs.size());

		for (MidiDeviceInfo *info : inputs) fs.writeString (info->identifier);
		for (MidiDeviceInfo *info : outputs) fs.writeString (info->identifier);

		fs.flush();
	}
}

void AutomationBridgeSettings::load()
{
	File f(path);

	if (f.exists() && f.existsAsFile())
	{
		FileInputStream fs(path);

		if (fs.openedOk())
		{
			fs.readInt(); // format version is 1, ignore for now
			faders = fs.readInt();
			width = fs.readInt();
			height = fs.readInt();
			int inputsSize = fs.readInt();
			int outputsSize = fs.readInt();

			StringArray inputIds;
			for (int i = 0; i < inputsSize; i++) inputIds.add (fs.readString());

			StringArray outputIds;
			for (int i = 0; i < outputsSize; i++) outputIds.add (fs.readString());

			for (MidiDeviceInfo &info : MidiInput::getAvailableDevices())
				if (inputIds.contains (info.identifier) inputs.add (&info);

			for (MidiDeviceInfo &info : MidiOutput::getAvailableDevices())
				if (outputIds.contains (info.identifier) outputs.add (&info);
		}
	}
}

void AutomationBridgeSettings::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setColour (Colours::white);
    g.setFont (14.0f);
}

void AutomationBridgeSettings::resized()
{
    Rectangle<int> area = getLocalBounds();
}

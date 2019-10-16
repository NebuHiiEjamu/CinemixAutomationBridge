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
#include "DeviceListBox.h"
#include "PluginEditor.h"
#include "PluginMainPanel.h"

//==============================================================================
AutomationBridgeSettings::AutomationBridgeSettings (AutomationBridgeEditor* e)
{
    path = File::getSpecialLocation (File::userApplicationDataDirectory).getFullPathName();
    path += File::getSeparatorString();
#ifdef __APPLE__
	path += "Preferences/AutomationBridge.dat";
#else
    path += "AutomationBridge.dat";
#endif
    
    editor = e;
    
    load();

    addAndMakeVisible (fadersSlider);
    fadersSlider.setSliderStyle (Slider::LinearBar);
    fadersSlider.setTextValueSuffix (" faders");
	fadersSlider.setRange (48.0, 128.0, 2.0);
	fadersSlider.onValueChange = [this] {
		faders = roundToInt (fadersSlider.getValue());
	};

	inDevices = MidiInput::getAvailableDevices();
    outDevices = MidiOutput::getAvailableDevices();

	inputs = std::make_unique<DeviceListBox> (inDevices, inputsOn);
	addAndMakeVisible (inputs.get());
	Label inputsLbl;
    inputsLbl.setText ("Input Devices:", NotificationType::dontSendNotification);
	inputsLbl.attachToComponent (inputs.get(), false);

	outputs = std::make_unique<DeviceListBox> (outDevices, outputsOn);
	addAndMakeVisible (outputs.get());
	Label outputsLbl;
	outputsLbl.setText ("Output Devices:", NotificationType::dontSendNotification);
	outputsLbl.attachToComponent (outputs.get(), false);

    addAndMakeVisible (cancelButton);
    cancelButton.setButtonText ("Cancel");
    cancelButton.onClick = [this] {
        load();
        editor->getMainPanel()->setVisible (true);
        setVisible (false);
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
        editor->getMainPanel()->setVisible (true);
        setVisible (false);
    };
    
    setOpaque (true);
    setVisible (false);
}

AutomationBridgeSettings::~AutomationBridgeSettings()
{
}

int AutomationBridgeSettings::getFaderCount() const
{
	return faders;
}

MidiDeviceInfo AutomationBridgeSettings::getActiveInput(int i) const
{
	return inDevices[inputsOn[i]];
}

MidiDeviceInfo AutomationBridgeSettings::getActiveOutput(int i) const
{
	return outDevices[outputsOn[i]];
}

void AutomationBridgeSettings::save() const
{
	FileOutputStream fs(path);

	if (fs.openedOk())
	{
        fs.truncate();
		fs.writeInt (1); // format version for future revisions
		fs.writeInt (faders);
		fs.writeInt (editor->getWidth());
		fs.writeInt (editor->getHeight());
		fs.writeInt (inputsOn.size());
		fs.writeInt (outputsOn.size());

		for (int i : inputsOn) fs.writeString (inDevices[i].identifier);
		for (int i : outputsOn) fs.writeString (outDevices[i].identifier);

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
            editor->setSize (fs.readInt(), fs.readInt());
			int inputsSize = fs.readInt();
			int outputsSize = fs.readInt();

			StringArray inputIds;
			for (int i = 0; i < inputsSize; i++) inputIds.add (fs.readString());

			StringArray outputIds;
			for (int i = 0; i < outputsSize; i++) outputIds.add (fs.readString());

			for (int i = 0; i < inDevices.size(); i++)
				if (inputIds.contains (inDevices[i].identifier)) inputsOn.add (i);

			for (int i = 0; i < outDevices.size(); i++)
				if (outputIds.contains (outDevices[i].identifier)) outputsOn.add (i);
		}
	}
    else
    {
        faders = 74; // nothing particularly special about default value
    }
    
    fadersSlider.setValue (static_cast<double> (faders));
}

void AutomationBridgeSettings::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (Colours::blue);

    g.setColour (Colours::white);
    g.setFont (14.0f);
}

void AutomationBridgeSettings::resized()
{
    Rectangle<int> area = getLocalBounds();
	Rectangle<int> footer = area.removeFromBottom (30);
	saveButton.setBounds (footer.removeFromRight (100));
	applyButton.setBounds (footer.removeFromRight (100));
	cancelButton.setBounds (footer.removeFromRight (100));
	fadersSlider.setBounds (area.removeFromTop (25));
	inputs->setBounds (area.removeFromTop (area.getHeight() / 2));
	outputs->setBounds (area);
}

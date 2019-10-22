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
AutomationBridgeSettings::AutomationBridgeSettings (AutomationBridgeEditor& e)
: editor (e)
{
    path = File::getSpecialLocation (File::userApplicationDataDirectory).getFullPathName();
    path += File::getSeparatorString();
#ifdef __APPLE__
	path += "Preferences/AutomationBridge.dat";
#else
    path += "AutomationBridge.dat";
#endif
    
    faders = 48; // failsafe
    load();
    refresh();

    addAndMakeVisible (fadersSlider);
    fadersSlider.setSliderStyle (Slider::LinearBar);
    fadersSlider.setTextValueSuffix (" faders");
	fadersSlider.setRange (48.0, 128.0, 2.0);
	fadersSlider.onValueChange = [this] {
		faders = roundToInt (fadersSlider.getValue());
	};

	inputs = std::make_unique<DeviceListBox> (e.processor.inDevices, e.processor.inputsOn);
	addAndMakeVisible (inputs.get());
    inputsLabel.setText ("Input Devices:", NotificationType::dontSendNotification);
	inputsLabel.attachToComponent (inputs.get(), false);

	outputs = std::make_unique<DeviceListBox> (e.processor.outDevices, e.processor.outputsOn);
	addAndMakeVisible (outputs.get());
	outputsLabel.setText ("Output Devices:", NotificationType::dontSendNotification);
	outputsLabel.attachToComponent (outputs.get(), false);

    addAndMakeVisible (cancelButton);
    cancelButton.setButtonText ("Cancel");
	cancelButton.changeWidthToFitText();
    cancelButton.onClick = [this, &e] {
        load();
        e.mainPanel->setVisible (true);
        setVisible (false);
    };

    addAndMakeVisible (applyButton);
    applyButton.setButtonText ("Apply");
	applyButton.changeWidthToFitText();
    applyButton.onClick = [this] {
		save();
        unload();
        refresh();
    };

    addAndMakeVisible (saveButton);
    saveButton.setButtonText ("Save");
	saveButton.changeWidthToFitText();
    saveButton.onClick = [this, &e] {
		save();
        unload();
        refresh();
        e.mainPanel->setVisible (true);
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

void AutomationBridgeSettings::save() const
{
	FileOutputStream fs (path);

	if (fs.openedOk())
	{
		fs.setPosition (0);
		fs.truncate();
		
		fs.writeInt (1); // format version for future revisions
		fs.writeInt (faders);
		fs.writeInt (editor.getWidth());
		fs.writeInt (editor.getHeight());
		fs.writeInt (editor.processor.inputsOn.size());
		fs.writeInt (editor.processor.outputsOn.size());

		for (int i : editor.processor.inputsOn)
			fs.writeString (editor.processor.inDevices[i].identifier);
		for (int i : editor.processor.outputsOn)
			fs.writeString (editor.processor.outDevices[i].identifier);

		fs.flush();
	}
}

void AutomationBridgeSettings::unload()
{
    editor.mainPanel->faders.clear();
    editor.mainPanel->mutes.clear();
    editor.mainPanel->faderIds.clear();
}

void AutomationBridgeSettings::refresh()
{
    for (int i = 0; i < faders; i++)
    {
        Slider* newFader = new Slider();
        editor.mainPanel->addAndMakeVisible (dynamic_cast<Component*> (newFader));
        newFader->setSliderStyle (Slider::LinearVertical);
        newFader->setRange (0.0, 127.0, 1.0);
        editor.mainPanel->faders.add (newFader);
        
        TextButton* newMute = new TextButton("M");
        editor.mainPanel->addAndMakeVisible (dynamic_cast<Component*> (newMute));
        newMute->changeWidthToFitText();
        newMute->setClickingTogglesState (true);
        editor.mainPanel->mutes.add (newMute);
        
        Label* newLabel = new Label();
        editor.mainPanel->addAndMakeVisible (dynamic_cast<Component*> (newLabel));
        newLabel->setText (String (i + 1), NotificationType::dontSendNotification);
        editor.mainPanel->faderIds.add (newLabel);
    }
    
    editor.mainPanel->resized();
}

void AutomationBridgeSettings::load()
{
	File f (path);

	if (f.exists() && f.existsAsFile())
	{
		FileInputStream fs (path);

		if (fs.openedOk())
		{
			fs.readInt(); // format version is 1, ignore for now
			faders = fs.readInt();
            editor.setSize (fs.readInt(), fs.readInt());
			int inputsSize = fs.readInt();
			int outputsSize = fs.readInt();

			StringArray inputIds;
			for (int i = 0; i < inputsSize; i++) inputIds.add (fs.readString());

			StringArray outputIds;
			for (int i = 0; i < outputsSize; i++) outputIds.add (fs.readString());

			for (int i = 0; i < editor.processor.inDevices.size(); i++)
				if (inputIds.contains (editor.processor.inDevices[i].identifier))
					editor.processor.inputsOn.add (i);

			for (int i = 0; i < editor.processor.outDevices.size(); i++)
				if (outputIds.contains (editor.processor.outDevices[i].identifier))
					editor.processor.outputsOn.add (i);
		}
	}
    else
    {
        // nothing particularly special about default value, but let's make it more than minimum
        faders = 74;
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
	inputs->setBounds (area.removeFromTop (area.getHeight() / 2).reduced (0, 25));
    outputs->setBounds (area);
}

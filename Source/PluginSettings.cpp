/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street - Fifth Floor, Boston, MA 02110-1301, USA.
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
    
    load();
    refresh();

    addAndMakeVisible (fadersSlider);
    fadersSlider.setSliderStyle (Slider::LinearBar);
    fadersSlider.setTextValueSuffix (" faders");
	fadersSlider.setRange (48.0, 128.0, 2.0);
	fadersSlider.onValueChange = [this, &e] {
		e.processor.faders = roundToInt (fadersSlider.getValue());
	};

    addAndMakeVisible (channelsSlider);
    channelsSlider.setSliderStyle (Slider::LinearBar);
    channelsSlider.setTextValueSuffix (" channels");
	channelsSlider.setRange (1.0, 64.0, 1.0);
	channelsSlider.onValueChange = [this, &e] {
		e.processor.channels = roundToInt (channelsSlider.getValue());
	};

	inputs = std::make_unique<DeviceListBox> (e.processor.inDeviceData, e.processor.inputsOn);
	addAndMakeVisible (inputs.get());
    inputsLabel.setText ("Input Devices:", NotificationType::dontSendNotification);
	inputsLabel.attachToComponent (inputs.get(), false);

	outputs = std::make_unique<DeviceListBox> (e.processor.outDeviceData, e.processor.outputsOn);
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

void AutomationBridgeSettings::save() const
{
	FileOutputStream fs (path);

	if (fs.openedOk())
	{
		fs.setPosition (0);
		fs.truncate();
		
		fs.writeInt (1); // format version for future revisions
		fs.writeInt (editor.processor.faders);
        fs.writeInt (editor.processor.channels);
		fs.writeInt (editor.getWidth());
		fs.writeInt (editor.getHeight());
		fs.writeInt (editor.processor.inputsOn.size());
		fs.writeInt (editor.processor.outputsOn.size());

		for (int i : editor.processor.inputsOn)
			fs.writeString (editor.processor.inDeviceData[i].identifier);
		for (int i : editor.processor.outputsOn)
			fs.writeString (editor.processor.outDeviceData[i].identifier);

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
    for (int i = 0; i < editor.processor.faders; i++)
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
    
    editor.processor.refresh();
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
			editor.processor.faders = fs.readInt();
            editor.processor.channels = fs.readInt();
            editor.setSize (fs.readInt(), fs.readInt());
			int inputsSize = fs.readInt();
			int outputsSize = fs.readInt();

			StringArray inputIds;
			for (int i = 0; i < inputsSize; i++) inputIds.add (fs.readString());

			StringArray outputIds;
			for (int i = 0; i < outputsSize; i++) outputIds.add (fs.readString());

			for (int i = 0; i < editor.processor.inDeviceData.size(); i++)
				if (inputIds.contains (editor.processor.inDeviceData[i].identifier))
					editor.processor.inputsOn.add (i);

			for (int i = 0; i < editor.processor.outDeviceData.size(); i++)
				if (outputIds.contains (editor.processor.outDeviceData[i].identifier))
					editor.processor.outputsOn.add (i);
		}
	}
    else
    {
		editor.setSize (1200, 720);
    }
    
    fadersSlider.setValue (static_cast<double> (editor.processor.faders));
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
	fadersSlider.setBounds (area.removeFromTop (25).reduced (2, 100));
    channelsSlider.setBounds (area.removeFromTop (25).reduced (2, 100));
	inputs->setBounds (area.removeFromTop (area.getHeight() / 2).reduced (0, 25));
    outputs->setBounds (area);
}

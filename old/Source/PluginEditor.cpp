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

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "PluginMainPanel.h"
#include "PluginSettings.h"

//==============================================================================
AutomationBridgeEditor::AutomationBridgeEditor (AutomationBridge& p)
	: AudioProcessorEditor (&p), processor (p)
{
	setOpaque (true);
	setResizable (true, true);
    
    mainPanel = std::make_unique<PluginMainPanel> (*this);
    prefsPanel = std::make_unique<AutomationBridgeSettings> (*this);
    
    addChildComponent(dynamic_cast<Component*> (prefsPanel.get()));
    addAndMakeVisible(dynamic_cast<Component*> (mainPanel.get()));

	//setSize (1200, 720);
}

AutomationBridgeEditor::~AutomationBridgeEditor()
{
	//deviceManager.removeMidiInputCallback (MidiInput::getDevices()[midiInputList.getSelectedItemIndex()], this);
}

void AutomationBridgeEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (Colours::blue);
}

void AutomationBridgeEditor::resized()
{
    if (mainPanel) mainPanel->setBounds (getLocalBounds());
    if (prefsPanel) prefsPanel->setBounds (getLocalBounds());
}

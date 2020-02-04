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

//==============================================================================
AutomationBridgeEditor::AutomationBridgeEditor (AutomationBridgeProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);

    addAndMakeVisible (testMute);
    testMute.setButtonText ("M");
    testMute.setClickingTogglesState(true);
    testMute.onClick = [&p] {
        uint8_t state = p.muteOn ? 2 : 3;
        std::vector<uint8_t> msg = { 178, 0, state };
        p.muteOn = state == 2 ? false : true;
        p.midiOut->sendMessage(&msg);
    };
}

AutomationBridgeEditor::~AutomationBridgeEditor()
{
}

//==============================================================================
void AutomationBridgeEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setColour (Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Hello World!", getLocalBounds(), Justification::centred, 1);
}

void AutomationBridgeEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    testMute.setBounds (getLocalBounds());
}

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
    setSize ((25*37)+10, 300);
    
    for (int i = 0; i < 74; i++)
    {
        TextButton *muteBtn = new TextButton("M");
        addAndMakeVisible (dynamic_cast<Component*> (muteBtn));
        muteBtn->changeWidthToFitText();
        muteBtn->setClickingTogglesState(true);
        muteBtn->onClick = [&] {
            uint8_t state = p.muteOn[i] ? 2 : 3;
            p.muteOn[i] = state == 2 ? false : true;
            p.sendMidiCC(3, static_cast<uint8_t>(i), state, i < 48 ? p.midiOut[0].get() : p.midiOut[1].get());
        };
        muteBtns.add(muteBtn);
    }
    
    addAndMakeVisible(faderTxt);
    faderTxt.setText("0");
    
    addAndMakeVisible(testFader);
    testFader.setSliderStyle(Slider::LinearHorizontal);
    testFader.setRange(0.0, 127.0, 1.0);
    testFader.onValueChange = [&] {
        for (uint8_t i = 0; i < 3; i++)
        {
            p.sendMidiCC(1, static_cast<uint8_t>(faderTxt.getText().getIntValue()), static_cast<uint8_t>(roundToInt(testFader.getValue())));
        }
    };
    
    addAndMakeVisible(activateBtn);
    activateBtn.setButtonText("Activate");
    activateBtn.onClick = [&] {
        p.sendMidiCC(5, 127, 127);
        
        // Set all channels to Write Mode
        p.setAllChannelsMode(2);
        
        // Set Master Section to Write Mode (Master fader and Joysticks)
        p.sendMidiCC(5, 64, 2);
        p.sendMidiCC(4, 88, 2);
        p.sendMidiCC(4, 90, 2);
        
        // Set all channels to RW
        p.setAllChannelsMode(3);
        
        // Set Master Section to RW (Master fader and Joysticks)
        p.sendMidiCC(5, 64, 3);
        p.sendMidiCC(4, 88, 3);
        p.sendMidiCC(4, 90, 3);
    };
    
    addAndMakeVisible(deactivateBtn);
    deactivateBtn.setButtonText("Deactivate");
    deactivateBtn.onClick = [&] {
        p.sendMidiCC(5, 127, 0);
        
        // Reset all channels
        p.setAllChannelsMode(0);
        
        // Master Section (Master fader and Joysticks)
        p.sendMidiCC(5, 64, 0);
        p.sendMidiCC(4, 88, 0);
        p.sendMidiCC(4, 90, 0);
        
        // Send a single MIDI Byte value FF for System Reset
        std::vector<uint8_t> msg { 255, 0, 0 };
        p.midiOut[0]->sendMessage(&msg);
        p.midiOut[1]->sendMessage(&msg);
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
}

void AutomationBridgeEditor::resized()
{
    Rectangle<int> area = getLocalBounds();
    Rectangle<int> faderArea = area.removeFromTop(25);
    Rectangle<int> muteAreaTop = area.removeFromTop(25);
    Rectangle<int> muteAreaBtm = area.removeFromTop(25);
    Rectangle<int> btnArea = area.removeFromTop(25);
    testFader.setBounds(faderArea.removeFromLeft(200));
    faderTxt.setBounds(faderArea);
    for (int i = 0; i < muteBtns.size(); i+=2)
    {
        muteBtns[i]->setBounds (muteAreaTop.removeFromLeft(25));
        if (i == 72) muteBtns[i+1]->setBounds (muteAreaBtm);
        else muteBtns[i+1]->setBounds (muteAreaBtm.removeFromLeft(25));
    }
    activateBtn.setBounds(btnArea.removeFromLeft(100));
    deactivateBtn.setBounds(btnArea);
}

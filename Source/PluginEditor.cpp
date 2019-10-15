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

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "PluginMainPanel.h"
#include "PluginSettings.h"

//==============================================================================
AutomationBridgeEditor::AutomationBridgeEditor (AutomationBridge& p)
	: AudioProcessorEditor (&p),
	  processor (p)
{
    setSize (1200, 720);
	setOpaque (true);
	setResizable (true, true);
    
    mainPanel = new PluginMainPanel (this);
    addAndMakeVisible(dynamic_cast<Component*>(mainPanel));
    
    prefsPanel = new AutomationBridgeSettings (this);
    addChildComponent(dynamic_cast<Component*>(prefsPanel));

	/*resizer (this, nullptr);
	addAndMakeVisible (resizer);*/
}

AutomationBridgeEditor::~AutomationBridgeEditor()
{
    if (mainPanel) delete mainPanel;
    if (prefsPanel) delete prefsPanel;
    
	//deviceManager.removeMidiInputCallback (MidiInput::getDevices()[midiInputList.getSelectedItemIndex()], this);
}

void AutomationBridgeEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
}

AutomationBridgeSettings* AutomationBridgeEditor::getPrefsPanel()
{
    return prefsPanel;
}

PluginMainPanel* AutomationBridgeEditor::getMainPanel()
{
    return mainPanel;
}

void AutomationBridgeEditor::resized()
{
    if (prefsPanel && prefsPanel->isVisible()) prefsPanel->setBounds (getLocalBounds());
    else if (mainPanel) mainPanel->setBounds (getLocalBounds());
}

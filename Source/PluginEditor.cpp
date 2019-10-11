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
#include "PluginSettings.h"

//==============================================================================
AutomationBridgeEditor::AutomationBridgeEditor (AutomationBridge& p)
	: AudioProcessorEditor (&p),
	  processor (p)
{
    setSize (1200, 720);
	setOpaque (true);
	setResizable (true, true);

	/*resizer (this, nullptr);
	addAndMakeVisible (resizer);*/

	addAndMakeVisible (prefsButton);
	prefsButton.setButtonText ("Settings");
	prefsButton.onClick = [this] {
		prefsWin = new AutomationBridgeSettings (*this);
		RectanglePlacement rp (RectanglePlacement::xMid | RectanglePlacement::yMid |
			RectanglePlacement::doNotResize);
		Rectangle<int> area (0, 0, 640, 480);
		
		prefsWin->setBounds (rp.appliedTo (area,
			Desktop::getInstance().getDisplays().getMainDisplay().userArea.reduced (20)));
		prefsWin->setVisible (true);
	};

	addAndMakeVisible (testModeToggle);
	testModeToggle.setButtonText ("Test Mode");
	testModeToggle.setClickingTogglesState (true);
}

AutomationBridgeEditor::~AutomationBridgeEditor()
{
	//deviceManager.removeMidiInputCallback (MidiInput::getDevices()[midiInputList.getSelectedItemIndex()], this);
}

void AutomationBridgeEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setColour (Colours::white);
    g.setFont (14.0f);
    /*g.drawImage (background, 0, 0, background.getWidth(), background.getHeight(), 0, 0,
		background.getWidth(), background.getHeight());*/
}

void AutomationBridgeEditor::resized()
{
	Rectangle<int> area = getLocalBounds();
	Rectangle<int> header = area.removeFromTop (25);
	Rectangle<int> faderLayout = area.removeFromLeft (area.getWidth() * 0.75f);
	Rectangle<int> faderRowTop = faderLayout.removeFromTop (faderLayout.getHeight() / 2);
	prefsButton.setBounds (header.removeFromLeft (150));
	testModeToggle.setBounds (header.removeFromRight (100));
}

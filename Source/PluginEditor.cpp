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

//==============================================================================
AutomationBridgeEditor::AutomationBridgeEditor (AutomationBridge& p)
	: AudioProcessorEditor (&p),
	  processor (p),
{
    setSize (400, 300);
	midiIn = nullptr;

	Array<MidiDeviceInfo> midiInputs = MidiInput::getAvailableDevices();
	for (int i = 1; i <= midiInputs.size(); i++)
	{
		String s = midiInputs[i-1]->name;
		s += " (";
		s += midiInputs[i-1]->identifier;
		s += ")";
		inputList.addItem(s, i);
	}
	
	addAndMakeVisible (inputList);

	/*// Logic debug
	logBox.setMultiLine (true);
	logBox.setReadOnly (true);
	logBox.setCaretPosition (0);
	addAndMakeVisible (logBox);
	String path = File::getSpecialLocation (File::userDocumentsDirectory).getFullPathName();
	path += File::getSeparatorString();
	path += "cinemix_debug.log";
	FileOutputStream fos(path);
	for (auto& midiInput : midiInputs)
	{
		String s(midiInput);
		s += '\n';
		logBox.insertTextAtCaret (s);

		// and for good measure
		fos.writeText(s, false, false, nullptr);
	}*/
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
    g.setFont (15.f);
    g.drawImage (background, 0, 0, background.getWidth(), background.getHeight(), 0, 0,
		background.getWidth(), background.getHeight());
}

void AutomationBridgeEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}

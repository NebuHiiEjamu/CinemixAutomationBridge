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
AutomationBridgeAudioProcessorEditor::AutomationBridgeAudioProcessorEditor (AutomationBridgeAudioProcessor& p)
	: AudioProcessorEditor (&p),
	  processor (p),
	  lastInputIndex(0),
	  isAddingFromMidiInput(false)
{
    setSize (400, 300);
}

AutomationBridgeAudioProcessorEditor::~AutomationBridgeAudioProcessorEditor()
{
	deviceManager.removeMidiInputCallback (MidiInput::getDevices()[midiInputList.getSelectedItemIndex()], this);
}

//==============================================================================
void AutomationBridgeAudioProcessorEditor::handleIncomingMidiMessage (MidiInput* source, const MidiMessage& message)
{
    const ScopedValueSetter<bool> scopedInputFlag (isAddingFromMidiInput, true);
    (new IncomingMessageCallback (this, message, source->getName()))->post();
}

void AutomationBridgeAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setColour (Colours::white);
    g.setFont (15.f);
    g.drawImage (background, 0, 0, background.getWidth(), background.getHeight(), 0, 0,
		background.getWidth(), background.getHeight());
	
	addAndMakeVisible (midiInputList);
	midiInputList.setTextWhenNoChoicesAvailable ("No MIDI Inputs Enabled");
	auto midiInputs = MidiInput::getDevices();
	midiInputList.addItemList (midiInputs, 0);
	midiInputList.onChange = [this]
	{
		setMidiInput (midiInputList.getSelectedItemIndex());
	};

	// find the first enabled device and use that by default
    for (auto& midiInput : midiInputs)
    {
        if (deviceManager.isMidiInputEnabled (midiInput))
        {
            setMidiInput (midiInputs.indexOf (midiInput));
            break;
        }
    }

	// if no enabled devices were found just use the first one in the list
    if (midiInputList.getSelectedId() == 0) setMidiInput (0);
}

void AutomationBridgeAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}

void AutomationBridgeAudioProcessorEditor::setMidiInput (int index)
{
	auto devices = MidiInput::getDevices();
	deviceManager.removeMidiInputCallback (devices[lastInputIndex], this);
	auto newInput = devices[index];
	if (! deviceManager.isMidiInputEnabled (newInput))
		deviceManager.setMidiInputEnabled (newInput, true);
	deviceManager.addMidiInputCallback (newInput, this);
	midiInputList.setSelectedId (index + 1, dontSendNotification);
	lastInputIndex = index;
}

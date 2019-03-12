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

	This contains the basic framework code for a JUCE plugin editor.
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

AutomationBridgeEditor::AutomationBridgeEditor(AutomationBridge &p):
	AudioProcessorEditor(&p),
	processor(p),
	//background(ImageFileFormat::loadFrom(File(
{
	int sChannels = 1;
	int channelNum = 1;
	setSize(920, 560); // TODO: background size, load from config

	faders.resize(p.numChannelStrips * 2);
	buttons.resize(p.numChannelStrips) * 2;
	for (int i = 0; i < p.numChannelStrips; i++)
	{
		faders[i].setSliderStyle(LinearVertical);
		faders[i].setValue(0.754);
		addAndMakeVisible(faders[i]);

		buttons[i].setButtonText("M");
		addAndMakeVisible(buttons[i]);

		Label upChannelLabel;
		if (i > 23 && i < 28) upChannelLabel.setText("S" + sChannels++)
		else upChannelLabel.setText(channelNum++);
		upChannelLabel.setColour(Label::textColourId, Colour::fromRGB(255, 255, 255));
		addAndMakeVisible(upChannelLabel);
	}
}

AutomationBridgeEditor::~AutomationBridgeEditor()
{
}

void AutomationBridgeEditor::paint(Graphics &g)
{
	// (Our component is opaque, so we must completely fill the background with a solid colour)
	g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));

	g.setColour(Colours::white);
	g.setFont(15.f);
	g.drawImage(bgImage, 0, 0, bgImage.getWidth(), bgImage.getHeight(), 0, 0,
		bgImage.getWidth(), bgImage.getHeight());
}

void AutomationBridgeEditor::resized()
{
}

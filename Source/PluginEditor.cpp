/*
  ==============================================================================

	This file was auto-generated!

	It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AutomationBridgeEditor::AutomationBridgeEditor(CAutomationBridge &p):
	AudioProcessorEditor(&p), m_processor(p),
	m_bgImage(ImageFileFormat::loadFrom(File(
{
	// Make sure that before the constructor has finished, you've set the
	// editor's size to whatever you need it to be.
	setSize(400, 300);
}

AutomationBridgeEditor::~AutomationBridgeEditor()
{
}

//==============================================================================
void AutomationBridgeEditor::paint(Graphics &g)
{
	// (Our component is opaque, so we must completely fill the background with a solid colour)
	g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));

	g.setColour(Colours::white);
	g.setFont(15.0f);
	g.drawImage(bgImage, 0, 0, bgImage.getWidth(), bgImage.getHeight(), 0, 0,
		bgImage.getWidth(), bgImage.getHeight());
}

void AutomationBridgeEditor::resized()
{
	// This is generally where you'll want to lay out the positions of any
	// subcomponents in your editor..
}

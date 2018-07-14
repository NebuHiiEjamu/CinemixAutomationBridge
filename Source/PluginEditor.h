/*
  ==============================================================================

	This file was auto-generated!

	It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class CAutomationBridgeEditor: public AudioProcessorEditor
{
public:	
	CAutomationBridgeEditor(CAutomationBridge&);
	~CAutomationBridgeEditor();

	//==============================================================================
	void paint(Graphics&) override;
	void resized() override;

private:
	// This reference is provided as a quick way for your editor to
	// access the processor object that created it.
	CAutomationBridge &m_processor;
	Image m_bgImage;
	Slider m_masterFader;
	Array<Slider> m_faders;
	ToggleButton m_joyMute[2], m_testMode, m_auxMute[10];
	Array<ToggleButton> m_buttons;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CAutomationBridgeEditor)
};

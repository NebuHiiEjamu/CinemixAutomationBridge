/*
  ==============================================================================

	It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/**
*/
class CAutomationBridge: public AudioProcessor
{
public:
	static const float ONE_127TH;
	static const float ONE_255TH;
	static const float ONE_16384TH;
	static const float RAND_MAX_RECIP;
public:
	//==============================================================================
	CAutomationBridge();	
	~CAutomationBridge();

	//==============================================================================
	void prepareToPlay(double fSampleRate, int iSamplesPerBlock) override;
	void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
	bool isBusesLayoutSupported(const BusesLayout &layouts) const override;
#endif

	void processBlock(AudioBuffer<float>&, MidiBuffer&) override;

	//==============================================================================
	AudioProcessorEditor* createEditor() override;
	bool hasEditor() const override;

	//==============================================================================
	const String getName() const override;

	bool acceptsMidi() const override;
	bool producesMidi() const override;
	bool isMidiEffect() const override;
	double getTailLengthSeconds() const override;

	//==============================================================================
	int getNumPrograms() override;
	int getCurrentProgram() override;
	void setCurrentProgram(int iIndex) override;
	const String getProgramName(int iIndex) override;
	void changeProgramName(int iIndex, const String &sNewName) override;

	//==============================================================================
	void getStateInformation(MemoryBlock &destData) override;
	void setStateInformation(const void* pData, int iSizeInBytes) override;
	
	void toggleTestMode();
	
	bool openMidiInPort(int iPort);
	bool openMidiOutPort(int iPort);
	
	//! Close MIDI Ports
	void closeAllMidiPorts();
	
	//! Open MIDI Ports
	void openAllMidiPorts();
	
	void setAllChannelsMode(int iMode);
	void activateMixer();
	void deactivateMixer();
	void sendSnapshot();
	void resetAllMixer();
	void toggleAllMutes();
	void saveConfig() const;
	void loadConfig();
	static String getConfigFilePath();
	static String getResFilePath();
	float randGen() const;
private:
	bool m_bTestMode;
	bool m_bAllMutesStatus;
	bool m_bFullInit;
	int m_iMidiInPorts[2];
	int m_iNumMidiInPorts;
	int m_iNumMidiOutPorts;
	int m_iMidiOutPorts[2];
	const int m_iNumFaders;
	const int m_iNumParams;
	const int m_iNumChannelStrips;
	const OwnedArray<AudioProcessorParameter> &m_params;
	StringArray m_sMidiInPortName;
	StringArray m_sMidiOutPortName;
	Array<int> m_iMidiCtrlParams;
	Array<int> m_iMidiCtrlFaders;
	int m_iFaderModeLeft[48];
	int m_iFaderModeRight[48];
	int m_iFaderModeMaster;
	Array<int> m_iMidiChannel;
	Array<int> m_iPrevCCVal;
	int m_iFaderSpeed;
	int m_iMuteSpeed;
	int m_iCountSamples[2];
	Array<float> m_fAnimRamp;
	StringArray m_sParameterName;
	String m_sConfigFileHash;
	
	//==============================================================================
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CAutomationBridge)
};

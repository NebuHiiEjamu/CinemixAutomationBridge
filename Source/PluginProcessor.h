/*
  ==============================================================================

	It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//! \enum MIDI_STATUS Channel Message Staus
enum MIDI_STATUS: uint8
{
	MIDI_NOTE_OFF = 0x80,
	MIDI_NOTE_ON = 0x90,
	MIDI_AFTER_TOUCH = 0xA0,
	MIDI_CONTROL_CHANGE = 0xB0,
	MIDI_PROGRAM_CHANGE = 0xC0,
	MIDI_CHANNEL_PRESSURE = 0xD0,
	MIDI_PITCH_BEND = 0xE0,
	MIDI_SYSEX = 0xF0,
	MIDI_SYSEX_CONT = 0xF7,
	MIDI_ACTIVE_SENSE = 0xFE,
	MIDI_STATUS_META = 0xFF
};

//! \enum MIDI_CONTROLLER Common controller numbers
enum MIDI_CONTROLLER: uint8
{
	MIDI_BANK_SELECT_MSB = 0,
	MIDI_BANK_SELECT_LSB = 32,
	MIDI_DAMPER = 64,
	MIDI_PORTAMENTO,
	MIDI_SOSTENUTO,
	MIDI_SOFT_PEDAL,
	MIDI_ALL_SOUND_OFF = 120,
	MIDI_RESET_ALL,
	MIDI_CONTROLLER_LOCAL,
	MIDI_ALL_NOTES_OFF,
	MIDI_OMNI_MODE_OFF,
	MIDI_OMNI_MODE_ON,
	MIDI_POLY_MODE,
	MIDI_POLY_MODE_ON
};

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
	void changeProgramName(int iIndex, const String& sNewName) override;

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
	
	void sendMidiEvent(uint8 uChannel, uint8 uCCNum, uint8 uValue);
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
	int m_iNumFaders;
	int m_iNumParams;
	int m_iNumChannelStrips;
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
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CCinemixAutomationBridge)
};

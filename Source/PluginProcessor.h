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
class AutomationBridge: public AudioProcessor
{
public:
	static constexpr float one127th = 0.00787401574803f;
	static constexpr float one255th = 0.00392156862745f;
	static constexpr float one16384th = 0.00006103515625f;
	static constexpr float randMaxRecip = 0.00003051757f;
	//==============================================================================
	AutomationBridge();	
	~AutomationBridge();

	//==============================================================================
	void prepareToPlay(double sampleRate, int samplesPerBlock) override;
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
	void setCurrentProgram(int index) override;
	const String getProgramName(int index) override;
	void changeProgramName(int index, const String &newName) override;

	//==============================================================================
	void getStateInformation(MemoryBlock &destData) override;
	void setStateInformation(const void *data, int sizeInBytes) override;
	
	void toggleTestMode();
	
	bool openMidiInPort(int port);
	bool openMidiOutPort(int port);
	
	//! Close MIDI Ports
	void closeAllMidiPorts();
	
	//! Open MIDI Ports
	void openAllMidiPorts();
	
	void setAllChannelsMode(int mode);
	void activateMixer();
	void deactivateMixer();
	void sendSnapshot();
	void resetAllMixer();
	void toggleAllMutes();
	bool saveConfig() const;
	void loadConfig();
	static String getConfigFilePath();
	static String getResFilePath();
	float randGen() const;
private:
	bool testMode;
	bool allMutesStatus;
	bool fullInit;
	int midiInPorts[2];
	int numMidiInPorts;
	int numMidiOutPorts;
	int midiOutPorts[2];
	const int numFaders;
	const int numParams;
	const int numChannelStrips;
	const OwnedArray<AudioProcessorParameter> &m_params;
	StringArray midiInPortName;
	StringArray midiOutPortName;
	Array<int> midiCtrlParams;
	Array<int> midiCtrlFaders;
	int faderModeLeft[48];
	int faderModeRight[48];
	int faderModeMaster;
	Array<int> midiChannel;
	Array<int> prevCCVal;
	int faderSpeed;
	int muteSpeed;
	int countSamples[2];
	Array<float> animRamp;
	StringArray parameterName;
	
	//==============================================================================
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AutomationBridge)
};

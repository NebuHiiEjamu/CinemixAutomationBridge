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

	This contains the framework code for a JUCE plugin processor.
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/**
*/
class AutomationBridge : public AudioProcessor
{
public:
	static constexpr float one127th = 0.00787401574803f;
	static constexpr float one255th = 0.00392156862745f;
	static constexpr float one16384th = 0.00006103515625f;
	static constexpr float randMaxRecip = 0.00003051757f;
	
	AutomationBridge();	
	~AutomationBridge();
	
	// known in VST2 as resume()
	void prepareToPlay(double sampleRate, int samplesPerBlock) override;
	
	// known in VST2 as suspend()
	void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
	bool isBusesLayoutSupported(const BusesLayout &layouts) const override;
#endif
	
	// known in VST2 as processReplacing()
	void processBlock(AudioBuffer<float>&, MidiBuffer&) override;

	AudioProcessorEditor* createEditor() override;
	bool hasEditor() const override;

	const String getName() const override;

	bool acceptsMidi() const override;
	bool producesMidi() const override;
	bool isMidiEffect() const override;
	
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
protected:
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
	const OwnedArray<AudioProcessorParameter> &params; // TODO: Do we need to hold this?
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
	
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AutomationBridge)
};

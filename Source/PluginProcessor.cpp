/*
  ==============================================================================

	This file was auto-generated!

	It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
	
//==============================================================================
AutomationBridge::AutomationBridge()
#ifndef JucePlugin_PreferredChannelConfigurations
	: AudioProcessor(BusesProperties()
#if !JucePlugin_IsMidiEffect
#if !JucePlugin_IsSynth
		.withInput("Input",  AudioChannelSet::stereo(), true)
#endif // JucePlugin_IsSynth
		.withOutput("Output", AudioChannelSet::stereo(), true)
#endif // JucePlugin_IsMidiEffect
	),
	testMode(false),
	fullInit(false)
#endif // JucePlugin_PreferredChannelConfigurations
{
	params = getParameters();
	numParams = params.size();
}

AutomationBridge::~AutomationBridge()
{
}

//==============================================================================
const String AutomationBridge::getName() const
{
	return JucePlugin_Name;
}

bool AutomationBridge::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
	return true;
#else
	return false;
#endif
}

bool AutomationBridge::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
	return true;
#else
	return false;
#endif
}

bool AutomationBridge::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
	return true;
#else
	return false;
#endif
}

double AutomationBridge::getTailLengthSeconds() const
{
	return 0.0;
}

int AutomationBridge::getNumPrograms()
{
	return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
				// so this should be at least 1, even if you're not really implementing programs.
}

int AutomationBridge::getCurrentProgram()
{
	return 0;
}

void AutomationBridge::setCurrentProgram(int index)
{
}

const String AutomationBridge::getProgramName(int index)
{
	return {};
}

void AutomationBridge::changeProgramName(int index, const String &newName)
{
}

//==============================================================================
void AutomationBridge::prepareToPlay(double sampleRate, int samplesPerBlock)
{
	// Use this method as the place to do any pre-playback
	// initialisation that you need..
}

void AutomationBridge::releaseResources()
{
	// When playback stops, you can use this as an opportunity to free up any
	// spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool AutomationBridge::isBusesLayoutSupported(const BusesLayout &layouts) const
{
#if JucePlugin_IsMidiEffect
	ignoreUnused(layouts);
	return true;
#else
	// This is the place where you check if the layout is supported.
	// In this template code we only support mono or stereo.
	if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
			&& layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
		return false;

	// This checks if the input layout matches the output layout
#if !JucePlugin_IsSynth
	if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
		return false;
#endif // JucePlugin_IsSynth
	return true;
#endif // JucePlugin_IsMidiEffect
}
#endif // JucePlugin_PreferredChannelConfigurations

void AutomationBridge::processBlock(AudioBuffer<float> &buffer, MidiBuffer &midiMessages)
{
	ignoreUnused(buffer);
	/*ScopedNoDenormals noDenormals;
	auto totalInputChannels  = getTotalNumInputChannels();
	auto totalOutputChannels = getTotalNumOutputChannels();

	// In case we have more outputs than inputs, this code clears any output
	// channels that didn't contain input data, (because these aren't
	// guaranteed to be empty - they may contain garbage).
	// This is here to avoid people getting screaming feedback
	// when they first compile a plugin, but obviously you don't need to keep
	// this code if your algorithm always overwrites all the output channels.
	for (auto i = totalInputChannels; i < totalOutputChannels; ++i)
		buffer.clear(i, 0, buffer.getNumSamples());

	// This is the place where you'd normally do the guts of your plugin's
	// audio processing...
	// Make sure to reset the state if your inner loop is processing
	// the samples and the outer loop is handling the channels.
	// Alternatively, you can process the samples with the channels
	// interleaved by keeping the same state.
	for (int channel = 0; channel < totalInputChannels; ++channel)
	{
		auto *channelData = buffer.getWritePointer(channel);

		// ..do something to the data...
	}*/
	
	MidiBuffer processedMidi;
	int time = 0;
	MidiMessage msg;
	
	for (MidiBuffer::Iterator i(midiMessages); i.getNextEvent(msg, time);)
	{
		const uint8 *midiData = msg.getRawData();
	
		// If a complete 3-byte MIDI message is received
		if (msg.getRawDataSize() == 3)
		{
			uint8 status = midiData[0] & 0xF0;
			uint8 channel = midiData[0] & 0xF; channel++;
			uint8 bytes[2] = { midiData[1] & 0x7F, midiData[2] & 0x7F };
			
			if (status == MIDI_CONTROL_CHANGE)
			{
				// Get Touch sensors and set the channels in WRITE mode automatically
				// Left part
				if (channel == 3 && bytes[0] > 63 && bytes[0] < 112)
				{
					// When SEL button is depressed...
					if (bytes[1] == 1)
					{
						// Rotate between modes: 0=ISO, 1=READ, 2=WRITE, 3=AUTO
						if (++faderModeLeft[bytes[0] - 64] > 3)
							faderModeLeft[bytes[0] - 64] = 0;
						processedMidi.addEvent(MidiMessage(channel, bytes[0], faderModeLeft[bytes[0] - 64]), time);
					}
					
					// In AUTO mode...
					if (faderModeLeft[bytes[0] - 64] == 3)
					{
						// Touch, set to W
						if (bytes[1] == 6)
							processedMidi.addEvent(MidiMessage(channel, bytes[0], 2), time);
						// Release, set to RW
						else if (bytes[1] == 5)
							processedMidi.addEvent(MidiMessage(channel, bytes[0], 3), time);
					}
				}
				
				// Right part
				if (channel == 4 && bytes[0] > 63 && bytes[0] < 112)
				{
					if (bytes[1] == 1)
					{
						// Rotate between modes: 0=ISO, 1=READ, 2=WRITE, 3=AUTO
						if (++faderModeRight[bytes[0] - 64] > 3)
							faderModeRight[bytes[0] - 64] = 0;
						processedMidi.addEvent(MidiMessage(channel, bytes[0], faderModeRight[bytes[0] - 64]), time);
					}
					
					// In AUTO mode...
					if (faderModeRight[bytes[0] - 64] == 3)
					{
						// Touch, set to W
						if (bytes[1] == 6)
							processedMidi.addEvent(MidiMessage(channel, bytes[0], 2), time);
						// Release, set to RW
						else if (bytes[1] == 5)
							processedMidi.addEvent(MidiMessage(channel, bytes[0], 3), time);
					}
				}
				
				// Master SEL R/W Button selects status for all channels
				if (channel == 5 && bytes[0] == 64 && bytes[1] == 1)
				{
					if (++faderModeMaster > 3) faderModeMaster = 0;
					setAllChannelsMode(faderModeMaster);
				}
				
				// Special check for AUX Mutes (could have found a more elegant way, but this one works!)
				if (channel == 5 && bytes[0] == 96)
				{
					switch (bytes[1])
					{
						default: ;
					}
				}
			}
		}
	}
	
	midiMessages.swapWith(processedMidi);
}

//==============================================================================
bool AutomationBridge::hasEditor() const
{
	return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* AutomationBridge::createEditor()
{
	return new AutomationBridgeEditor(*this);
}

//==============================================================================
void AutomationBridge::getStateInformation(MemoryBlock &destData)
{
	// You should use this method to store your parameters in the memory block.
	// You could do that either as raw data, or use the XML or ValueTree classes
	// as intermediaries to make it easy to save and load complex data.
}

void AutomationBridge::setStateInformation(const void *data, int sizeInBytes)
{
	// You should use this method to restore your parameters from this memory block,
	// whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
	return new AutomationBridge();
}

void AutomationBridge::activateMixer()
{
	sendMidiEvent(5, 127, 127);
	setAllChannelsMode(2); // Set all channels to Write Mode
	
	// Set Master Section to Write Mode (Master fader and Joysticks)
	sendMidiEvent(5, 64, 2);
	sendMidiEvent(
}

void AutomationBridge::toggleTestMode()
{
	if (testMode)
	{
		testMode = false;
		resetAllMixer();
		setAllChannelsMode(3);
	}
	else
	{
		resetAllMixer();
		setAllChannelsMode(1);

		// Calculate frame rates for faders and mutes animations separately
		// This is rather slow, but avoids flooding the  MIDI buffers
		faderSpeed = static_cast<int>(getSampleRate() / 25);
		muteSpeed = static_cast<int>(getSampleRate() / 10);
		countSamples[0] = countSamples[1] = 0;
		
		float scaleP = 2.0f / numFaders;
		
		// Starting point of all faders
		for (int i = 0; i < numFaders; i++)
			animRamp[i] = -2.0f + scaleP * i;
		
		testMode = true;
	}
}

bool AutomationBridge::saveConfig() const
{
	// No MIDI ports found? Don't save!
	if (numMidiInPorts + numMidiOutPorts < 2)
		return false;
	
	String configFilePath = getConfigFilePath();
	XmlElement config("AutomationBridge");
	XmlElement *inPortListXml = new XmlElement("InPorts");
	XmlElement *outPortListXml = new XmlElement("OutPorts");
	
	config.addChildElement(inPortListXml);
	config.addChildElement(outPortListXml);
	
	for (int i = 0; i < numMidiInPorts; i++)
	{
		XmlElement *inPortXml = new XmlElement("port");
		inPortXml->setAttribute("name", midiInPortName[i]);
		inPortListXml->addChildElement(inPortXml);
	}
	
	for (int i = 0; i < numMidiOutPorts; i++)
	{
		XmlElement *outPortXml = new XmlElement("port");
		outPortXml->setAttribute("name", m_sMidiOutPortName[i]);
		pOutPortListXml->addChildElement(outPortXml);
	}
	
	return config.writeToFile(File(configFilePath), String());
}

void AutomationBridge::loadConfig()
{
	String configFilePath = getConfigFilePath();
	XmlDocument configFile(File(configFilePath));
	XmlElement *config = configFile.getDocumentElement();
	
	if (config)
	{
		
	}
}

String AutomationBridge::getConfigFilePath()
{
	String configFilePath;
#ifdef _WIN32
	String appDataDefaultPath = "C:\\Users\\";
	appDataDefaultPath << SystemStats::getLogonName() << "\\AppData\\Roaming";
	configFilePath = SystemStats::getEnvironmentVariable("APPDATA", appDataPath);
#else
	configFilePath = SystemStats::getEnvironmentVariable("HOME", "~");
#endif

#ifdef __APPLE__
	configFilePath += "/Library/Preferences";
#endif

#if !defined(_WIN32) && !defined(__APPLE__)
	configFilePath += ".config";
#endif

	configFilePath << File::getSeparatorChar() << "AutomationBridge.xml"

	return configFilePath;
}

static String AutomationBridge::getResFilePath()
{
	return String("data") + File::getSeparatorChar();
}

float AutomationBridge::randGen() const
{
	return Random::getSystemRandom().nextFloat() * randMaxRecip;
}

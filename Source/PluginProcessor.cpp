/*
  ==============================================================================

	This file was auto-generated!

	It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
	
//==============================================================================
CAutomationBridge::CAutomationBridge()
#ifndef JucePlugin_PreferredChannelConfigurations
	: AudioProcessor(BusesProperties()
#if !JucePlugin_IsMidiEffect
#if !JucePlugin_IsSynth
		.withInput("Input",  AudioChannelSet::stereo(), true)
#endif // JucePlugin_IsSynth
		.withOutput("Output", AudioChannelSet::stereo(), true)
#endif // JucePlugin_IsMidiEffect
	),
	m_bTestMode(false),
	m_bFullInit(false)
#endif // JucePlugin_PreferredChannelConfigurations
{
	m_params = getParameters();
	m_iNumParams = m_params.size();
}

CAutomationBridge::~CAutomationBridge()
{
}

//==============================================================================
const String CAutomationBridge::getName() const
{
	return JucePlugin_Name;
}

bool CAutomationBridge::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
	return true;
#else
	return false;
#endif
}

bool CAutomationBridge::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
	return true;
#else
	return false;
#endif
}

bool CAutomationBridge::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
	return true;
#else
	return false;
#endif
}

double CAutomationBridge::getTailLengthSeconds() const
{
	return 0.0;
}

int CAutomationBridge::getNumPrograms()
{
	return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
				// so this should be at least 1, even if you're not really implementing programs.
}

int CAutomationBridge::getCurrentProgram()
{
	return 0;
}

void CAutomationBridge::setCurrentProgram(int iIndex)
{
}

const String CAutomationBridge::getProgramName(int iIndex)
{
	return {};
}

void CAutomationBridge::changeProgramName(int iIndex, const String &sNewName)
{
}

//==============================================================================
void CAutomationBridge::prepareToPlay(double fSampleRate, int iSamplesPerBlock)
{
	// Use this method as the place to do any pre-playback
	// initialisation that you need..
}

void CAutomationBridge::releaseResources()
{
	// When playback stops, you can use this as an opportunity to free up any
	// spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool CAutomationBridge::isBusesLayoutSupported(const BusesLayout &layouts) const
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

void CAutomationBridge::processBlock(AudioBuffer<float> &buffer, MidiBuffer &midiMessages)
{
	ignoreUnused(buffer);
	/*ScopedNoDenormals noDenormals;
	auto iTotalInputChannels  = getTotalNumInputChannels();
	auto iTotalOutputChannels = getTotalNumOutputChannels();

	// In case we have more outputs than inputs, this code clears any output
	// channels that didn't contain input data, (because these aren't
	// guaranteed to be empty - they may contain garbage).
	// This is here to avoid people getting screaming feedback
	// when they first compile a plugin, but obviously you don't need to keep
	// this code if your algorithm always overwrites all the output channels.
	for (auto i = iTotalInputChannels; i < iTotalOutputChannels; ++i)
		buffer.clear(i, 0, buffer.getNumSamples());

	// This is the place where you'd normally do the guts of your plugin's
	// audio processing...
	// Make sure to reset the state if your inner loop is processing
	// the samples and the outer loop is handling the channels.
	// Alternatively, you can process the samples with the channels
	// interleaved by keeping the same state.
	for (int iChannel = 0; iChannel < iTotalInputChannels; ++iChannel)
	{
		auto *pChannelData = buffer.getWritePointer(channel);

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
			uint8 uStatus = midiData[0] & 0xF0;
			uint8 uChannel = midiData[0] & 0xF; uChannel++;
			uint8 uBytes[2] = { midiData[1] & 0x7F, midiData[2] & 0x7F };
			
			if (uStatus == MIDI_CONTROL_CHANGE)
			{
				// Get Touch sensors and set the channels in WRITE mode automatically
				// Left part
				if (uChannel == 3 && uBytes[0] > 63 && uBytes[0] < 112)
				{
					// When SEL button is depressed...
					if (uBytes[1] == 1)
					{
						// Rotate between modes: 0=ISO, 1=READ, 2=WRITE, 3=AUTO
						if (++m_iFaderModeLeft[uBytes[0] - 64] > 3)
							m_iFaderModeLeft[uBytes[0] - 64] = 0;
						processedMidi.addEvent(MidiMessage(uChannel, uBytes[0], m_iFaderModeLeft[uBytes[0] - 64]), time);
					}
					
					// In AUTO mode...
					if (m_iFaderModeLeft[uBytes[0] - 64] == 3)
					{
						// Touch, set to W
						if (uBytes[1] == 6)
							processedMidi.addEvent(MidiMessage(uChannel, uBytes[0], 2), time);
						// Release, set to RW
						else if (uBytes[1] == 5)
							processedMidi.addEvent(MidiMessage(uChannel, uBytes[0], 3), time);
					}
				}
				
				// Right part
				if (uChannel == 4 && uBytes[0] > 63 && uBytes[0] < 112)
				{
					if (uBytes[1] == 1)
					{
						// Rotate between modes: 0=ISO, 1=READ, 2=WRITE, 3=AUTO
						if (++m_iFaderModeRight[uBytes[0] - 64] > 3)
							m_iFaderModeRight[uBytes[0] - 64] = 0;
						processedMidi.addEvent(MidiMessage(uChannel, uBytes[0], m_iFaderModeRight[uBytes[0] - 64]), time);
					}
					
					// In AUTO mode...
					if (m_iFaderModeRight[uBytes[0] - 64] == 3)
					{
						// Touch, set to W
						if (uBytes[1] == 6)
							processedMidi.addEvent(MidiMessage(uChannel, uBytes[0], 2), time);
						// Release, set to RW
						else if (uBytes[1] == 5)
							processedMidi.addEvent(MidiMessage(uChannel, uBytes[0], 3), time);
					}
				}
				
				// Master SEL R/W Button selects status for all channels
				if (uChannel == 5 && uBytes[0] == 64 && uBytes[1] == 1)
				{
					if (++m_iFaderModeMaster > 3) m_iFaderModeMaster = 0;
					setAllChannelsMode(m_iFaderModeMaster);
				}
				
				// Special check for AUX Mutes (could have found a more elegant way, but this one works!)
				if (uChannel == 5 && uBytes[0] == 96)
				{
					switch (uBytes[1])
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
bool CAutomationBridge::hasEditor() const
{
	return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* CAutomationBridge::createEditor()
{
	return new CAutomationBridgeEditor(*this);
}

//==============================================================================
void CAutomationBridge::getStateInformation(MemoryBlock &destData)
{
	// You should use this method to store your parameters in the memory block.
	// You could do that either as raw data, or use the XML or ValueTree classes
	// as intermediaries to make it easy to save and load complex data.
}

void CAutomationBridge::setStateInformation(const void *pData, int iSizeInBytes)
{
	// You should use this method to restore your parameters from this memory block,
	// whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
	return new CAutomationBridge();
}

void CAutomationBridge::activateMixer()
{
	sendMidiEvent(5, 127, 127);
	setAllChannelsMode(2); // Set all channels to Write Mode
	
	// Set Master Section to Write Mode (Master fader and Joysticks)
	sendMidiEvent(5, 64, 2);
	sendMidiEvent(
}

void CAutomationBridge::toggleTestMode()
{
	if (m_bTestMode)
	{
		m_bTestMode = false;
		resetAllMixer();
		setAllChannelsMode(3);
	}
	else
	{
		resetAllMixer();
		setAllChannelsMode(1);

		// Calculate frame rates for faders and mutes animations separately
		// This is rather slow, but avoids flooding the  MIDI buffers
		m_iFaderSpeed = static_cast<int>(getSampleRate() / 25);
		m_iMuteSpeed = static_cast<int>(getSampleRate() / 10);
		m_iCountSamples[0] = m_iCountSamples[1] = 0;
		
		float fScaleP = 2.0f / m_iNumFaders;
		
		// Starting point of all faders
		for (int i = 0; i < m_iNumFaders; i++)
			m_fAnimRamp[i] = -2.0f + fScaleP * i;
		
		m_bTestMode = true;
	}
}

bool CAutomationBridge::saveConfig() const
{
	// No MIDI ports found? Don't save!
	if (m_iNumMidiInPorts + m_iNumMidiOutPorts < 2)
		return false;
	
	String sConfigFilePath = getConfigFilePath();
	XmlElement config("AutomationBridge");
	XmlElement *pInPortListXml = new XmlElement("InPorts");
	XmlElement *pOutPortListXml = new XmlElement("OutPorts");
	
	config.addChildElement(pInPortListXml);
	config.addChildElement(pOutPortListXml);
	
	for (int i = 0; i < m_iNumMidiInPorts; i++)
	{
		XmlElement *pInPortXml = new XmlElement("port");
		pInPortXml->setAttribute("name", m_sMidiInPortName[i]);
		pInPortListXml->addChildElement(pInPortXml);
	}
	
	for (int i = 0; i < m_iNumMidiOutPorts; i++)
	{
		XmlElement *pOutPortXml = new XmlElement("port");
		pOutPortXml->setAttribute("name", m_sMidiOutPortName[i]);
		pOutPortListXml->addChildElement(pOutPortXml);
	}
	
	return config.writeToFile(File(sConfigFilePath), String());
}

void CAutomationBridge::loadConfig()
{
	String sConfigFilePath = getConfigFilePath();
	XmlDocument configFile(File(sConfigFilePath));
	XmlElement *pConfig = configFile.getDocumentElement();
	
	if (pConfig)
	{
		
	}
}

String CAutomationBridge::getConfigFilePath()
{
	String sConfigFilePath;
#ifdef _WIN32
	String sAppDataDefaultPath = "C:\\Users\\";
	sAppDataDefaultPath << SystemStats::getLogonName() << "\\AppData\\Roaming";
	sConfigFilePath = SystemStats::getEnvironmentVariable("APPDATA", sAppDataPath);
#else
	sConfigFilePath = SystemStats::getEnvironmentVariable("HOME", "~");
#endif

#ifdef __APPLE__
	sConfigFilePath += "/Library/Preferences";
#endif

#if !defined(_WIN32) && !defined(__APPLE__)
	sConfigFilePath += ".config";
#endif

	sConfigFilePath << File::getSeparatorChar() << "AutomationBridge.xml"

	return sConfigFilePath;
}

static String CAutomationBridge::getResFilePath()
{
	return String("data") + File::getSeparatorChar();
}

float CAutomationBridge::randGen() const
{
	return Random::getSystemRandom().nextFloat() * RAND_MAX_RECIP;
}

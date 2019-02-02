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

#include "PluginProcessor.h"
#include "PluginEditor.h"
	
//==============================================================================
AutomationBridge::AutomationBridge():
	testMode(false),
	fullInit(false) // TODO: needed?
{
	params = getParameters();
	numParams = params.size();
}

AutomationBridge::~AutomationBridge()
{
}

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

void AutomationBridge::prepareToPlay(double sampleRate, int samplesPerBlock)
{
}

void AutomationBridge::releaseResources()
{
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
	{
		return false;
	}

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
	
	MidiBuffer processedMidi;
	int time = 0;
	MidiMessage msg;
	
	for (MidiBuffer::Iterator iter(midiMessages); i.getNextEvent(msg, time);)
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
						{
							faderModeLeft[bytes[0] - 64] = 0;
						}
						processedMidi.addEvent(MidiMessage(channel, bytes[0], faderModeLeft[bytes[0] - 64]), time);
					}
					
					// In AUTO mode...
					if (faderModeLeft[bytes[0] - 64] == 3)
					{
						// Touch, set to W
						if (bytes[1] == 6)
						{
							processedMidi.addEvent(MidiMessage(channel, bytes[0], 2), time);
						}
						// Release, set to RW
						else if (bytes[1] == 5)
						{
							processedMidi.addEvent(MidiMessage(channel, bytes[0], 3), time);
						}
					}
				}
				
				// Right part
				if (channel == 4 && bytes[0] > 63 && bytes[0] < 112)
				{
					if (bytes[1] == 1)
					{
						// Rotate between modes: 0=ISO, 1=READ, 2=WRITE, 3=AUTO
						if (++faderModeRight[bytes[0] - 64] > 3)
						{
							faderModeRight[bytes[0] - 64] = 0;
						}
						processedMidi.addEvent(MidiMessage(channel, bytes[0], faderModeRight[bytes[0] - 64]), time);
					}
					
					// In AUTO mode...
					if (faderModeRight[bytes[0] - 64] == 3)
					{
						// Touch, set to W
						if (bytes[1] == 6)
						{
							processedMidi.addEvent(MidiMessage(channel, bytes[0], 2), time);
						}
						// Release, set to RW
						else if (bytes[1] == 5)
						{
							processedMidi.addEvent(MidiMessage(channel, bytes[0], 3), time);
						}
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
		
		float scaleP = 2.f / numFaders;
		
		// Starting point of all faders
		for (int i = 0; i < numFaders; i++)
			animRamp[i] = -2.f + scaleP * i;
		
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
		outPortXml->setAttribute("name", midiOutPortName[i]);
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

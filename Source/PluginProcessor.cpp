/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street - Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include "PluginProcessor.h"
#include "PluginEditor.h"


AutomationBridge::AutomationBridge()
    : inDeviceData (MidiInput::getAvailableDevices()),
      outDeviceData (MidiOutput::getAvailableDevices()),
      faderSpeed (0),
      muteSpeed (0),
      faders (48),
      channels (1),
      testModeOn (false)
{
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

void AutomationBridge::setCurrentProgram (int index)
{
    ignoreUnused (index);
}

const String AutomationBridge::getProgramName (int index)
{
    ignoreUnused (index);
    return {};
}

void AutomationBridge::changeProgramName (int index, const String& newName)
{
    ignoreUnused (index);
    ignoreUnused (newName);
}

void AutomationBridge::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..

    ignoreUnused (sampleRate);
    ignoreUnused (samplesPerBlock);
}

void AutomationBridge::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool AutomationBridge::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void AutomationBridge::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    buffer.clear();

    MidiBuffer processedMidi;
    int time;
    MidiMessage m;

    for (MidiBuffer::Iterator i (midiMessages); i.getNextEvent (m, time);)
    {
        if (m.getRawDataSize() == 3)
        {
            const uint8* data = m.getRawData();
            int channel = (data[0] & 0xF) + 1;
            int byte1 = data[1] & 0x7F;
            int byte2 = data[2] & 0x7F;

            //if (m.isController()) midiControlChanged (channel, byte1, byte2);
        }

        processedMidi.addEvent (m, time);
    }

    midiMessages.swapWith (processedMidi);
}

//==============================================================================
bool AutomationBridge::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* AutomationBridge::createEditor()
{
    return new AutomationBridgeEditor (*this);
}

//==============================================================================
void AutomationBridge::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.

    ignoreUnused (destData);
}

void AutomationBridge::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.

    ignoreUnused (data);
    ignoreUnused (sizeInBytes);
}

MidiDeviceInfo AutomationBridge::getActiveInput (int i) const
{
	return inDeviceData[inputsOn[i]];
}

MidiDeviceInfo AutomationBridge::getActiveOutput (int i) const
{
	return outDeviceData[outputsOn[i]];
}

void AutomationBridge::sendMidiCC (int channel, int ccNum, int value, MidiOutput* output = nullptr)
{
    if (!output) output = outDevices[0].get();
    output->sendMessageNow (MidiMessage (0xAF + channel, ccNum, value));
}

void AutomationBridge::refresh()
{
    inDevices.clear();
    outDevices.clear();

    for (int i : inputsOn)
        inDevices.push_back (MidiInput::openDevice (inDeviceData[i].identifier, nullptr));
    
    for (int i : outputsOn)
        outDevices.push_back (MidiOutput::openDevice (outDeviceData[i].identifier));
}

/*void AutomationBridge::midiControlChanged (int channel, int byte1, int byte2)
{
    // Get Touch sensors and set the channels in WRITE mode automatically
    if (channel == 3 && byte1 > 63 && byte1 < 112) // Left part
    {
        if (byte2 == 1) // When SEL button is depressed...
    }
}*/

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AutomationBridge();
}

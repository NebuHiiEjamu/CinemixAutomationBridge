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

#include <vector>

static void callback(double dtime, std::vector<uint8_t> *pMsg, void *data)
{
}

//==============================================================================
AutomationBridgeProcessor::AutomationBridgeProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    for (int i = 0; i < 74; i++) muteOn[i] = false;
    
    for (int i = 0; i < midiIn.size(); i++)
    {
        midiIn[i].reset(new RtMidiIn());
        midiIn[i]->setCallback(&callback);
        midiIn[i]->ignoreTypes(false, false, false);
    }
    
    for (int i = 0; i < midiOut.size(); i++)
    {
        midiOut[i].reset(new RtMidiOut());
    }
    
    midiIn[0]->openPort(2);
    midiOut[0]->openPort(2);
    midiIn[1]->openPort(3);
    midiOut[1]->openPort(3);
}

AutomationBridgeProcessor::~AutomationBridgeProcessor()
{
}

//==============================================================================
const String AutomationBridgeProcessor::getName() const
{
    return JucePlugin_Name;
}

bool AutomationBridgeProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool AutomationBridgeProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool AutomationBridgeProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double AutomationBridgeProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int AutomationBridgeProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int AutomationBridgeProcessor::getCurrentProgram()
{
    return 0;
}

void AutomationBridgeProcessor::setCurrentProgram (int index)
{
}

const String AutomationBridgeProcessor::getProgramName (int index)
{
    return {};
}

void AutomationBridgeProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void AutomationBridgeProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void AutomationBridgeProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool AutomationBridgeProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void AutomationBridgeProcessor::processBlock (AudioBuffer<float>&, MidiBuffer& midiMessages)
{
    /*auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();*/
}

//==============================================================================
bool AutomationBridgeProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* AutomationBridgeProcessor::createEditor()
{
    return new AutomationBridgeEditor (*this);
}

//==============================================================================
void AutomationBridgeProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void AutomationBridgeProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AutomationBridgeProcessor();
}

void AutomationBridgeProcessor::sendMidiCC(uint8_t channel, uint8_t ccNum, uint8_t value, RtMidiOut *port)
{
    if (!port) port = midiOut[0].get();
    std::vector<uint8_t> msg { static_cast<uint8_t>(175 + channel), ccNum, value };
    port->sendMessage(&msg);
}

void AutomationBridgeProcessor::setAllChannelsMode(int mode)
{
    for (int i = 0; i < 74; i++)
    {
        if (i < 48)
        {
            sendMidiCC(3, 64 + i, mode);
            sendMidiCC(4, 64 + i, mode);
        }
        else
        {
            sendMidiCC(3, 64 + i, mode, midiOut[1].get());
            sendMidiCC(4, 64 + i, mode, midiOut[1].get());
        }
    }
}

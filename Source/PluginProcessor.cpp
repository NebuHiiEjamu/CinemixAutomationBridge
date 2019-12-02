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
      fullInit (false),
      testModeOn (false),
      allMutesStatus (false)
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
    // NB: some hosts don't cope very well if you tell them there are 0 programs,
    // so this should be at least 1, even if you're not really implementing programs.
    return 1;
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

            if (m.isController()) midiControlChanged (channel, byte1, byte2);
        }

        processedMidi.addEvent (m, time);
    }

    midiMessages.swapWith (processedMidi);
}

//==============================================================================
bool AutomationBridge::hasEditor() const
{
    return true;
}

AudioProcessorEditor* AutomationBridge::createEditor()
{
    AutomationBridgeEditor* editor = new AutomationBridgeEditor (*this);
    auxStart = faders * 2;

    for (int i = 0; i < auxStart; i++)
    {
        AudioParameterFloat* param = new AudioParameterFloat (String ("fader_") +
            String (i + 1), String ("Fader ") + String (i + 1), 0.0f, 1.0f, 0.0f);
        params.add (param);
    }

    for (int i = 0; i < 10; i++)
    {
        AudioParameterFloat* param = new AudioParameterFloat (String ("auxMute_") +
            String (i + 1), String ("Aux Mute ") + String (i + 1), 0.0f, 1.0f, 0.0f);
        params.add (param);
    }

    for (int i = 0; i < 7; i++)
    {
        AudioParameterFloat* param = new AudioParameterFloat (String ("master_") +
            String (i + 1), String ("Master ") + String (i + 1), 0.0f, 1.0f, 0.0f);
        params.add (param);
    }

    for (AudioParameterFloat* param : param) addParameter (param);

    midiControllers[0].ensureStorageAllocated (params.size());
    midiControllers[1].ensureStorageAllocated (faders);
    midiChannels.ensureStorageAllocated (params.size());
    faderModeLeft.resize (getTotalNumInputChannels());
    faderModeRight.resize (getTotalNumInputChannels());
    prevCCVals.ensureStorageAllocated (params.size());
    animRamp.ensureStorageAllocated (faders);
    
    fullInit = true;
    return editor;
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

void AutomationBridge::setAllChannelsMode (int mode)
{
    for (int i = 0; i < getTotalNumInputChannels(); i++)
    {
        sendMidiCC (3, 64 + i, mode);
        sendMidiCC (4, 64 + i, mode);
        faderModeLeft[i] = mode;
        faderModeRight[i] = mode;
    }
}

void AutomationBridge::sendMidiData (int index, float value)
{
    int channel = midiChannels[index];
    int control = midiControllers[0][index];
    int val = roundToInt (value) * 127;

    // Avoid sending duplicated values for the same parameter
    if (val != prevCCVals[index])
    {
        // Channel Strips
        if (index < faders * 2)
        {
            // Faders
            if (index < faders) sendMidiCC (channel, control, val,
                index < getTotalNumInputChannels() ? outDevices[0].get() : outDevices[1].get());
            // Mutes
            else sendMidiCC (channel, control, value ? 3 : 2,
                index < 120 ? outDevices[0].get() : outDevices[1].get());
        }
        // Master Section and other
        else
        {
            if (index == auxStart) // AUX 1
                sendMidiCC (channel, control, value ? 3 : 2, outDevices[1].get());
            else if (index == auxStart + 1) // AUX 2
                sendMidiCC (channel, control, value ? 5 : 4, outDevices[1].get());
            else if (index ==  auxStart + 2) // AUX 3
                sendMidiCC (channel, control, value ? 7 : 6, outDevices[1].get());
            else if (index ==  auxStart + 3) // AUX 4
                sendMidiCC (channel, control, value ? 9 : 8, outDevices[1].get());
            else if (index ==  auxStart + 4) // AUX 5
                sendMidiCC (channel, control, value ? 11 : 10, outDevices[1].get());
            else if (index ==  auxStart + 5) // AUX 6
                sendMidiCC (channel, control, value ? 13 : 12, outDevices[1].get());
            else if (index ==  auxStart + 6) // AUX 7
                sendMidiCC (channel, control, value ? 15 : 14, outDevices[1].get());
            else if (index ==  auxStart + 7) // AUX 8
                sendMidiCC (channel, control, value ? 17 : 16, outDevices[1].get());
            else if (index ==  auxStart + 8) // AUX 9
                sendMidiCC (channel, control, value ? 19 : 18, outDevices[1].get());
            else if (index ==  auxStart + 9) // AUX 10
                sendMidiCC (channel, control, value ? 21 : 20, outDevices[1].get());
            
            else if (index ==  auxStart + 12) // Joy 1 Mute
                sendMidiCC (channel, control, value ? 3 : 2, outDevices[1].get());
            else if (index ==  auxStart + 15) // Joy 2 Mute
                sendMidiCC (channel, control, value ? 3 : 2, outDevices[1].get());
            else
                sendMidiCC (channel, control, val, outDevices[1].get());
        }

        prevCCVals.set (index, val);
    }
}

void AutomationBridge::midiControlChanged (int channel, int byte1, int byte2)
{
    // Get Touch sensors and set the channels in WRITE mode automatically
    if (channel == 3 && byte1 > 63 && byte1 < 112) // Left part
    {
        if (byte2 == 1) // When SEL button is depressed...
        {
            // Rotate between modes: 0=ISO, 1=READ, 2=WRITE, 3=AUTO
            if (++faderModeLeft[byte1 - 64] > 3) faderModeLeft[byte1 - 64] = 0;
            sendMidiCC (channel, byte1, faderModeLeft[byte1 - 64]);
        }

        if (faderModeLeft[byte1 - 64] == 3) // In AUTO mode...
        {
            if (byte2 == 6) sendMidiCC (channel, byte1, 2); // Touch, set to W
            else if (byte2 == 5) sendMidiCC (channel, byte1, 3); // Release, set to RW
        }
    }

    if (channel == 4 && byte1 > 63 && byte1 < 112) // Right part
    {
        if (byte2 == 1) // When SEL button is depressed...
        {
            // Rotate between modes: 0=ISO, 1=READ, 2=WRITE, 3=AUTO
            if (++faderModeRight[byte1 - 64] > 3) faderModeRight[byte1 - 64] = 0;
            sendMidiCC (channel, byte1, faderModeRight[byte1 - 64]);
        }
        
        if (faderModeRight[byte1 - 64] == 3) // In AUTO mode...
        {
            if (byte2 == 6) sendMidiCC (channel, byte1, 2); // Touch, set to W
            else if (byte2 == 5) sendMidiCC (channel, byte1, 3); // Release, set to RW
        }
    }

    // Master SEL R/W Button selects status for all channels
    if (channel == 5 && byte1 == 64 && byte2 == 1)
    {
        if (++faderModeMaster > 3) faderModeMaster = 0;
        setAllChannelsMode (faderModeMaster);
    }

    // Special check for AUX Mutes (could have found a more elegant way, but this one works!)
    if (channel == 5 && byte1 == 96)
    {
        switch (byte2)
        {
            // AUX 1
            case 2: (*params[auxStart]) = 0.0f; break;
            case 3: (*params[auxStart]) = 1.0f; break;

            // AUX 2
            case 4: (*params[auxStart + 1]) = 0.0f; break;
            case 5: (*params[auxStart + 1]) = 1.0f; break;

            // AUX 3
            case 6: (*params[auxStart + 2]) = 0.0f; break;
            case 7: (*params[auxStart + 2]) = 1.0f; break;

            // AUX 4
            case 8: (*params[auxStart + 3]) = 0.0f; break;
            case 9: (*params[auxStart + 3]) = 1.0f; break;

            // AUX 5
            case 10: (*params[auxStart + 4]) = 0.0f; break;
            case 11: (*params[auxStart + 4]) = 1.0f; break;

            // AUX 6
            case 12: (*params[auxStart + 5]) = 0.0f; break;
            case 13: (*params[auxStart + 5]) = 1.0f; break;

            // AUX 7
            case 14: (*params[auxStart + 6]) = 0.0f; break;
            case 15: (*params[auxStart + 6]) = 1.0f; break;

            // AUX 8
            case 16: (*params[auxStart + 7]) = 0.0f; break;
            case 17: (*params[auxStart + 7]) = 1.0f; break;

            // AUX 9
            case 18: (*params[auxStart + 8]) = 0.0f; break;
            case 19: (*params[auxStart + 8]) = 1.0f; break;

            // AUX 10
            case 20: (*params[auxStart + 9]) = 0.0f; break;
            case 21: (*params[auxStart + 9]) = 1.0f; break;

            default: ;
        }
    }

    // Move screen controls and automation parameters
    for (int p = 0; p < params.size(); p++)
    {
        if (midiChannels[p] == channel && (midiControllers[0][p] == byte1 ||
            midiControllers[1][p] == byte1))
        {
            // Channel Strips
            if (p < auxStart)
            {
                // Faders
                if (p < faders) (*params[p]) = byte2 * one127th;
                // Mutes
                else (*params[p - faders]) = byte2 == 3 ? 1.0f : 0.0f;
            }

            // Master Section and other
            else
            {
                // Joy Mutes
                if (p == auxStart + 12 || p == auxStart + 15)
                    (*params[p]) = byte2 == 3 ? 1.0f : 0.0f;
                
                // Joysticks 1&2 XY and Master fader
                if (p == auxStart + 10 || p == auxStart + 11 || p == auxStart + 13 ||
                    p == auxStart + 14 || p == auxStart + 16)
                    (*params[p]) = byte2 * one127th;
            }

            break; // stop for cycle
        }
    }

    // Special: second CC for Master fader
    if (channel == 5 && byte1 == 1) *(params.getLast()) = byte2 * one127th;
}

void AutomationBridge::sendSnapshot()
{
    for (int i = 0; i < params.size(); i++)
    {
        prevCCVals.set (i, -1);
        //params.set (i, (*params[i])); // ???
    }
}

void AutomationBridge::resetAllMixer()
{
    // Set all to AUTO mode
    for (int i = 0; i < getTotalNumInputChannels(); i++)
    {
        sendMidiCC (3, 64 + i, 3);
        sendMidiCC (4, 64 + i, 3);
        faderModeLeft[i] = 3;
        faderModeRight[i] = 3;
    }

    // Master Section (Master fader and Joysticks) to AUTO
    sendMidiCC (5, 64, 3);
    sendMidiCC (4, 88, 3);
    sendMidiCC (4, 90, 3);

    // All parameters to zero...
    for (AudioParameterFloat* param : params) *param = 0;
    
    // ...except the Master Fader
    params.getLast()->operator= (1.0f);

    sendSnapshot();
    allMutesStatus = false;
}

void AutomationBridge::testMode()
{
    if (testModeOn)
    {
        testModeOn = false;
        resetAllMixer();
        setAllChannelsMode (3);
    }
    else
    {
        resetAllMixer();
        setAllChannelsMode (1);

        // Calculate frame rates for faders and mutes animations separately
        // This is rather slow, but avoids flooding the Cinemix MIDI buffers
        faderSpeed = roundToInt (getSampleRate() / 25.0);
        muteSpeed = roundToInt (getSampleRate() / 10.0);

        float scaleP = 2.0f / faders;
        // Starting point of all faders
        for (int i = 0; i < faders; i++) animRamp.set (i, -2.0f + scaleP * i);

        testModeOn = true;
    }
    
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AutomationBridge();
}

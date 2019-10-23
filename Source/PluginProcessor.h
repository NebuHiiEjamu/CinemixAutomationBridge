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

#pragma once

#include <memory>
#include <vector>
#include "../JuceLibraryCode/JuceHeader.h"

/**
*/
class AutomationBridge  : public AudioProcessor
{
public:
    AutomationBridge();
    ~AutomationBridge();

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;
    const String getName() const override;
    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    MidiDeviceInfo getActiveInput (int) const;
    MidiDeviceInfo getActiveOutput (int) const;
    void refresh();
    void sendMidiCC (int, int, int, MidiOutput*);
    //void setAllChannelsMode (int);

public:
    Array<MidiDeviceInfo> inDeviceData;
    Array<MidiDeviceInfo> outDeviceData;
    std::vector<std::unique_ptr<MidiInput>> inDevices;
    std::vector<std::unique_ptr<MidiOutput>> outDevices;
    SortedSet<int> inputsOn;
	SortedSet<int> outputsOn;
    int faderSpeed;
    int muteSpeed;
    int faders;
    int channels;
    bool testModeOn;

private:
    //void midiControlChanged (int, int, int);

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AutomationBridge)
};

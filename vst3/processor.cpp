#include "processor.h"

#include <RtMidi.h>

PlugProcessor::PlugProcessor():
	mRandGen(mRandDev),
	mRandDis(0.f, kRandMaxRecip),
	mTestMode(0),
	mRtMidiPortsOpen(0),
	mAllMutesStatus(0)
{
}

void PlugProcessor::toggleTestMode()
{
	if (!mTestMode)
	{
		mFaderSpeed = static_cast<int>(processSetup.sampleRate / 25);
		mMuteSpeed = static_cast<int>(processSetup.sampleRate / 10);
		for (int &i : mCountSamples) i = 0;

		float scaleP = 2.f / mNumFaders;
		for (int i = 0; i < mNumFaders; i++)
			mAnimRamp[i] = -2.f + scaleP * i;
		
		mTestMode = 1;
	}
	else
	{
		mTestMode = 0;
	}
	
}

void PlugProcessor::rtmidiSendEvent(uint8 b1, uint8 b2, uint8 b3, uint8 port)
{
	if (!mRtMidiPortsOpen) return;

	std::vector<uint8> midiMessage = { b1, b2, b3 };

	if (!port)
	{
		for (RtMidiOut *midiOut : mMidiOut)
			midiOut->sendMessage(&midiMessage);
	}
	else
		mMidiOut[port-1]->sendMessage(&midiMessage);
}

void PlugProcessor::rtmidiSendByte(uint8 b)
{
	if (!mRtMidiPortsOpen) return;

	std::vector<uint8> midiMessage = { b };

	for (RtMidiOut *midiOut : mMidiOut)
	{
		try
		{
			midiOut->sendMessage(&midiMessage);
		}
		catch (RtMidiError &err)
		{
		}
	}
}

static void PlugProcessor::processMidiInput(double deltaTime, std::vector<uint8> *message, void *userData)
{
	std::vector<uint8> &midiData = *message;

	if (midiData.size() == 3)
	{
		uint8 status = midiData[0] & 0xF0;
		uint8 channel = (midiData[0] & 0x0F) + 1;
		std::array<uint8, 2> bytes = { midiData[1] & 0x7F, midiData[2] & 0x7F };

		if (status == kStatusControlChange)

	}
}

void PlugProcessor::setAllChannelsMode(int mode)
{
	int i = 0;

	for (int fml : mFaderModeLeft)
	{
		sendMidiCC(3, 64 + i, mode);
		sendMidiCC(4, 64 + i, mode);
		mFaderModeLeft[i] = mode;
		mFaderModeRight[i++] = mode;
	}
}

void PlugProcessor::activateMixer()
{
	sendMidiCC(5, 127, 127);

	setAllChannelsMode(2); // write mode

	// Set Master Section to Write Mode (Master fader and Joysticks)
	sendMidiCC(5, 64, 2);
	sendMidiCC(4, 88, 2);
	sendMidiCC(4, 90, 2);
	mFaderModeMaster = 2;

	setAllChannelsMode(3); // read+write

	// Set Master Section to RW (Master fader and Joysticks)
	sendMidiCC(5, 64, 3);
	sendMidiCC(4, 88, 3);
	sendMidiCC(4, 90, 3);
	mFaderModeMaster = 3;
}

void PlugProcessor::deactivateMixer()
{
	sendMidiCC(5, 127, 0);

	setAllChannelsMode(0); // reset

	// Master Section (Master fader and Joysticks)
	sendMidiCC(5, 64, 0);
	sendMidiCC(4, 88, 0);
	sendMidiCC(4, 90, 0);

	rtmidiSendByte(255); // Send a single MIDI Byte value FF for System Reset
}

void PlugProcessor::resetAllMixer()
{
	// Set all to AUTO mode
	for (int i = 0; i < 48; i++) // TODO: abstract '48'
	{
		sendMidiCC(3, 64 + i, 3);
		sendMidiCC(4, 64 + i, 3);
		mFaderModeLeft[i] = 3;
		mFaderModeRight[i] = 3;
	}

	// Master Section (Master fader and Joysticks) to AUTO
	sendMidiCC(5, 64, 3);
	sendMidiCC(4, 88, 3);
	sendMidiCC(4, 90, 3);
}

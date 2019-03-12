#pragma once

#include "public.sdk/source/vst/vstaudioeffect.h"

#include <array>
#include <random>
#include <string>
#include <vector>

class PlugProcessor : public Vst::AudioEffect
{
public:
	enum
	{
		kControllerBankSelectMSB = 0,
		kControllerBankSelectLSB = 32,
		kControllerDamper = 64,
		kControllerPortamento,
		kControllerSostenuto,
		kControllerSoftPedal,
		kControllerAllSoundOff = 120,
		kControllerResetAllControllers,
		kControllerLocalControl,
		kControllerAllNotesOff,
		kControllerOmniModeOff,
		kControllerOmniModeOn,
		kControllerPolyMode,
		kControllerPolyModeOn,
		kStatusNoteOff = 0x80,
		kStatusNoteOn = 0x90,
		kStatusAfterTouch = 0xA0,
		kStatusControlChange = 0xB0,
		kStatusProgramChange = 0xC0,
		kStatusChannelPressure = 0xD0,
		kStatusPitchBend = 0xE0,
		kStatusSysEx = 0xF0,
		kStatusSysExCont = 0xF7,
		kStatusActiveSense = 0xFE,
		kStatusMeta = 0xFF
	};

	/** Useful definitions */
	static constexpr float kOne127th = 0.00787401574803f;
	static constexpr float kOne255th = 0.00392156862745f;
	static constexpr float kOne16384th = 0.00006103515625f;
	static constexpr float kPi = 3.14159265358f;
	static constexpr float kTwoPi = 6.28318530718f;
	static constexpr float kRandMaxRecip = 0.00003051757f;

	PlugProcessor();

	tresult PLUGIN_API initialize(FUnknown *context) override;

	static FUnknown* createInstance(void*)
	{
		return dynamic_cast<Vst::IAudioProcessor *>(new PlugProcessor());
	}

	void toggleTestMode();

	/** Send a complete 3-byte MIDI event */
	void rtmidiSendEvent(uint8 b1, uint8 b2, uint8 b3, uint8 port = 0);

	/** Send a single byte, used to send FF (System Reset) to deactivate the console */
	void rtmidiSendByte(uint8 b);

	/** This function must be static, otherwise it can't be referenced, but we use the *userData pointer to point to a non-static function */
	static void processMidiInput(double deltaTime, std::vector<uint8> *message, void *userData);

	/** Try to open MIDI ports */
	bool openMidiInPort(int port);
	bool openMidiOutPort(int port);

	/** Close MIDI Ports */
	void closeAllMidiPorts();

	/** Open MIDI Ports */
	void openAllMidiPorts();

	void sendMidiCC(uint8 channel, uint8 ccNum, uint8 value, uint8 port = 0)
	{
		rtmidiSendEvent(175 + channel, ccNum, value, port);
	}

	void setAllChannelsMode(int mode);
	void activateMixer();
	void deactivateMixer();
	void resetAllMixer();

private:
	float randGen() const
	{
		return mRandDis(mRandGen);
	}
private:
	int mNumFaders;
	std::array<class RtMidiIn *, 2> mMidiIn;
	std::array<class RtMidiOut *, 2> mMidiOut;
	std::array<int, 2> mMidiInPorts;
	std::array<int, 2> mMidiOutPorts;
	std::vector<std::string> mMidiInPortNames;
	std::vector<std::string> mMidiOutPortNames;
	std::vector<int> mMidiParameters;
	std::vector<int> mMidiFaders;
	std::vector<int> mFaderModeLeft;
	std::vector<int> mFaderModeRight;
	int mFaderModeMaster;
	std::vector<int> mMidiChannels;
	std::vector<int> mPrevCCVals;
	int mFaderSpeed;
	int mMuteSpeed;
	std::array<int, 2> mCountSamples;
	std::vector<float> mAnimRamp;
	std::random_device mRandDev;
	std::mt19937 mRandGen;
	std::uniform_real_distribution<float> mRandDis;
	bool mTestMode;
	bool mRtMidiPortsOpen;
	bool mAllMutesStatus;
};

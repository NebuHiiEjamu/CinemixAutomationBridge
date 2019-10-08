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
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class AutomationBridgeEditor;
class DeviceListBox;

//==============================================================================
/*
*/
class AutomationBridgeSettings    : public DocumentWindow
{
public:
    AutomationBridgeSettings (AutomationBridgeEditor&);
	  ~AutomationBridgeSettings();

    void paint (Graphics&) override;
    void resized() override;
    int getFaderCount() const;
    int getWidth() const;
    int getHeight() const;
    MidiDeviceInfo* getActiveInput(int) const;
    MidiDeviceInfo* getActiveOutput(int) const;

private:
  	void load();
	void save() const;

private:
	Array<MidiDeviceInfo>& inDevices;
	Array<MidiDeviceInfo>& outDevices;
    DeviceListBox inputs;
	DeviceListBox outputs;
	SortedSet<int> inputsOn;
	SortedSet<int> outputsOn;
    AutomationBridgeEditor &editor;
    Slider fadersSlider;
    TextButton cancelButton;
    TextButton applyButton;
    TextButton saveButton;
    String path;
    int faders;
    int width;
    int height;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AutomationBridgeSettings)
};

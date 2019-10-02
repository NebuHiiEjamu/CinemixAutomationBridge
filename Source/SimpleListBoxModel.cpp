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

#include "../JuceLibraryCode/JuceHeader.h"
#include "SimpleListBoxModel.h"

//==============================================================================
SimpleListBoxModel::SimpleListBoxModel(const Array<MidiDeviceInfo>& d),
	: data(d), font(Font(14.0f))
{
	addAndMakeVisible (listBox);
	listBox.setModel (this);
	listBox.setColour (ListBox::outlineColourId, Colours::grey);
	listBox.setOutlineThickness (1);

	for (MidiDeviceInfo& info : data)
	
}

SimpleListBoxModel::~SimpleListBoxModel()
{
}

int SimpleListBoxModel::getNumRows()
{
	return data.size();
}

void SimpleListBoxModel::paintListBoxItem (int rowNumber, Graphics& g, int width, int height,
    bool /*rowIsSelected*/)
{
	g.setColour (getLookAndFeel().findColour (ListBox::textColourId));
	g.setFont (font);
	g.drawText (data[rowNumber].name, 2, 0, width - 4, height, Justification::centredLeft);
	g.setColour (getLookAndFeel().findColour (ListBox::backgroundColourId));
	g.fillRect (width - 1, 0, 1, height);
}

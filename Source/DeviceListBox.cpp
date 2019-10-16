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
#include "DeviceListBox.h"

//==============================================================================
DeviceListBox::DeviceListBox (Array<MidiDeviceInfo>& mdi, SortedSet<int>& checked)
	: items(mdi), itemsChecked(checked)
{
    setModel (this);
    setOpaque (true);
}

DeviceListBox::~DeviceListBox()
{
}

int DeviceListBox::getNumRows()
{
	return items.size();
}

void DeviceListBox::listBoxItemClicked (int row, const MouseEvent& e)
{
	selectRow (row);
	if (e.x < getRowHeight())
	{
		if (itemsChecked.contains (row))
            itemsChecked.removeValue (row);
		else
			itemsChecked.add (row);
	}
}

void DeviceListBox::returnKeyPressed (int row)
{
	if (itemsChecked.contains (row))
        itemsChecked.removeValue (row);
	else
		itemsChecked.add (row);
}

void DeviceListBox::paint (Graphics& g)
{
	ListBox::paint(g);

	if (items.isEmpty())
	{
		g.setColour (Colours::dimgrey);
		g.setFont (getRowHeight() / 2);
		g.drawText ("No devices detected", 0, 0, getWidth(), getHeight() / 2,
			Justification::centred);
	}
    
    /*g.setColour (Colours::white);
    g.setFont (18.0f);
    g.drawText (String(count), getLocalBounds(), Justification::centredLeft, true);*/
}

void DeviceListBox::paintListBoxItem (int row, Graphics& g, int w, int h, bool)
{
	if (isPositiveAndBelow (row, items.size()))
	{
        g.fillAll (Colours::blue);

		bool enabled = itemsChecked.contains (row) ? true : false;
		int x = getRowHeight();
		float tickW = h * 0.75f;

		getLookAndFeel().drawTickBox (g, *this, x - tickW, (h - tickW) / 2, tickW, tickW, enabled,
			true, true, false);
		g.setFont (h * 0.6f);
        g.setColour (Colours::white);
		g.drawText (items[row].name, x + 5, 0, w - x - 5, h, Justification::centredLeft, true);
	}
}

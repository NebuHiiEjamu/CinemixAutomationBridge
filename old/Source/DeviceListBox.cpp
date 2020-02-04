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
		g.setFont (getRowHeight() / 2.0f);
		g.drawText ("No devices detected", 0, 0, getWidth(), getHeight() / 2, Justification::centred);
	}
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

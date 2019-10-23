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
#include "TouchPad.h"

//==============================================================================
TouchPadPoint::TouchPadPoint (TouchPad* tp)
	: parent(tp), xValue(0.5f), xRest(0.0f), yValue(0.5f), yRest(1.0f)
{
	setOpaque (true);
}

TouchPadPoint::~TouchPadPoint()
{
}

void TouchPadPoint::mouseDown (const MouseEvent& e)
{
}

void TouchPadPoint::mouseDrag (const MouseEvent& e)
{
}

void TouchPadPoint::modifierKeysChanged (const ModifierKeys& keys)
{
	if ((parent->isMouseButtonDown() || parent->isMouseOverOrDragging()) &&
		(keys.isShiftDown() || keys.isCtrlDown() || keys.isAltDown()))
	{
		xValue = yValue = 0.5f;
	}
}

void TouchPadPoint::paint (Graphics& g)
{
	g.fillAll (Colours::blue); // to erase past positions

	Point<float> mp = parent->getMouseXYRelative().toFloat();
	g.setColour (Colours::cyan);
	g.fillEllipse (mp.x - 12.0f, mp.y - 12.0f, 24.0f, 24.0f);
}

void TouchPadPoint::resized()
{
}

TouchPad::TouchPad()
	: point (this)
{
	setOpaque (true);
}

TouchPad::~TouchPad()
{
}

void TouchPad::paint (Graphics& g)
{
    g.fillAll (Colours::blue);

	// draw an inset border
    g.setColour (Colours::darkblue);
	g.drawLine (getX(), getY(), getX() + getWidth(), getY(), 1);
	g.drawLine (getX(), getY(), getX(), getY() + getHeight(), 1);
	g.setColour (Colours::lightblue);
	g.drawLine (getX() + getWidth(), getY() + 1, getX() + getWidth(), getY() + getHeight(), 1);
	g.drawLine (getX() + 1, getY() + getHeight(), getX() + getWidth(), getY() + getHeight(), 1);
}

void TouchPad::resized()
{
}

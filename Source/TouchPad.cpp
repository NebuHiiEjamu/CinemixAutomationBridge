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

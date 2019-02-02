#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class TouchPad : public Component
{
public:
	TouchPad(const String&);
	
	//void paint(Graphics &g) override; // TODO: Is this needed?
	void mouseEnter(const MouseEvent &me) override;
	
	void getRestPoint(float &x, float &y) const
	{
		x = xRest;
		y = 1.f - yRest;
	}
public:
	float xValue, yValue;
protected:
	Image background;
	float xRest, yRest;
private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TouchPad)
};

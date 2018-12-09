#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class TouchPad: public Component
{
public:
	TouchPad();
	
	void paint(Graphics &g) override;
	void mouseEnter(const MouseEvent &me) override;
	
	void getRestPoint(float &x, float &y) const
	{
		x = xRest;
		y = 1.0f - yRest;
	}
public:
	float xValue, yValue;
protected:
	Image bg;
	float xRest, yRest;
private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TouchPad)
};

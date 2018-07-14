#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class CTouchPad: public Component
{
public:
	CTouchPad();
	
	void paint(Graphics &g) override;
	void mouseEnter(const MouseEvent &me) override;
	
	void getRestPoint(float &x, float &y) const
	{
		x = m_fXRest;
		y = 1.0f - m_fYRest;
	}
public:
	float fXValue, fYValue;
protected:
	Image m_bg;
	float m_fXRest, m_fYRest;
private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CTouchPad)
};

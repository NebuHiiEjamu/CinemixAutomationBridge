#include "TouchPad.h"

TouchPad::TouchPad(): xValue(0.5f), yValue(0.5f),
	xRest(0.0f), yRest(1.0f)
{
}

void TouchPad::mouseEnter(const MouseEvent &me) override
{
	if (me.mods.isCommandDown() || me.mods.isAltDown() || me.mods.isShiftDown() ||
		me.mods.isCtrlDown())
	{
		xValue = yValue = 0.5f;
		return;
	}
}

#include "TouchPad.h"

CTouchPad::CTouchPad(): fXValue(0.5f), fYValue(0.5f),
	m_fXRest(0.0f), m_fYRest(1.0f)
{
}

void CTouchPad::mouseEnter(const MouseEvent &me) override
{
	if (me.mods.isCommandDown() || me.mods.isAltDown() || me.mods.isShiftDown() ||
		me.mods.isCtrlDown())
	{
		fXValue = fYValue = 0.5f;
		return;
	}
}

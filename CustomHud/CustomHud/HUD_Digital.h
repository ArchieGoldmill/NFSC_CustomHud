#pragma once
#include "HUD_Element.h"

class HUD_Digital : public HUD_Element
{
public:
	HUD_Digital(LPDIRECT3DDEVICE9 pDevice) : HUD_Element(pDevice)
	{

	}

protected:
	int GetNumDigits(int val)
	{
		if (val < 10) return 1;
		if (val < 100) return 2;
		if (val < 1000) return 3;
		if (val < 10000) return 4;

		return -1;
	}
};
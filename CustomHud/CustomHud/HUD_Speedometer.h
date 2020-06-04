#pragma once
#include "HUD_Element.h"
#include "HUD_Digit.h"
#include "DALVehicleCarbon.h"

class HUD_Speedometer : HUD_Element
{
private:
	HUD_Digit* Speed[3];

public:
	HUD_Speedometer(LPDIRECT3DDEVICE9 pDevice, float size, float distance, D3DXVECTOR2 position, Sprite* digits) : HUD_Element(pDevice)
	{
		for (int i = 0; i < 3; i++)
		{
			IntValueCallback* cb = 0;
			if (i == 0)
			{
				cb = GetSpeed0;
			}
			if (i == 1)
			{
				cb = GetSpeed1;
			}
			if (i == 2)
			{
				cb = GetSpeed2;
			}

			D3DXVECTOR2 pos;
			pos.x = i * (size / 3.0f) + position.x + distance * i;
			pos.y = position.y;

			this->Speed[i] = new HUD_Digit(pDevice, size, pos, digits, cb, D3DCOLOR_RGBA(12, 26, 48, 255));
		}
	}

	void Draw()
	{
		for (int i = 0; i < 3; i++)
		{
			this->Speed[i]->Draw();
		}
	}

	void Release()
	{
		for (int i = 0; i < 3; i++)
		{
			this->Speed[i]->Release();
		}
	}

	~HUD_Speedometer()
	{
		for (int i = 0; i < 3; i++)
		{
			delete this->Speed[i];
		}
	}
};

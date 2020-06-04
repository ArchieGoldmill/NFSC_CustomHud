#pragma once
#include "HUD_Element.h"
#include "HUD_Digit.h"
#include "DALVehicleCarbon.h"

class HUD_Speedometer : HUD_Element
{
private:
	HUD_Digit* Speed[3];
	Sprite* units;
	float size;
	float distance;
	D3DXVECTOR2 position;

public:
	HUD_Speedometer(LPDIRECT3DDEVICE9 pDevice, float size, float distance, D3DXVECTOR2 position, Sprite* digits, Sprite* units) : HUD_Element(pDevice)
	{
		this->units = units;
		this->size = size;
		this->position = position;
		this->distance = distance;

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

		RECT rect;
		if (IsKMH())
		{
			rect.left = this->units->Info.Width / 2.0;
			rect.right = this->units->Info.Width;
		}
		else
		{
			rect.left = 0;
			rect.right = this->units->Info.Width / 2.0;
		}

		rect.top = 0;
		rect.bottom = this->units->Info.Height;

		this->Setup(this->units, { 128, 32 }, { 0, 0 }, { 162, 65 }, &rect, 0);
		this->units->Draw(&rect, D3DCOLOR_RGBA(150, 227, 255, 255));
	}

	void Release()
	{
		if (!this->isReleased)
		{
			for (int i = 0; i < 3; i++)
			{
				this->Speed[i]->Release();
			}

			this->units->Release();

			this->isReleased = true;
		}
	}

	~HUD_Speedometer()
	{
		this->Release();

		for (int i = 0; i < 3; i++)
		{
			delete this->Speed[i];
		}
	}
};

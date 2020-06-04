#pragma once
#include "HUD_Element.h"

class HUD_Digit : HUD_Element
{
private:
	Sprite* digits;
	float size;
	D3DXVECTOR2 position;
	IntValueCallback* getNumber;
	D3DCOLOR color;

public:
	HUD_Digit(LPDIRECT3DDEVICE9 pDevice, float size, D3DXVECTOR2 position, Sprite* digits, IntValueCallback* getNumber, D3DCOLOR color) : HUD_Element(pDevice)
	{
		this->digits = digits;
		this->size = size;
		this->position = position;
		this->getNumber = getNumber;
		this->color = color;
	}

	void Draw()
	{
		D3DXVECTOR2 size;
		size.x = 12 * (this->size / 1.55);
		size.y = this->size;


		int gear = this->getNumber();
		float numSize = this->digits->Info.Width / 12.0;

		RECT rect;
		rect.left = (gear - 1) * numSize + 1;
		rect.right = numSize * gear - 2;
		rect.top = 0;
		rect.bottom = this->digits->Info.Height;

		this->Setup(this->digits, size, { 0, 0 }, this->position, &rect, 0);

		this->digits->Draw(&rect, color);
	}

	void Release()
	{
		if (!this->isReleased)
		{
			this->digits->Release();
			this->isReleased = true;
		}
	}

	~HUD_Digit()
	{
		this->Release();
	}
};
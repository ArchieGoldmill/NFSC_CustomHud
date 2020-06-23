#pragma once
#include "HUD_Element.h"

class HUD_Digit : HUD_Element
{
private:
	HUD_Digit_Params params;
	Sprite* digits = NULL;

public:
	HUD_Digit(LPDIRECT3DDEVICE9 pDevice, HUD_Digit_Params& params) : HUD_Element(pDevice)
	{
		this->params = params;

		if (!this->params.TextureDigits.empty())
		{
			this->digits = new Sprite(pDevice, this->params.TextureDigits);
		}
	}

	void Draw()
	{
		if (this->digits == NULL)
		{
			return;
		}

		D3DXVECTOR2 size;
		size.x = 12 * (this->params.Size / 1.55);
		size.y = this->params.Size;

		int gear = this->params.GetNumber();
		float numSize = this->digits->Info.Width / 12.0;

		RECT rect;
		rect.left = (gear - 1) * numSize + 1;
		rect.right = numSize * gear - 2;
		rect.top = 0;
		rect.bottom = this->digits->Info.Height;

		this->Setup(this->digits, size, { 0, 0 }, this->params.Position, &rect, 0);

		this->digits->Draw(&rect, this->params.Color);
	}

	void Release()
	{
		if (!this->isReleased)
		{
			this->digits->Release();
			delete this->digits;

			this->isReleased = true;
		}
	}

	~HUD_Digit()
	{
		this->Release();
	}
};
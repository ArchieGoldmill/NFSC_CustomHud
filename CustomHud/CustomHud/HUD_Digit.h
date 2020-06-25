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

		int gear = this->params.GetNumber();
		float numSize = this->digits->Info.Width / 12.0f;

		RECT rect;
		rect.left = (gear - 1) * numSize;
		rect.right = gear * numSize;
		rect.top = 0;
		rect.bottom = this->digits->Info.Height;

		D3DXVECTOR2 size;
		size.x = this->params.Size / 1.50f;
		size.y = this->params.Size;

		D3DXVECTOR2 texSize = size;
		texSize.x *= 12;

		this->Setup(this->digits, texSize, { 0, 0 }, this->params.Position, &size, 0);

		this->digits->Draw(&rect, this->params.Color);
	}

	void Release()
	{
		if (!this->isReleased)
		{
			if (this->digits != NULL)
			{
				delete this->digits;
			}

			this->isReleased = true;
		}
	}

	~HUD_Digit()
	{
		this->Release();
	}
};
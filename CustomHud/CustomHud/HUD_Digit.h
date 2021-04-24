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

		if (!this->params.DigitsTexture.empty())
		{
			this->digits = new Sprite(pDevice, this->params.DigitsTexture, this->params.DigitsTextureBlendMode, D3DXVECTOR2(0, 0));
		}
	}

	void SetPositionX(float position)
	{
		this->params.Position.x = position;
	}

	void Draw()
	{
		if (this->params.BackgroundColor)
		{
			this->DrawDigit(9, this->params.BackgroundColor);
		}

		this->DrawDigit(this->params.GetNumber(), this->params.Color);
	}

	void Draw(bool drawDigit)
	{
		if (this->params.BackgroundColor)
		{
			this->DrawDigit(9, this->params.BackgroundColor);
		}

		if ((drawDigit && this->params.BackgroundColor) || !this->params.BackgroundColor)
		{
			this->DrawDigit(this->params.GetNumber(), this->params.Color);
		}
	}

	void DrawDigit(int gear, D3DCOLOR color)
	{
		if (this->digits == NULL)
		{
			return;
		}

		float numSize = this->digits->Info.Width / 12.0f;

		RECT rect;
		rect.left = (gear - 1) * numSize;
		rect.right = gear * numSize;
		rect.top = 0;
		rect.bottom = this->digits->Info.Height;

		D3DXVECTOR2 size;
		size.x = (this->digits->InfoOriginal.Width / 12.0f) / this->digits->InfoOriginal.Height * this->params.Size;
		size.y = this->params.Size;

		D3DXVECTOR2 texSize = size;
		texSize.x *= 12;

		this->Setup(this->digits, texSize, { 0, 0 }, this->params.Position, &size, 0);

		this->digits->Draw(&rect, color);
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
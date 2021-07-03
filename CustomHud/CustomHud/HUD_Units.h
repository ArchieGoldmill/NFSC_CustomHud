#pragma once
#include "HUD_Element.h"
#include "GameApi.h"

class HUD_Units : HUD_Element
{
private:
	HUD_Units_Params params;
	Sprite* units = NULL;

public:
	HUD_Units(LPDIRECT3DDEVICE9 pDevice, HUD_Units_Params& params) : HUD_Element(pDevice)
	{
		this->params = params;

		if (!this->params.UnitsTexture.empty())
		{
			this->units = new Sprite(pDevice, this->params.UnitsTexture, this->params.UnitsTextureBlendMode, D3DXVECTOR2(0, 0));
		}
	}

	void Draw()
	{
		if (this->units)
		{
			RECT rect;
			if (Game::GetUnits())
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

			D3DXVECTOR2 size;
			size.x = this->params.Size * 2;
			size.y = this->params.Size;

			D3DXVECTOR2 texSize = size;
			texSize.x *= 2;

			this->Setup(this->units, texSize, { 0, 0 }, this->params.Position, &size, 0);
			this->units->Draw(&rect, this->params.Color);
		}
	}

	void Release()
	{
		if (!this->isReleased)
		{
			if (this->units)
			{
				delete this->units;
			}

			this->isReleased = true;
		}
	}
};
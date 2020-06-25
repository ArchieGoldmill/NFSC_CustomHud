#pragma once
#include "HUD_Element.h"

class HUD_Filled : HUD_Element
{
	HUD_Filled_Params params;
	Sprite* texture = NULL;
	Sprite* background = NULL;

public:
	HUD_Filled(LPDIRECT3DDEVICE9 pDevice, HUD_Filled_Params& params) : HUD_Element(pDevice)
	{
		this->params = params;

		if (!this->params.TextureFilled.empty())
		{
			this->texture = new Sprite(pDevice, this->params.TextureFilled);
		}

		if (!this->params.TextureBackground.empty())
		{
			this->background = new Sprite(pDevice, this->params.TextureBackground);
		}
	}

	void Draw()
	{
		this->DrawBackground();
		this->DrawFilled();
	}

	void DrawBackground()
	{
		if (this->background == NULL)
		{
			return;
		}

		this->Setup(this->background, { this->params.Size, this->params.Size }, { 0, 0 }, this->params.Position, NULL, 0);

		this->background->Draw(NULL, this->params.BackgroundColor);
	}

	void DrawFilled()
	{
		if (this->texture == NULL)
		{
			return;
		}

		float val = this->params.GetValue();

		D3DXVECTOR2 size;
		size.x = this->params.Size;
		size.y = this->params.Size;

		D3DXVECTOR2 texSize = size;
		RECT rect;
		if (this->params.Direction)
		{
			float fill = val / this->params.Value;

			rect.left = 0;
			rect.bottom = this->texture->Info.Height;
			rect.right = this->texture->Info.Width;
			rect.top = this->texture->Info.Height * (1.0f - fill);
			texSize.y *= fill;
		}
		else
		{
			float fill = val / this->params.Value;

			rect.left = 0;
			rect.bottom = this->texture->Info.Height;
			rect.right = this->texture->Info.Width * fill;
			rect.top = 0;
		}

		this->Setup(this->texture, size, { 0, 0 }, this->params.Position, &texSize, 0);

		this->texture->Draw(&rect, this->params.Color);
	}

	void Release()
	{
		if (!this->isReleased)
		{
			if (this->texture != NULL)
			{
				delete this->texture;
			}

			if (this->background != NULL)
			{
				delete this->background;
			}

			this->isReleased = true;
		}
	}

	~HUD_Filled()
	{
		this->Release();
	}
};
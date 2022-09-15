#pragma once
#include "HUD_Element.h"

class HUD_Liniar : HUD_Element
{
	HUD_Liniar_Params params;
	Sprite* texture = NULL;
	Sprite* background = NULL;

public:
	HUD_Liniar(LPDIRECT3DDEVICE9 pDevice, HUD_Liniar_Params& params) : HUD_Element(pDevice)
	{
		this->params = params;

		if (!this->params.FilledTexture.empty())
		{
			this->texture = new Sprite(pDevice, this->params.FilledTexture, this->params.FilledTextureBlendMode, { 0, 0 });
		}

		if (!this->params.BackgroundTexture.empty())
		{
			this->background = new Sprite(pDevice, this->params.BackgroundTexture, this->params.BackgroundTextureBlendMode, { 0, 0 });
		}
	}

	void Draw()
	{
		bool draw = true;
		if (this->params.IsInstalled)
		{
			draw = this->params.IsInstalled();
		}

		if (draw)
		{
			this->DrawBackground();
			this->DrawFilled();
		}
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
		float fill = val / this->params.Value;
		if (this->params.Direction == 2)
		{
			rect.left = this->texture->Info.Width * (this->params.Value - fill);
			rect.bottom = this->texture->Info.Height;
			rect.right = this->texture->Info.Width;
			rect.top = 0;
			texSize.x *= fill;
		}
		else if (this->params.Direction == 1)
		{
			rect.left = 0;
			rect.bottom = this->texture->Info.Height;
			rect.right = this->texture->Info.Width;
			rect.top = this->texture->Info.Height * (this->params.Value - fill);
			texSize.y *= fill;
		}
		else
		{
			rect.left = 0;
			rect.bottom = this->texture->Info.Height;
			rect.right = this->texture->Info.Width * fill;
			rect.top = 0;
		}

		this->Setup(this->texture, size, { 0, 0 }, this->params.Position, &texSize, 0);

		D3DCOLOR color = this->params.FilledColor;
		if (this->params.IsInperfectZone != NULL && this->params.FilledPerfectZoneColor)
		{
			if (this->params.IsInperfectZone())
			{
				color = this->params.FilledPerfectZoneColor;
			}
		}
		this->texture->Draw(&rect, color);
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

	~HUD_Liniar()
	{
		this->Release();
	}
};
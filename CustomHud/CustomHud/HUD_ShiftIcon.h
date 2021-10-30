#pragma once
#include "HUD_Element.h"
#include "GameApi.h"

union _D3DCOLOR {
	D3DCOLOR Color;
	unsigned char Bytes[4];
};

class HUD_ShiftIcon : HUD_Element {

private:
	HUD_ShiftIcon_Params params;
	Sprite* texture = NULL;
	float counter;

public:
	HUD_ShiftIcon(LPDIRECT3DDEVICE9 pDevice, HUD_ShiftIcon_Params& params) : HUD_Element(pDevice)
	{
		this->params = params;

		if (!this->params.IconTexture.empty())
		{
			this->texture = new Sprite(pDevice, this->params.IconTexture, this->params.IconTextureBlendMode, D3DXVECTOR2(0, 0));
		}
	}

	void Draw()
	{
		if (!this->texture)
		{
			return;
		}

		this->Setup(this->texture, { this->params.Size, this->params.Size }, { 0, 0 }, this->params.Position, NULL, 0);

		if (Game::GetRPM() + 0.5f >= Game::GetRedline())
		{
			if (this->params.BlinkMode == 0)
			{
				this->texture->Draw(NULL, this->params.Color);
			}

			if (this->params.BlinkMode)
			{
				this->counter += Global::DeltaTime;

				if (this->params.BlinkMode == 1)
				{
					if (this->counter > 1000)
					{
						this->counter = 0;
					}

					if (this->counter > 500)
					{
						this->texture->Draw(NULL, this->params.Color);
					}
				}
				else if (this->params.BlinkMode == 2)
				{
					if (this->counter > 500)
					{
						this->counter = 0;
					}

					int val = this->counter;
					if (val > 250)
					{
						val = 500 - val;
					}

					char alpha = val / 500.0f * 256;

					_D3DCOLOR color;
					color.Color = this->params.Color;
					color.Bytes[3] = alpha;
					this->texture->Draw(NULL, color.Color);
				}
			}
		}
		else
		{
			this->counter = 0;
		}
	}

	void Release()
	{
		if (!this->isReleased)
		{
			if (this->texture)
			{
				delete this->texture;
			}

			this->isReleased = true;
		}
	}

	~HUD_ShiftIcon()
	{
		this->Release();
	}
};

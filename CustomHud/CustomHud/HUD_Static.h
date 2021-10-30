#pragma once
#include "HUD_Element.h"

class HUD_Static : public HUD_Element
{
private:
	HUD_Static_Params params;
	Sprite* texture = NULL;

public:
	HUD_Static(LPDIRECT3DDEVICE9 pDevice, HUD_Static_Params& params) : HUD_Element(pDevice)
	{
		this->params = params;

		if (!this->params.Texture.empty())
		{
			this->texture = new Sprite(pDevice, this->params.Texture, this->params.TextureBlendMode, D3DXVECTOR2(0, 0));			
		}
	}

	void Draw()
	{
		if (this->texture == NULL)
		{
			return;
		}

		this->Setup(this->texture, { this->params.Size, this->params.Size }, { 0, 0 }, this->params.Position, NULL, 0);
		this->texture->Draw(NULL, this->params.Color);
	}

	void Release()
	{
		if (!this->isReleased)
		{
			if (this->texture != NULL)
			{
				delete this->texture;
			}

			this->isReleased = true;
		}
	}

	~HUD_Static()
	{
		this->Release();
	}
};
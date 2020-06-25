#pragma once
#include "HUD_Element.h"
#include "CircleSprite.h"

class HUD_Gauge : HUD_Element
{
private:
	Sprite* numbers = NULL;
	Sprite* arrow = NULL;
	Sprite* background = NULL;
	CircleSprite* masked = NULL;
	CircleSprite* arrowMasked = NULL;

	HUD_Gauge_Params params;

public:
	HUD_Gauge(LPDIRECT3DDEVICE9 pDevice, HUD_Gauge_Params& params) : HUD_Element(pDevice)
	{
		this->pDevice = pDevice;
		this->params = params;

		if (!this->params.TextureNumbers.empty())
		{
			this->numbers = new Sprite(pDevice, this->params.TextureNumbers);
		}

		if (!this->params.TextureArrow.empty())
		{
			this->arrow = new Sprite(pDevice, this->params.TextureArrow);
		}

		if (!this->params.TextureBackground.empty())
		{
			this->background = new Sprite(pDevice, this->params.TextureBackground);
		}

		if (!this->params.TextureBackgroundMasked.empty())
		{
			D3DXVECTOR2 maskSize = { this->params.BackgroundMaskedSize, this->params.BackgroundMaskedSize };
			this->masked = new CircleSprite(pDevice, this->params.TextureBackgroundMasked, &maskSize);
		}

		if (!this->params.TextureArrowMasked.empty())
		{
			D3DXVECTOR2 maskSize = { this->params.ArrowMaskedSize, this->params.ArrowMaskedSize };
			this->arrowMasked = new CircleSprite(pDevice, this->params.TextureArrowMasked, &maskSize);
		}
	}

	void Draw()
	{
		this->DrawBackground();
		this->DrawNumbers();
		this->DrawMasked();
		this->DrawArrow();
		this->DrawArrowMasked();
	}

	void Release()
	{
		if (!this->isReleased)
		{
			if (this->numbers != NULL) 
			{
				delete this->numbers;
			}
			if (this->arrow != NULL) 
			{
				delete this->arrow;
			}
			if (this->background != NULL) 
			{
				delete this->background;
			}
			if (this->masked != NULL)
			{
				delete this->masked;
			}
			if (this->arrowMasked != NULL) 
			{
				delete this->arrowMasked;
			}

			this->isReleased = true;
		}
	}

	~HUD_Gauge()
	{
		this->Release();
	}

private:
	void DrawNumbers()
	{
		if (this->numbers == NULL)
		{
			return;
		}

		float rpm = this->params.GetArrowValue();
		float redline = rpm;
		if (this->params.GetMaxValue != NULL)
		{
			redline = this->params.GetMaxValue();
		}

		D3DCOLOR color;
		if (redline - rpm < this->params.NumbersMaxThreshold)
		{
			color = this->params.NumbersMaxColor;
		}
		else
		{
			color = this->params.NumbersColor;
		}

		this->Setup(this->numbers, { this->params.Size, this->params.Size }, { 0, 0 }, this->params.Position, NULL, 0);

		this->numbers->Draw(NULL, color);
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

	void DrawMasked()
	{
		if (this->masked == NULL)
		{
			return;
		}

		if (this->params.GetMaskValue1 == NULL)
		{
			return;
		}

		this->Setup(this->masked, { this->params.Size, this->params.Size }, { 0, 0 }, this->params.Position, NULL, 0);

		float step = (this->params.ArrowMaxAngle - this->params.ArrowMinAngle) / this->params.Value;
		float val1 = this->params.GetMaskValue1();
		float a1 = step * val1 + this->params.ArrowMinAngle;

		float val2 = this->params.GetMaskValue2();
		float a2 = step * val2 + this->params.ArrowMinAngle;

		this->masked->SetupMask({ 0.5f, 0.5f }, a1, a2, this->params.BackgroundMaskedColor1, this->params.BackgroundMaskedColor2);

		this->masked->Draw(NULL, this->params.BackgroundMaskedColor);
	}

	void DrawArrowMasked()
	{
		if (this->masked == NULL)
		{
			return;
		}

		if (this->params.GetArrowMaskValue1 == NULL)
		{
			return;
		}

		this->Setup(this->masked, { this->params.Size, this->params.Size }, { 0, 0 }, this->params.Position, NULL, 0);

		float step = (this->params.ArrowMaxAngle - this->params.ArrowMinAngle) / this->params.Value;
		float val1 = this->params.GetArrowMaskValue1();
		float a1 = step * val1 + this->params.ArrowMinAngle;

		float val2 = this->params.GetArrowMaskValue2();
		float a2 = step * val2 + this->params.ArrowMinAngle;

		this->masked->SetupMask({ 0.5f, 0.5f }, a1, a2, this->params.ArrowMaskedColor1, this->params.ArrowMaskedColor2);

		this->masked->Draw(NULL, this->params.ArrowMaskedColor);
	}

	void DrawArrow()
	{
		if (this->arrow == NULL)
		{
			return;
		}

		D3DXVECTOR2 targetRes;
		targetRes.x = this->params.Size / 1.75f;
		targetRes.y = this->params.Size / 3.5f;

		D3DXVECTOR2 position;
		position.x = this->params.Size / 2.65f + this->params.Position.x;
		position.y = this->params.Size / 2.77f + this->params.Position.y;

		float val = this->params.GetArrowValue();

		float step = (this->params.ArrowMaxAngle - this->params.ArrowMinAngle) / this->params.Value;
		float rotation = step * val + this->params.ArrowMinAngle;

		this->Setup(this->arrow, targetRes, { 0.78f, 0.5f }, position, NULL, rotation);

		D3DCOLOR color = this->params.ArrowColor;
		if (this->params.IsInperfectZone != NULL)
		{
			if (this->params.IsInperfectZone())
			{
				color = this->params.PerfectZoneColor;
			}
		}

		this->arrow->Draw(NULL, color);
	}
};
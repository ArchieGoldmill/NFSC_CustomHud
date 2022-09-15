#pragma once
#include "HUD_Element.h"
#include "HUD_Hidable.h"
#include "CircleSprite.h"

class HUD_Gauge : HUD_Element, HUD_Hidable
{
private:
	Sprite* numbers = NULL;
	Sprite* arrow = NULL;
	Sprite* background = NULL;
	CircleSprite* masked = NULL;
	CircleSprite* arrowMasked = NULL;

	HUD_Gauge_Params params;

public:
	HUD_Gauge(LPDIRECT3DDEVICE9 pDevice, HUD_Gauge_Params& params) : HUD_Element(pDevice), HUD_Hidable(params.GetArrowValue, !params.HideOnMaxValue, params.Value)
	{
		this->pDevice = pDevice;
		this->params = params;

		if (!this->params.NumbersTexture.empty())
		{
			this->numbers = new Sprite(pDevice, this->params.NumbersTexture, this->params.NumbersTextureBlendMode, { 0, 0 });
		}

		if (!this->params.ArrowTexture.empty())
		{
			this->arrow = new Sprite(pDevice, this->params.ArrowTexture, this->params.ArrowTextureBlendMode, { 0, 0 });
		}

		if (!this->params.BackgroundTexture.empty())
		{
			this->background = new Sprite(pDevice, this->params.BackgroundTexture, this->params.BackgroundTextureBlendMode, { 0, 0 });
		}

		if (!this->params.NumbersMaskedTexture.empty())
		{
			this->masked = new CircleSprite(pDevice, this->params.NumbersMaskedTexture, this->params.NumbersMaskedTextureBlendMode, { this->params.NumbersMaskedSize, this->params.NumbersMaskedSize });
		}

		if (!this->params.ArrowMaskedTexture.empty())
		{
			this->arrowMasked = new CircleSprite(pDevice, this->params.ArrowMaskedTexture, this->params.ArrowMaskedTextureBlendMode, { this->params.ArrowMaskedSize, this->params.ArrowMaskedSize });
		}
	}

	void Draw()
	{
		if (this->IsInstalled())
		{
			if (this->params.HideOnMaxValue)
			{
				this->CalculateOpacity();
			}

			this->DrawBackground();
			this->DrawNumbers();
			this->DrawArrowMasked();
			this->DrawNumbersMasked();
		}
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
	bool IsInstalled()
	{
		bool draw = true;
		if (this->params.IsInstalled)
		{
			draw = this->params.IsInstalled();
		}

		return draw;
	}

	void DrawNumbers()
	{
		if (this->numbers == NULL)
		{
			return;
		}

		this->Setup(this->numbers, { this->params.Size, this->params.Size }, { 0, 0 }, this->params.Position, NULL, 0);

		this->numbers->Draw(NULL, this->SetOpacity(this->RedLineColor(this->params.NumbersColor, this->params.NumbersMaxColor, this->params.NumbersMaxThreshold)));
	}

	void DrawBackground()
	{
		if (this->background == NULL)
		{
			return;
		}

		this->Setup(this->background, { this->params.Size, this->params.Size }, { 0, 0 }, this->params.Position, NULL, 0);

		this->background->Draw(NULL, this->SetOpacity(this->params.BackgroundColor));
	}

	void DrawNumbersMasked()
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
		if (a1 > a2)
		{
			swap(a1, a2);
		}

		a2 += this->params.NumbersMaskedMaxOffset;

		this->masked->SetupMask({ 0.5f, 0.5f }, a1, a2, this->params.NumbersMaskedColor1, this->params.NumbersMaskedColor2);

		this->masked->Draw(NULL, this->SetOpacity(this->params.NumbersMaskedColor));
	}

	void DrawArrowMasked()
	{
		if (this->arrowMasked == NULL)
		{
			return;
		}

		if (this->params.GetArrowMaskValue1 == NULL)
		{
			return;
		}

		this->Setup(this->arrowMasked, { this->params.Size, this->params.Size }, { 0, 0 }, this->params.Position, NULL, 0);

		float step = (this->params.ArrowMaxAngle - this->params.ArrowMinAngle) / this->params.Value;
		float val1 = this->params.GetArrowMaskValue1();
		float a1 = step * val1 + this->params.ArrowMinAngle - this->params.ArrowMaskedMinOffset;

		float val2 = this->params.GetArrowMaskValue2();
		if (val2 > this->params.Value)
		{
			val2 = this->params.Value;
		}

		if (!this->params.CanBeNegative && val2 < 0)
		{
			val2 = 0;
		}

		float a2 = step * val2 + this->params.ArrowMinAngle;

		if (a1 > a2)
		{
			swap(a1, a2);
		}
		
		this->arrowMasked->SetupMask({ 0.5f, 0.5f }, a1, a2, this->params.ArrowMaskedColor1, this->params.ArrowMaskedColor2);

		D3DCOLOR color = this->params.ArrowMaskedColor;
		if (this->params.IsInperfectZone != NULL && this->params.ArrowMaskedPerfectZoneColor)
		{
			if (this->params.IsInperfectZone())
			{
				color = this->params.ArrowMaskedPerfectZoneColor;
			}
		}
		this->arrowMasked->Draw(NULL, this->SetOpacity(this->RedLineColor(color, this->params.ArrowMaskedMaxColor, this->params.ArrowMaskedMaxThreshold)));
	}

	D3DCOLOR RedLineColor(D3DCOLOR originalColor, D3DCOLOR redlineColor, float threshold)
	{
		float rpm = this->params.GetArrowValue();
		float redline = rpm;
		if (this->params.GetMaxValue != NULL)
		{
			redline = this->params.GetMaxValue();
		}

		return (threshold > 0) && (redline - rpm < threshold)
			? redlineColor
			: originalColor;
	}

public:
	void DrawArrow()
	{
		if (this->arrow == NULL || !this->IsInstalled())
		{
			return;
		}

		float arrowCenterOffset = this->params.ArrowCenterOffset;
		if (arrowCenterOffset > 1)
		{
			arrowCenterOffset = 1;
		}

		D3DXVECTOR2 targetRes;
		targetRes.x = this->params.Size / 1.4f * this->params.ArrowScale;
		targetRes.y = this->params.Size / 2.8f * this->params.ArrowScale;

		D3DXVECTOR2 position;
		position.x = this->params.Position.x + this->params.Size / 2.0f - targetRes.x * arrowCenterOffset;
		position.y = this->params.Position.y + this->params.Size / 2.0f - targetRes.y * 0.5;

		float val = this->params.GetArrowValue();
		if (val > this->params.Value)
		{
			val = this->params.Value;
		}

		if (!this->params.CanBeNegative && val < 0)
		{
			val = 0;
		}

		float step = (this->params.ArrowMaxAngle - this->params.ArrowMinAngle) / this->params.Value;
		float rotation = step * val + this->params.ArrowMinAngle;

		this->Setup(this->arrow, targetRes, { 1.0f - arrowCenterOffset, 0.5f }, position, NULL, rotation);

		D3DCOLOR color = this->params.ArrowColor;
		if (this->params.IsInperfectZone != NULL && this->params.ArrowPerfectZoneColor)
		{
			if (this->params.IsInperfectZone())
			{
				color = this->params.ArrowPerfectZoneColor;
			}
		}

		this->arrow->Draw(NULL, this->SetOpacity(color));
	}
};
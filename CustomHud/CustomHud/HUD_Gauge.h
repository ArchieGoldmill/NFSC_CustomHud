#pragma once
#include "HUD_Element.h"
#include "CircleSprite.h"

struct HUD_Gauge_Callbacks
{
	FloatValueCallback* GetArrowValue;
	FloatValueCallback* GetMaxValue;

	FloatValueCallback* GetMaskValue1;
	FloatValueCallback* GetMaskValue2;
	D3DCOLOR MaskColor1;
	D3DCOLOR MaskColor2;

	float ZeroAngle;
	float MaxAngle;
	float Value;
	int direction;
};

class HUD_Gauge : HUD_Element
{
private:
	Sprite* numbers;
	Sprite* arrow;
	Sprite* background;
	CircleSprite* masked;
	float size;
	D3DXVECTOR2 position;
	HUD_Gauge_Callbacks callbacks;

public:
	HUD_Gauge(LPDIRECT3DDEVICE9 pDevice, float size, D3DXVECTOR2 position, Sprite* numbers, Sprite* arrow, Sprite* background, CircleSprite* masked, HUD_Gauge_Callbacks callbacks) : HUD_Element(pDevice)
	{
		this->pDevice = pDevice;

		this->numbers = numbers;
		this->arrow = arrow;
		this->background = background;
		this->masked = masked;

		this->size = size;
		this->position = position;
		this->callbacks = callbacks;
	}

	void Draw()
	{
		this->DrawBackground();
		this->DrawNumbers();
		this->DrawMasked();
		this->DrawArrow();
	}

	void Release()
	{
		if (!this->isReleased)
		{
			if (this->numbers != NULL) this->numbers->Release();
			if (this->arrow != NULL) this->arrow->Release();
			if (this->background != NULL) this->background->Release();
			if (this->masked != NULL) this->masked->Release();
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

		float rpm = this->callbacks.GetArrowValue();
		float redline = this->callbacks.GetMaxValue();

		D3DCOLOR color = D3DCOLOR_RGBA(150, 227, 255, 255);
		D3DCOLOR colorMax = D3DCOLOR_RGBA(255, 30, 30, 255);
		if (redline - rpm < 0.2f)
		{
			color = colorMax;
		}

		this->Setup(this->numbers, { this->size,this->size }, { 0, 0 }, this->position, NULL, 0);

		this->numbers->Draw(NULL, color);
	}

	void DrawBackground()
	{
		if (this->background == NULL)
		{
			return;
		}

		this->Setup(this->background, { this->size, this->size }, { 0, 0 }, this->position, NULL, 0);

		this->background->Draw(NULL, D3DCOLOR_RGBA(255, 255, 255, 255));
	}

	void DrawMasked()
	{
		if (this->masked == NULL)
		{
			return;
		}

		if (this->callbacks.GetMaskValue1 == NULL)
		{
			return;
		}

		this->Setup(this->masked, { this->size, this->size }, { 0, 0 }, this->position, NULL, 0);

		float step = (this->callbacks.MaxAngle - this->callbacks.ZeroAngle) / this->callbacks.Value;
		float val1 = this->callbacks.GetMaskValue1();
		float a1 = step * val1 + this->callbacks.ZeroAngle;

		float val2 = this->callbacks.GetMaskValue2();
		float a2 = step * val2 + this->callbacks.ZeroAngle;

		this->masked->SetupMask({ 0.5f, 0.5f }, a1, a2, this->callbacks.MaskColor1, this->callbacks.MaskColor2);

		this->masked->Draw(NULL, D3DCOLOR_RGBA(255, 255, 255, 255));
	}

	void DrawArrow()
	{
		if (this->arrow == NULL)
		{
			return;
		}

		D3DXVECTOR2 targetRes;
		targetRes.x = this->size / 1.75f;
		targetRes.y = this->size / 3.5f;

		D3DXVECTOR2 position;
		position.x = this->size / 2.69f + this->position.x;
		position.y = this->size / 2.77f + this->position.y;

		float val = this->callbacks.GetArrowValue();

		float step = (this->callbacks.MaxAngle - this->callbacks.ZeroAngle) / this->callbacks.Value;
		float rotation = step * val + this->callbacks.ZeroAngle;

		this->Setup(this->arrow, targetRes, { 0.78f, 0.5f }, position, NULL, rotation);

		D3DCOLOR color = D3DCOLOR_RGBA(255, 44, 44, 170);

		// TODO
		/*if (IsInPerfectLaunchRange())
		{
			color = D3DCOLOR_RGBA(68, 245, 37, 170);
		}*/

		this->arrow->Draw(NULL, color);
	}
};
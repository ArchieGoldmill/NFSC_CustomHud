#pragma once
#include "HUD_Element.h"

struct HUD_Gauge_Callbacks
{
	FloatValueCallback* GetValue;
	FloatValueCallback* GetMaxValue;

	float ZeroAngle;
	float StepAngle;
};

class HUD_Gauge : HUD_Element
{
private:
	Sprite* numbers;
	Sprite* arrow;
	Sprite* background;
	float size;
	D3DXVECTOR2 position;
	HUD_Gauge_Callbacks callbacks;

public:
	HUD_Gauge(LPDIRECT3DDEVICE9 pDevice, float size, D3DXVECTOR2 position, Sprite* numbers, Sprite* arrow, Sprite* background, HUD_Gauge_Callbacks callbacks) : HUD_Element(pDevice)
	{
		this->pDevice = pDevice;

		this->numbers = numbers;
		this->arrow = arrow;
		this->background = background;

		this->size = size;
		this->position = position;
		this->callbacks = callbacks;
	}

	void Draw()
	{
		this->DrawNumbers();
		this->DrawArrow();
		this->DrawBackground();
	}

	void Release()
	{
		this->numbers->Release();
		this->arrow->Release();
		this->background->Release();
	}

	~HUD_Gauge()
	{
		this->Release();
	}

private:
	void DrawNumbers()
	{
		D3DXVECTOR2 center;
		center.x = 0;
		center.y = 0;

		float rpm = this->callbacks.GetValue();
		float redline = this->callbacks.GetMaxValue();

		D3DCOLOR color = D3DCOLOR_RGBA(150, 227, 255, 255);

		if (redline - rpm < 200)
		{
			color = D3DCOLOR_RGBA(255, 30, 30, 255);
		}

		this->Setup(this->numbers, { this->size,this->size }, center, this->position, NULL, 0);

		this->numbers->Draw(NULL, color);
	}

	void DrawBackground()
	{
		this->Setup(this->background, { this->size, this->size }, { 0, 0 }, this->position, NULL, 0);

		this->background->Draw(NULL, D3DCOLOR_RGBA(255, 255, 255, 255));
	}

	void DrawArrow()
	{
		D3DXVECTOR2 targetRes;
		targetRes.x = this->size / 1.75;
		targetRes.y = this->size / 3.5;

		D3DXVECTOR2 center;
		center.x = 0.78;
		center.y = 0.5;

		D3DXVECTOR2 position;
		position.x = this->size / 2.69 + this->position.x;
		position.y = this->size / 2.77 + this->position.y;

		float rpm = this->callbacks.GetValue();

		float rotation = this->callbacks.ZeroAngle + this->callbacks.StepAngle * rpm / 1000.0;

		this->Setup(this->arrow, targetRes, center, position, NULL, rotation);

		this->arrow->Draw(NULL, D3DCOLOR_RGBA(255, 44, 44, 170));
	}
};
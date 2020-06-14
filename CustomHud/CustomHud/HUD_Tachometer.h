#pragma once
#include "HUD_Element.h"
#include "HUD_Gauge.h"
#include "HUD_Digit.h"
#include "DALVehicleCarbon.h"

class HUD_Tachometer : HUD_Element
{
private:
	HUD_Gauge* Gauge;
	HUD_Digit* Gear;

public:
	HUD_Tachometer(LPDIRECT3DDEVICE9 pDevice, float size, D3DXVECTOR2 position, Sprite* numbers, Sprite* arrow, Sprite* digits, Sprite* background, CircleSprite* redline) : HUD_Element(pDevice)
	{
		HUD_Gauge_Callbacks hudGaugeCallbacks;

		hudGaugeCallbacks.GetArrowValue = GetRPM;
		hudGaugeCallbacks.GetMaxValue = GetRedline;

		hudGaugeCallbacks.GetMaskValue1 = GetRedline;
		hudGaugeCallbacks.GetMaskValue2 = []() { return 10.0f; };

		hudGaugeCallbacks.MaskColor1 = D3DCOLOR_RGBA(0, 0, 0, 0);
		hudGaugeCallbacks.MaskColor2 = D3DCOLOR_RGBA(255, 255, 255, 255);

		hudGaugeCallbacks.Value = 10;
		hudGaugeCallbacks.ZeroAngle = -30;
		hudGaugeCallbacks.MaxAngle = 210;
		hudGaugeCallbacks.direction = 1;
		this->Gauge = new HUD_Gauge(pDevice, size, position, numbers, arrow, background, redline, hudGaugeCallbacks);

		this->Gear = new HUD_Digit(pDevice, 80, { 183, 156 }, digits, GetGear, D3DCOLOR_RGBA(255, 44, 44, 255));
	}

	void Draw()
	{
		if (this->Gauge != NULL)
		{
			this->Gauge->Draw();
		}

		if (this->Gear != NULL)
		{
			this->Gear->Draw();
		}
	}


	void Release()
	{
		if (!this->isReleased)
		{
			if (this->Gauge != NULL)
			{
				this->Gauge->Release();
			}

			if (this->Gear != NULL)
			{
				this->Gear->Release();
			}

			this->isReleased = true;
		}
	}

	~HUD_Tachometer()
	{
		this->Release();

		if (this->Gauge != NULL)
		{
			delete this->Gauge;
		}

		if (this->Gear != NULL)
		{
			delete this->Gear;
		}
	}

};
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
	HUD_Tachometer(LPDIRECT3DDEVICE9 pDevice, float size, D3DXVECTOR2 position, Sprite* numbers, Sprite* arrow, Sprite* digits, Sprite* background) : HUD_Element(pDevice)
	{
		HUD_Gauge_Callbacks hudGaugeCallbacks;
		hudGaugeCallbacks.GetValue = GetRPM;
		hudGaugeCallbacks.GetMaxValue = GetRedline;
		hudGaugeCallbacks.ZeroAngle = -30;
		hudGaugeCallbacks.StepAngle = 24;
		this->Gauge = new HUD_Gauge(pDevice, size, position, numbers, arrow, background, hudGaugeCallbacks);

		this->Gear = new HUD_Digit(pDevice, 80, { 162 + position.x, 104 + position.y }, digits, GetGear, D3DCOLOR_RGBA(255, 44, 44, 255));
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
		if (this->Gauge != NULL)
		{
			this->Gauge->Release();
		}

		if (this->Gear != NULL)
		{
			this->Gear->Release();
		}
	}

	~HUD_Tachometer()
	{
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
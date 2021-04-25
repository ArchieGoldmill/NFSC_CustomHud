#pragma once
#include "HUD_Element.h"
#include "HUD_Gauge.h"
#include "HUD_Digit.h"
#include "GameApi.h"

class HUD_Tachometer : HUD_Element
{
private:
	HUD_Tachometer_Params params;
	HUD_Gauge* Gauge;
	HUD_Digit* Gear;

public:
	HUD_Tachometer(LPDIRECT3DDEVICE9 pDevice, HUD_Tachometer_Params& params) : HUD_Element(pDevice)
	{
		this->params = params;
		
		this->Gauge = new HUD_Gauge(pDevice, this->params.Gauge);

		this->Gear = new HUD_Digit(pDevice, this->params.Gear);
	}

	void Draw()
	{
		if (this->Gauge != NULL)
		{
			this->Gauge->Draw(false);
		}

		if (this->Gear != NULL)
		{
			this->Gear->Draw();
		}
	}

	void DrawArrow()
	{
		if (this->Gauge != NULL)
		{
			this->Gauge->DrawArrow();
		}
	}

	void Release()
	{
		if (!this->isReleased)
		{
			if (this->Gauge != NULL)
			{
				delete this->Gauge;
			}

			if (this->Gear != NULL)
			{
				delete this->Gear;
			}

			this->isReleased = true;
		}
	}

	~HUD_Tachometer()
	{
		this->Release();
	}

};
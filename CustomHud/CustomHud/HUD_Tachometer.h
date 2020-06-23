#pragma once
#include "HUD_Element.h"
#include "HUD_Gauge.h"
#include "HUD_Digit.h"
#include "DALVehicleCarbon.h"

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

		this->params.GaugeParams.GetArrowValue = GetRPM;
		this->params.GaugeParams.GetMaxValue = GetRedline;

		this->params.GaugeParams.GetMaskValue1 = GetRedline;
		this->params.GaugeParams.GetMaskValue2 = []() { return 10.0f; };

		this->params.GaugeParams.IsInperfectZone = IsInPerfectLaunchRange;

		this->params.GearParams.GetNumber = GetGear;

		
		this->Gauge = new HUD_Gauge(pDevice, this->params.GaugeParams);

		this->Gear = new HUD_Digit(pDevice, this->params.GearParams);
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
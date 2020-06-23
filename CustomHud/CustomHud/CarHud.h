#pragma once

#include <string>
#define _USE_MATH_DEFINES
#include <math.h>
#include "DALVehicleCarbon.h"
#include <map>

#include "HUD_Tachometer.h"
#include "HUD_Speedometer.h"

using namespace std;

class HUD
{
private:
	IDirect3DDevice9* pDevice;

	HUD_Tachometer* Tachometer = NULL;
	HUD_Speedometer* Speedometer = NULL;
	HUD_Gauge* Boost = NULL;
	HUD_Gauge* Nos = NULL;
	HUD_Gauge* SpeedBreak = NULL;

public:
	static bool ShowHUD;

	HUD(IDirect3DDevice9* pDevice)
	{
		this->pDevice = pDevice;

		if (Global::HUDParams.Tachometer.GaugeParams.Enabled)
		{
			this->Tachometer = new HUD_Tachometer(pDevice, Global::HUDParams.Tachometer);
		}

		if (Global::HUDParams.Speedometer.Enabled)
		{
			this->Speedometer = new HUD_Speedometer(pDevice, Global::HUDParams.Speedometer);
		}

		if (Global::HUDParams.Boost.Enabled)
		{
			Global::HUDParams.Boost.GetArrowValue = GetBoost;
			this->Boost = new HUD_Gauge(pDevice, Global::HUDParams.Boost);
		}

		if (Global::HUDParams.Nos.Enabled)
		{
			Global::HUDParams.Nos.GetMaskValue1 = []() {return 0.0f; };
			Global::HUDParams.Nos.GetMaskValue2 = GetNos;
			Global::HUDParams.Nos.GetArrowValue = GetNos;
			this->Nos = new HUD_Gauge(pDevice, Global::HUDParams.Nos);
		}

		if (Global::HUDParams.SpeedBreak.Enabled)
		{
			Global::HUDParams.SpeedBreak.GetMaskValue1 = []() {return 0.0f; };
			Global::HUDParams.SpeedBreak.GetMaskValue2 = GetSpeedBreaker;
			Global::HUDParams.SpeedBreak.GetArrowValue = GetSpeedBreaker;
			this->SpeedBreak = new HUD_Gauge(pDevice, Global::HUDParams.SpeedBreak);
		}
	}

	int gen = 0;
	void Draw()
	{
		auto start = chrono::steady_clock::now();

		if (!IsHudVisible() || !IsPlayerControlling() || !HUD::ShowHUD)
		{
			//return;
		}

		if (this->Tachometer != NULL)
		{
			this->Tachometer->Draw();
		}

		if (this->Speedometer != NULL)
		{
			this->Speedometer->Draw();
		}

		if (this->Boost != NULL)
		{
			this->Boost->Draw();
		}

		if (this->Nos != NULL)
		{
			this->Nos->Draw();
		}

		if (this->SpeedBreak != NULL)
		{
			this->SpeedBreak->Draw();
		}

		auto now = chrono::steady_clock::now();

		int a = chrono::duration_cast<std::chrono::microseconds>(now - start).count();

		gen = (a + gen) / 2;
	}

	~HUD()
	{
		if (this->Tachometer != NULL)
		{
			delete this->Tachometer;
		}

		if (this->Speedometer != NULL)
		{
			delete this->Speedometer;
		}

		if (this->Boost != NULL)
		{
			delete this->Boost;
		}

		if (this->Nos != NULL)
		{
			delete this->Nos;
		}

		if (this->SpeedBreak != NULL)
		{
			delete this->SpeedBreak;
		}
	}
};
bool HUD::ShowHUD = false;
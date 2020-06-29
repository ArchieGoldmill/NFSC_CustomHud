#pragma once

#include <string>
#define _USE_MATH_DEFINES
#include <math.h>
#include "DALVehicleCarbon.h"
#include "TextureStateManager.h"

#include "HUD_Tachometer.h"
#include "HUD_Speedometer.h"
#include "HUD_Filled.h"

using namespace std;

class HUD
{
private:
	LPDIRECT3DDEVICE9 pDevice;
	TextureStateManager* tsm;

	HUD_Tachometer* Tachometer = NULL;
	HUD_Speedometer* Speedometer = NULL;
	HUD_Gauge* Boost = NULL;
	HUD_Gauge* Nos = NULL;
	HUD_Filled* NosFilled = NULL;
	HUD_Gauge* SpeedBreak = NULL;

public:
	static bool ShowHUD;

	HUD(LPDIRECT3DDEVICE9 pDevice)
	{
		this->pDevice = pDevice;
		this->tsm = new TextureStateManager(pDevice);

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

		if (Global::HUDParams.NosFilled.Enabled)
		{
			Global::HUDParams.NosFilled.GetValue = GetNos;
			this->NosFilled = new HUD_Filled(pDevice, Global::HUDParams.NosFilled);
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

		for (int i = 0; i < NUM_TEX; i++)
		{
			this->tsm->SetTexture(i);
			this->tsm->SetTextureStageState(i, D3DTSS_COLOROP);
			this->tsm->SetTextureStageState(i, D3DTSS_COLORARG1);
			this->tsm->SetTextureStageState(i, D3DTSS_COLORARG2);
			this->tsm->SetTextureStageState(i, D3DTSS_ALPHAOP);
			this->tsm->SetTextureStageState(i, D3DTSS_ALPHAARG1);
			this->tsm->SetTextureStageState(i, D3DTSS_ALPHAARG2);
			this->tsm->SetTextureStageState(i, D3DTSS_BUMPENVMAT00);
			this->tsm->SetTextureStageState(i, D3DTSS_BUMPENVMAT01);
			this->tsm->SetTextureStageState(i, D3DTSS_BUMPENVMAT10);
			this->tsm->SetTextureStageState(i, D3DTSS_BUMPENVMAT11);
			this->tsm->SetTextureStageState(i, D3DTSS_TEXCOORDINDEX);
			this->tsm->SetTextureStageState(i, D3DTSS_BUMPENVLSCALE);
			this->tsm->SetTextureStageState(i, D3DTSS_BUMPENVLOFFSET);
			this->tsm->SetTextureStageState(i, D3DTSS_TEXTURETRANSFORMFLAGS);
			this->tsm->SetTextureStageState(i, D3DTSS_COLORARG0);
			this->tsm->SetTextureStageState(i, D3DTSS_ALPHAARG0);
			this->tsm->SetTextureStageState(i, D3DTSS_RESULTARG);
			this->tsm->SetTextureStageState(i, D3DTSS_CONSTANT);
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

		if (this->NosFilled != NULL)
		{
			this->NosFilled->Draw();
		}

		if (this->SpeedBreak != NULL)
		{
			this->SpeedBreak->Draw();
		}

		this->tsm->Restore();

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

		if (this->NosFilled != NULL)
		{
			delete this->NosFilled;
		}

		if (this->SpeedBreak != NULL)
		{
			delete this->SpeedBreak;
		}

		delete this->tsm;
	}
};
bool HUD::ShowHUD = false;
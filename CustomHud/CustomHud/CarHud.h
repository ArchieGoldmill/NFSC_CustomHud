#pragma once

#include <string>
#define _USE_MATH_DEFINES
#include <math.h>
#include "TextureStateManager.h"

#include "HUD_Gauge.h"
#include "HUD_Speedometer.h"
#include "HUD_Filled.h"
#include "HUD_Units.h"
#include "GameApi.h"
#include "HUD_ShiftIcon.h"

using namespace std;

class HUD
{
private:
	LPDIRECT3DDEVICE9 pDevice;
	//TextureStateManager* tsm;

	HUD_Gauge* Tachometer = NULL;
	HUD_Gauge* Speedometer = NULL;
	HUD_Speedometer* SpeedometerDigital = NULL;
	HUD_Gauge* Boost = NULL;
	HUD_Gauge* Nos = NULL;
	HUD_Filled* NosFilled = NULL;
	HUD_Gauge* SpeedBreak = NULL;
	HUD_Filled* SpeedBreakFilled = NULL;
	HUD_Digit* Gear = NULL;
	HUD_Units* Units = NULL;
	HUD_ShiftIcon* ShiftIcon = NULL;

public:
	HUD(LPDIRECT3DDEVICE9 pDevice)
	{
		this->pDevice = pDevice;
		//this->tsm = new TextureStateManager(pDevice);

		try
		{
			if (Global::HUDParams.Tachometer.Enabled)
			{
				Global::HUDParams.Tachometer.GetArrowValue = Game::GetRPM;
				Global::HUDParams.Tachometer.GetMaxValue = Game::GetRedline;

				Global::HUDParams.Tachometer.GetMaskValue1 = Game::GetRedline;
				Global::HUDParams.Tachometer.GetMaskValue2 = []() { return Global::HUDParams.Tachometer.Value; };

				Global::HUDParams.Tachometer.GetArrowMaskValue1 = []() { return 0.0f; };
				Global::HUDParams.Tachometer.GetArrowMaskValue2 = Game::GetRPM;

				Global::HUDParams.Tachometer.IsInperfectZone = Game::IsInPerfectLaunchRange;

				this->Tachometer = new HUD_Gauge(pDevice, Global::HUDParams.Tachometer);
			}

			if (Global::HUDParams.SpeedometerDigital.Enabled)
			{
				this->SpeedometerDigital = new HUD_Speedometer(pDevice, Global::HUDParams.SpeedometerDigital);
			}

			if (Global::HUDParams.Speedometer.Enabled)
			{
				Global::HUDParams.Speedometer.GetArrowValue = Game::GetSpeed;
				this->Speedometer = new HUD_Gauge(pDevice, Global::HUDParams.Speedometer);
			}

			if (Global::HUDParams.Boost.Enabled)
			{
				Global::HUDParams.Boost.GetArrowValue = Game::GetBoost;
				Global::HUDParams.Boost.GetMaskValue1 = []() {return 0.0f; };
				Global::HUDParams.Boost.GetMaskValue2 = Game::GetBoost;
				Global::HUDParams.Boost.IsInstalled = Game::IsBoostInstalled;
				this->Boost = new HUD_Gauge(pDevice, Global::HUDParams.Boost);
			}

			if (Global::HUDParams.Nos.Enabled)
			{
				Global::HUDParams.Nos.GetMaskValue1 = []() {return 0.0f; };
				Global::HUDParams.Nos.GetMaskValue2 = Game::GetNos;
				Global::HUDParams.Nos.GetArrowValue = Game::GetNos;
				Global::HUDParams.Nos.IsInstalled = Game::IsNosInstalled;
				this->Nos = new HUD_Gauge(pDevice, Global::HUDParams.Nos);
			}

			if (Global::HUDParams.NosFilled.Enabled)
			{
				Global::HUDParams.NosFilled.GetValue = Game::GetNos;
				Global::HUDParams.NosFilled.IsInstalled = Game::IsNosInstalled;
				this->NosFilled = new HUD_Filled(pDevice, Global::HUDParams.NosFilled);
			}

			if (Global::HUDParams.SpeedBreak.Enabled)
			{
				Global::HUDParams.SpeedBreak.GetMaskValue1 = []() {return 0.0f; };
				Global::HUDParams.SpeedBreak.GetMaskValue2 = Game::GetSpeedBreaker;
				Global::HUDParams.SpeedBreak.GetArrowValue = Game::GetSpeedBreaker;
				this->SpeedBreak = new HUD_Gauge(pDevice, Global::HUDParams.SpeedBreak);
			}

			if (Global::HUDParams.SpeedBreakFilled.Enabled)
			{
				Global::HUDParams.SpeedBreakFilled.GetValue = Game::GetSpeedBreaker;
				this->SpeedBreakFilled = new HUD_Filled(pDevice, Global::HUDParams.SpeedBreakFilled);
			}

			if (Global::HUDParams.Gear.Enabled)
			{
				Global::HUDParams.Gear.GetNumber = Game::GetGear;
				this->Gear = new HUD_Digit(pDevice, Global::HUDParams.Gear);
			}

			if (Global::HUDParams.Units.Enabled)
			{
				this->Units = new HUD_Units(pDevice, Global::HUDParams.Units);
			}

			if (Global::HUDParams.ShiftIcon.Enabled)
			{
				this->ShiftIcon = new HUD_ShiftIcon(pDevice, Global::HUDParams.ShiftIcon);
			}
		}
		catch (string& s)
		{
			MessageBoxA(NULL, s.c_str(), "Custom HUD", MB_ICONERROR);
		}
	}

	void Draw()
	{
		/*for (int i = 0; i < NUM_TEX; i++)
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
		}*/

		if (this->Tachometer)
		{
			this->Tachometer->Draw();
		}

		if (this->Speedometer)
		{
			this->Speedometer->Draw();
		}

		if (this->SpeedometerDigital)
		{
			this->SpeedometerDigital->Draw();
		}

		if (this->Boost)
		{
			this->Boost->Draw();
			this->Boost->DrawArrow();
		}

		if (this->Nos)
		{
			this->Nos->Draw();
			this->Nos->DrawArrow();
		}

		if (this->NosFilled)
		{
			this->NosFilled->Draw();
		}

		if (this->SpeedBreak)
		{
			this->SpeedBreak->Draw();
			this->SpeedBreak->DrawArrow();
		}

		if (this->SpeedBreakFilled)
		{
			this->SpeedBreakFilled->Draw();
		}

		if (this->Gear)
		{
			this->Gear->Draw();
		}

		if (this->Units)
		{
			this->Units->Draw();
		}

		if (this->ShiftIcon)
		{
			this->ShiftIcon->Draw();
		}

		if (this->Tachometer)
		{
			this->Tachometer->DrawArrow();
		}

		if (this->Speedometer)
		{
			this->Speedometer->DrawArrow();
		}

		//this->tsm->Restore();
	}

	~HUD()
	{
		if (this->Tachometer)
		{
			delete this->Tachometer;
		}

		if (this->SpeedometerDigital)
		{
			delete this->SpeedometerDigital;
		}

		if (this->Speedometer)
		{
			delete this->Speedometer;
		}

		if (this->Boost)
		{
			delete this->Boost;
		}

		if (this->Nos)
		{
			delete this->Nos;
		}

		if (this->NosFilled)
		{
			delete this->NosFilled;
		}

		if (this->SpeedBreak)
		{
			delete this->SpeedBreak;
		}

		if (this->SpeedBreakFilled)
		{
			delete this->SpeedBreakFilled;
		}

		if (this->Gear)
		{
			delete this->Gear;
		}

		if (this->Units)
		{
			delete this->Units;
		}

		if (this->ShiftIcon)
		{
			delete this->ShiftIcon;
		}

		//delete this->tsm;
	}
};
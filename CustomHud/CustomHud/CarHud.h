#pragma once

#include <string>
#define _USE_MATH_DEFINES
#include <math.h>
#include "TextureStateManager.h"

#include "HUD_Gauge.h"
#include "HUD_SpeedometerDigital.h"
#include "HUD_TachometerDigital.h"
#include "HUD_Liniar.h"
#include "HUD_Units.h"
#include "GameApi.h"
#include "HUD_ShiftIcon.h"
#include "HUD_Static.h"

using namespace std;

class HUD
{
private:
	LPDIRECT3DDEVICE9 pDevice;
	//TextureStateManager* tsm;

	HUD_Static* Background = NULL;
	HUD_Gauge* TachometerGauge = NULL;
	HUD_TachometerDigital* TachometerDigital = NULL;
	HUD_Liniar* TachometerLiniar = NULL;
	HUD_Gauge* SpeedometerGauge = NULL;
	HUD_SpeedometerDigital* SpeedometerDigital = NULL;
	HUD_Gauge* BoostGauge = NULL;
	HUD_Gauge* NosGauge = NULL;
	HUD_Liniar* NosLiniar = NULL;
	HUD_Gauge* SpeedBreakGauge = NULL;
	HUD_Liniar* SpeedBreakLiniar = NULL;
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
			if (Global::HUDParams.Background.Enabled)
			{
				this->Background = new HUD_Static(pDevice, Global::HUDParams.Background);
			}

			if (Global::HUDParams.TachometerGauge.Enabled)
			{
				Global::HUDParams.TachometerGauge.GetArrowValue = Game::GetRPM;
				Global::HUDParams.TachometerGauge.GetMaxValue = Game::GetRedline;

				Global::HUDParams.TachometerGauge.GetMaskValue1 = Game::GetRedline;
				Global::HUDParams.TachometerGauge.GetMaskValue2 = []() { return Global::HUDParams.TachometerGauge.Value; };

				Global::HUDParams.TachometerGauge.GetArrowMaskValue1 = []() { return 0.0f; };
				Global::HUDParams.TachometerGauge.GetArrowMaskValue2 = Game::GetRPM;

				Global::HUDParams.TachometerGauge.IsInperfectZone = Game::IsInPerfectLaunchRange;

				this->TachometerGauge = new HUD_Gauge(pDevice, Global::HUDParams.TachometerGauge);
			}

			if (Global::HUDParams.TachometerDigital.Enabled)
			{
				this->TachometerDigital = new HUD_TachometerDigital(pDevice, Global::HUDParams.TachometerDigital);
			}

			if (Global::HUDParams.TachometerLiniar.Enabled)
			{
				Global::HUDParams.TachometerLiniar.GetValue = Game::GetRPM;
				this->TachometerLiniar = new HUD_Liniar(pDevice, Global::HUDParams.TachometerLiniar);
			}

			if (Global::HUDParams.SpeedometerDigital.Enabled)
			{
				this->SpeedometerDigital = new HUD_SpeedometerDigital(pDevice, Global::HUDParams.SpeedometerDigital);
			}

			if (Global::HUDParams.SpeedometerGauge.Enabled)
			{
				Global::HUDParams.SpeedometerGauge.GetArrowValue = Game::GetSpeed;
				this->SpeedometerGauge = new HUD_Gauge(pDevice, Global::HUDParams.SpeedometerGauge);
			}

			if (Global::HUDParams.BoostGauge.Enabled)
			{
				Global::HUDParams.BoostGauge.GetArrowValue = Game::GetBoost;
				Global::HUDParams.BoostGauge.GetArrowMaskValue1 = []() {return 0.0f; };
				Global::HUDParams.BoostGauge.GetArrowMaskValue2 = Game::GetBoost;
				Global::HUDParams.BoostGauge.IsInstalled = Game::IsBoostInstalled;
				this->BoostGauge = new HUD_Gauge(pDevice, Global::HUDParams.BoostGauge);
			}

			if (Global::HUDParams.NosGauge.Enabled)
			{
				Global::HUDParams.NosGauge.GetArrowMaskValue1 = []() {return 0.0f; };
				Global::HUDParams.NosGauge.GetArrowMaskValue2 = Game::GetNos;
				Global::HUDParams.NosGauge.GetArrowValue = Game::GetNos;
				Global::HUDParams.NosGauge.IsInstalled = Game::IsNosInstalled;
				this->NosGauge = new HUD_Gauge(pDevice, Global::HUDParams.NosGauge);
			}

			if (Global::HUDParams.NosLiniar.Enabled)
			{
				Global::HUDParams.NosLiniar.GetValue = Game::GetNos;
				Global::HUDParams.NosLiniar.IsInstalled = Game::IsNosInstalled;
				this->NosLiniar = new HUD_Liniar(pDevice, Global::HUDParams.NosLiniar);
			}

			if (Global::HUDParams.SpeedBreakGauge.Enabled)
			{
				Global::HUDParams.SpeedBreakGauge.GetArrowMaskValue1 = []() {return 0.0f; };
				Global::HUDParams.SpeedBreakGauge.GetArrowMaskValue2 = Game::GetSpeedBreaker;
				Global::HUDParams.SpeedBreakGauge.GetArrowValue = Game::GetSpeedBreaker;
				Global::HUDParams.SpeedBreakGauge.IsInstalled = Game::IsSpeedBreakerInstalled;
				this->SpeedBreakGauge = new HUD_Gauge(pDevice, Global::HUDParams.SpeedBreakGauge);
			}

			if (Global::HUDParams.SpeedBreakLiniar.Enabled)
			{
				Global::HUDParams.SpeedBreakLiniar.GetValue = Game::GetSpeedBreaker;
				Global::HUDParams.SpeedBreakLiniar.IsInstalled = Game::IsSpeedBreakerInstalled;
				this->SpeedBreakLiniar = new HUD_Liniar(pDevice, Global::HUDParams.SpeedBreakLiniar);
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
		//for (int i = 0; i < NUM_TEX; i++)
		//{
		//	this->tsm->SetTexture(i);
		//	this->tsm->SetTextureStageState(i, D3DTSS_COLOROP);
		//	this->tsm->SetTextureStageState(i, D3DTSS_COLORARG1);
		//	this->tsm->SetTextureStageState(i, D3DTSS_COLORARG2);
		//	this->tsm->SetTextureStageState(i, D3DTSS_ALPHAOP);
		//	this->tsm->SetTextureStageState(i, D3DTSS_ALPHAARG1);
		//	this->tsm->SetTextureStageState(i, D3DTSS_ALPHAARG2);
		//	this->tsm->SetTextureStageState(i, D3DTSS_BUMPENVMAT00);
		//	this->tsm->SetTextureStageState(i, D3DTSS_BUMPENVMAT01);
		//	this->tsm->SetTextureStageState(i, D3DTSS_BUMPENVMAT10);
		//	this->tsm->SetTextureStageState(i, D3DTSS_BUMPENVMAT11);
		//	this->tsm->SetTextureStageState(i, D3DTSS_TEXCOORDINDEX);
		//	this->tsm->SetTextureStageState(i, D3DTSS_BUMPENVLSCALE);
		//	this->tsm->SetTextureStageState(i, D3DTSS_BUMPENVLOFFSET);
		//	this->tsm->SetTextureStageState(i, D3DTSS_TEXTURETRANSFORMFLAGS);
		//	this->tsm->SetTextureStageState(i, D3DTSS_COLORARG0);
		//	this->tsm->SetTextureStageState(i, D3DTSS_ALPHAARG0);
		//	this->tsm->SetTextureStageState(i, D3DTSS_RESULTARG);
		//	this->tsm->SetTextureStageState(i, D3DTSS_CONSTANT);
		//}

		if (this->Background)
		{
			this->Background->Draw();
		}

		if (this->TachometerGauge)
		{
			this->TachometerGauge->Draw();
		}
		
		if (this->TachometerDigital)
		{
			this->TachometerDigital->Draw();
		}
		
		if (this->TachometerLiniar)
		{
			this->TachometerLiniar->Draw();
		}

		if (this->SpeedometerGauge)
		{
			this->SpeedometerGauge->Draw();
		}

		if (this->SpeedometerDigital)
		{
			this->SpeedometerDigital->Draw();
		}

		if (this->BoostGauge)
		{
			this->BoostGauge->Draw();
			this->BoostGauge->DrawArrow();
		}

		if (this->NosGauge)
		{
			this->NosGauge->Draw();
			this->NosGauge->DrawArrow();
		}

		if (this->NosLiniar)
		{
			this->NosLiniar->Draw();
		}

		if (this->SpeedBreakGauge)
		{
			this->SpeedBreakGauge->Draw();
			this->SpeedBreakGauge->DrawArrow();
		}

		if (this->SpeedBreakLiniar)
		{
			this->SpeedBreakLiniar->Draw();
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

		if (this->TachometerGauge)
		{
			this->TachometerGauge->DrawArrow();
		}

		if (this->SpeedometerGauge)
		{
			this->SpeedometerGauge->DrawArrow();
		}

		//this->tsm->Restore();
	}

	~HUD()
	{
		if (this->Background)
		{
			delete this->Background;
		}

		if (this->TachometerGauge)
		{
			delete this->TachometerGauge;
		}
		
		if (this->TachometerDigital)
		{
			delete this->TachometerDigital;
		}
		
		if (this->TachometerLiniar)
		{
			delete this->TachometerLiniar;
		}

		if (this->SpeedometerGauge)
		{
			delete this->SpeedometerGauge;
		}

		if (this->SpeedometerDigital)
		{
			delete this->SpeedometerDigital;
		}

		if (this->BoostGauge)
		{
			delete this->BoostGauge;
		}

		if (this->NosGauge)
		{
			delete this->NosGauge;
		}

		if (this->NosLiniar)
		{
			delete this->NosLiniar;
		}

		if (this->SpeedBreakGauge)
		{
			delete this->SpeedBreakGauge;
		}

		if (this->SpeedBreakLiniar)
		{
			delete this->SpeedBreakLiniar;
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
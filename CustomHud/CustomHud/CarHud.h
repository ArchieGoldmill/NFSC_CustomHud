#pragma once

#include <string>
#define _USE_MATH_DEFINES
#include <math.h>
#include "TextureStateManager.h"

#include "HUD_Tachometer.h"
#include "HUD_Speedometer.h"
#include "HUD_Filled.h"
#include "GameApi.h"

using namespace std;

class HUD
{
private:
	LPDIRECT3DDEVICE9 pDevice;
	TextureStateManager* tsm;

	HUD_Tachometer* Tachometer = NULL;
	HUD_Gauge* Speedometer = NULL;
	HUD_Speedometer* SpeedometerDigital = NULL;
	HUD_Gauge* Boost = NULL;
	HUD_Gauge* Nos = NULL;
	HUD_Filled* NosFilled = NULL;
	HUD_Gauge* SpeedBreak = NULL;
	HUD_Filled* SpeedBreakFilled = NULL;

	ID3DXFont* Font = NULL;

public:
	static bool ShowHUD;

	HUD(LPDIRECT3DDEVICE9 pDevice)
	{
		this->pDevice = pDevice;
		this->tsm = new TextureStateManager(pDevice);

		try
		{
			if (Global::HUDParams.Tachometer.Gauge.Enabled)
			{
				Global::HUDParams.Tachometer.Gauge.GetArrowValue = Game::GetRPM;
				Global::HUDParams.Tachometer.Gauge.GetMaxValue = Game::GetRedline;

				Global::HUDParams.Tachometer.Gauge.GetMaskValue1 = Game::GetRedline;
				Global::HUDParams.Tachometer.Gauge.GetMaskValue2 = []() { return Global::HUDParams.Tachometer.Gauge.Value; };

				Global::HUDParams.Tachometer.Gauge.GetArrowMaskValue1 = []() { return 0.0f; };
				Global::HUDParams.Tachometer.Gauge.GetArrowMaskValue2 = Game::GetRPM;

				Global::HUDParams.Tachometer.Gauge.IsInperfectZone = Game::IsInPerfectLaunchRange;

				Global::HUDParams.Tachometer.Gear.GetNumber = Game::GetGear;

				this->Tachometer = new HUD_Tachometer(pDevice, Global::HUDParams.Tachometer);
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
		}
		catch (string& s)
		{
			MessageBoxA(NULL, s.c_str(), "NFSC - Custom HUD", MB_ICONERROR);
		}
	}

	int gen = 0;
	void Draw()
	{
		auto start = chrono::steady_clock::now();

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

		if (this->SpeedometerDigital != NULL)
		{
			this->SpeedometerDigital->Draw();
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

		if (this->SpeedBreakFilled != NULL)
		{
			this->SpeedBreakFilled->Draw();
		}

		if (this->Tachometer != NULL)
		{
			this->Tachometer->DrawArrow();
		}

		this->tsm->Restore();

		if (Global::HUDParams.ShowDebugInfo)
		{
			this->DrawDebugInfo();
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

		if (this->SpeedometerDigital != NULL)
		{
			delete this->SpeedometerDigital;
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

		if (this->SpeedBreakFilled != NULL)
		{
			delete this->SpeedBreakFilled;
		}


		if (this->Font != NULL)
		{
			this->Font->Release();
		}

		delete this->tsm;
	}

private:
	void DrawDebugInfo()
	{
		this->DrawTextS("RPM=" + std::to_string(Game::GetRPM() * 1000.0f), 0);
		this->DrawTextS("RedLine=" + std::to_string(Game::GetRedline() * 1000.0f), 1);
		this->DrawTextS("Speed=" + std::to_string(Game::GetSpeed()), 2);
		this->DrawTextS("NOS=" + std::to_string(Game::GetNos()), 3);
		this->DrawTextS("SpeedBreaker=" + std::to_string(Game::GetSpeedBreaker()), 4);
		this->DrawTextS("Boost=" + std::to_string(Game::GetBoost()), 5);
		this->DrawTextS("TIME(microsec)=" + std::to_string(gen), 6);
	}

	void DrawTextS(string str, int line)
	{
		if (this->Font == NULL)
		{
			D3DXCreateFont(this->pDevice, 22, 0, FW_NORMAL, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial", &this->Font);
		}

		RECT font_rect;
		font_rect.left = 10;
		font_rect.top = 30 * line;
		font_rect.right = 100;
		font_rect.bottom = 30 * (line + 1);

		this->Font->DrawText(NULL, str.c_str(), -1, &font_rect, DT_LEFT | DT_NOCLIP, 0xFFFFFFFF);
	}
};
bool HUD::ShowHUD = false;
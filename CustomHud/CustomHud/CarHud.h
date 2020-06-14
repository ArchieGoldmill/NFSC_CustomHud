#pragma once

#include <string>
#define _USE_MATH_DEFINES
#include <math.h>
#include "DALVehicleCarbon.h"
#include <map>

#include "HUD_Tachometer.h"
#include "HUD_Speedometer.h"

using namespace std;

class CarHud
{
private:
	IDirect3DDevice9* pDevice;
	vector<Sprite*> sprites;

	HUD_Tachometer* Tachometer;
	HUD_Speedometer* Speedometer;
	HUD_Gauge* Boost;
	HUD_Gauge* Nos;
	HUD_Gauge* SpeedBreak;

public:
	static bool ShowHUD;

	CarHud(IDirect3DDevice9* pDevice, string path)
	{
		this->pDevice = pDevice;

		D3DXVECTOR2 maskSize;
		maskSize.x = 128;
		maskSize.y = maskSize.x;

		string temp = path;
		auto tachNumbers = new Sprite(this->pDevice, temp.append("\\tach-numbers.dds"));
		this->sprites.push_back(tachNumbers);

		temp = path;
		auto redLine = new CircleSprite(this->pDevice, temp.append("\\tach-redline.dds"), &maskSize);
		this->sprites.push_back(redLine);

		temp = path;
		auto tachArrow = new Sprite(this->pDevice, temp.append("\\tach-arrow.dds"));
		this->sprites.push_back(tachArrow);

		temp = path;
		auto tachBackground = new Sprite(this->pDevice, temp.append("\\tach-background.dds"));
		this->sprites.push_back(tachBackground);

		temp = path;
		auto digits = new Sprite(this->pDevice, temp.append("\\digits.dds"));
		this->sprites.push_back(digits);

		temp = path;
		auto speedUnits = new Sprite(this->pDevice, temp.append("\\speed-units.dds"));
		this->sprites.push_back(speedUnits);

		temp = path;
		auto boostBackground = new Sprite(this->pDevice, temp.append("\\boost-background.dds"));
		this->sprites.push_back(boostBackground);

		temp = path;
		auto nosBackground = new Sprite(this->pDevice, temp.append("\\nos-background.dds"));
		this->sprites.push_back(nosBackground);
		
		temp = path;
		auto nosFilled = new CircleSprite(this->pDevice, temp.append("\\nos-filled.dds"), &maskSize);
		this->sprites.push_back(nosFilled);

		temp = path;
		auto speedbreakBackground = new Sprite(this->pDevice, temp.append("\\speedbreak-background.dds"));
		this->sprites.push_back(speedbreakBackground);

		this->Tachometer = new HUD_Tachometer(pDevice, 330, { 30, 60 }, tachNumbers, tachArrow, digits, tachBackground, redLine);

		this->Speedometer = new HUD_Speedometer(pDevice, 75, 10, { 145, 99 }, digits, speedUnits);

		HUD_Gauge_Callbacks boostCallbacks;
		boostCallbacks.GetArrowValue = GetBoost;
		boostCallbacks.GetMaxValue = NULL;
		boostCallbacks.Value = 20;
		boostCallbacks.ZeroAngle = 90;
		boostCallbacks.MaxAngle = 210;
		boostCallbacks.direction = 1;
		this->Boost = new HUD_Gauge(pDevice, 110, { 340, 65 }, NULL, tachArrow, boostBackground, NULL, boostCallbacks);

		HUD_Gauge_Callbacks nosCallbacks;
		nosCallbacks.GetArrowValue = GetNos;
		nosCallbacks.GetMaxValue = NULL;
		nosCallbacks.GetMaskValue1 = []() {return 0.0f; };
		nosCallbacks.GetMaskValue2 = GetNos;
		nosCallbacks.MaskColor1 = D3DCOLOR_RGBA(0, 0, 0, 60);
		nosCallbacks.MaskColor2 = D3DCOLOR_RGBA(255, 255, 255, 255);
		nosCallbacks.Value = 1;
		nosCallbacks.ZeroAngle = -30;
		nosCallbacks.MaxAngle = 210;
		nosCallbacks.direction = 1;
		this->Nos = new HUD_Gauge(pDevice, 110, { 360, 190 }, NULL, tachArrow, nosBackground, nosFilled, nosCallbacks);

		HUD_Gauge_Callbacks speedbreakCallbacks;
		speedbreakCallbacks.GetArrowValue = GetSpeedBreaker;
		speedbreakCallbacks.GetMaxValue = NULL;
		speedbreakCallbacks.GetMaskValue1 = []() {return 0.0f; };
		speedbreakCallbacks.GetMaskValue2 = GetSpeedBreaker;
		speedbreakCallbacks.MaskColor1 = D3DCOLOR_RGBA(0, 0, 0, 60);
		speedbreakCallbacks.MaskColor2 = D3DCOLOR_RGBA(255, 255, 255, 255);
		speedbreakCallbacks.Value = 1;
		speedbreakCallbacks.ZeroAngle = -30;
		speedbreakCallbacks.MaxAngle = 210;
		speedbreakCallbacks.direction = 1;
		this->SpeedBreak = new HUD_Gauge(pDevice, 110, { 320, 310 }, NULL, tachArrow, speedbreakBackground, nosFilled, speedbreakCallbacks);
	}

	void Draw()
	{
		auto start = chrono::steady_clock::now();

		if (!IsHudVisible() || !IsPlayerControlling() || !CarHud::ShowHUD)
		{
			//return;
		}

		this->Tachometer->Draw();

		this->Speedometer->Draw();

		this->Boost->Draw();

		this->Nos->Draw();

		this->SpeedBreak->Draw();

		auto now = chrono::steady_clock::now();

		int a = chrono::duration_cast<std::chrono::microseconds>(now - start).count();

		int b = a;
	}

	~CarHud()
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

		for (auto sprite : this->sprites)
		{
			sprite->Release();
			delete sprite;
		}
	}
};
bool CarHud::ShowHUD = false;
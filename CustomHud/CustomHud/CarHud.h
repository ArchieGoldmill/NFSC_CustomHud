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
	hash<string> stringHasher;
	map<int, Sprite*> spritesMap;

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

		string temp = path;
		Sprite* tachNumbers = this->CreateSprite(temp.append("\\tach-numbers.dds"));

		temp = path;
		Sprite* tachArrow = this->CreateSprite(temp.append("\\tach-arrow.dds"));

		temp = path;
		Sprite* tachBackground = this->CreateSprite(temp.append("\\tach-background.dds"));

		temp = path;
		Sprite* digits = this->CreateSprite(temp.append("\\digits.dds"));
		
		temp = path;
		Sprite* speedUnits = this->CreateSprite(temp.append("\\speed-units.dds"));
		
		temp = path;
		Sprite* boostBackground = this->CreateSprite(temp.append("\\boost-background.dds"));
		
		temp = path;
		Sprite* nosBackground = this->CreateSprite(temp.append("\\nos-background.dds"));
		
		temp = path;
		Sprite* speedbreakBackground = this->CreateSprite(temp.append("\\speedbreak-background.dds"));

		this->Tachometer = new HUD_Tachometer(pDevice, 330, { 30, 60 }, tachNumbers, tachArrow, digits, tachBackground);

		this->Speedometer = new HUD_Speedometer(pDevice, 75, 10, { 145, 99 }, digits, speedUnits);

		HUD_Gauge_Callbacks boostCallbacks;
		boostCallbacks.GetValue = GetBoost;
		boostCallbacks.GetMaxValue = NULL;
		boostCallbacks.ZeroAngle = 90;
		boostCallbacks.StepAngle = 6;
		boostCallbacks.direction = 1;
		this->Boost = new HUD_Gauge(pDevice, 110, { 340, 65 }, NULL, tachArrow, boostBackground, boostCallbacks);

		HUD_Gauge_Callbacks nosCallbacks;
		nosCallbacks.GetValue = GetNos;
		nosCallbacks.GetMaxValue = NULL;
		nosCallbacks.ZeroAngle = -30;
		nosCallbacks.StepAngle = 24;
		nosCallbacks.direction = 1;
		this->Nos = new HUD_Gauge(pDevice, 110, { 360, 190 }, NULL, tachArrow, nosBackground, nosCallbacks);

		HUD_Gauge_Callbacks speedbreakCallbacks;
		speedbreakCallbacks.GetValue = GetSpeedBreaker;
		speedbreakCallbacks.GetMaxValue = NULL;
		speedbreakCallbacks.ZeroAngle = -30;
		speedbreakCallbacks.StepAngle = 24;
		speedbreakCallbacks.direction = 1;
		this->SpeedBreak = new HUD_Gauge(pDevice, 110, { 320, 310 }, NULL, tachArrow, speedbreakBackground, speedbreakCallbacks);
	}

	void Draw()
	{
		if (!IsHudVisible() || !IsPlayerControlling() || !CarHud::ShowHUD)
		{
			return;
		}

		this->Tachometer->Draw();

		this->Speedometer->Draw();

		this->Boost->Draw();

		this->Nos->Draw();

		this->SpeedBreak->Draw();
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

		for (auto sprite : this->spritesMap)
		{
			delete sprite.second;
		}
	}

private:
	Sprite* CreateSprite(string& path)
	{
		int hash = stringHasher(path);
		for (auto i : this->spritesMap)
		{
			if (i.first == hash)
			{
				return i.second;
			}
		}

		Sprite* sprite = new Sprite(pDevice, path);
		this->spritesMap.insert({ hash, sprite });

		return sprite;
	}
};
bool CarHud::ShowHUD = false;
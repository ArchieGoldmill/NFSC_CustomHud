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

		this->Tachometer = new HUD_Tachometer(pDevice, 350, { 30, 60 }, tachNumbers, tachArrow, digits, tachBackground);

		this->Speedometer = new HUD_Speedometer(pDevice, 80, 10, { 155, 105 }, digits);
	}

	void Draw()
	{
		if (!IsHudVisible() || !IsPlayerControlling() || !CarHud::ShowHUD)
		{
			//return;
		}

		this->Tachometer->Draw();

		this->Speedometer->Draw();
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
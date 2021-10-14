#pragma once
#include <vector>
#include <string>

namespace Game
{
	class Api
	{
	public:
		virtual int Device() = 0;

		virtual float GetBoost() = 0;
		virtual bool IsBoostInstalled() = 0;
		virtual float GetNos() = 0;
		virtual bool IsNosInstalled() = 0;
		virtual float GetSpeedBreaker() = 0;
		virtual float GetSpeed() = 0;
		virtual bool IsHudVisible() = 0;
		virtual float GetRPM() = 0;
		virtual float GetRedline() = 0;
		virtual int GetGear() = 0;
		virtual bool GetUnits() = 0;
		virtual bool IsInPerfectLaunchRange() = 0;
		virtual std::string GetCarName() = 0;
	};

	float GetBoost();
	bool IsBoostInstalled();
	float GetNos();
	bool IsNosInstalled();
	float GetSpeedBreaker();
	float GetSpeed();
	bool IsHudVisible();
	float GetRPM();
	float GetRedline();
	int GetGear();
	bool GetUnits();
	bool IsInPerfectLaunchRange();

	int GetSpeed0();

	int GetSpeed1();

	int GetSpeed2();

	float LocalizeSpeed(float speed);

	extern Api* Current;
}

unsigned int* GetPtr(unsigned int* offsets, int count);
bool GetBit(int n, int k);
void ClearBit(int& n, int k);
void ClearBit(__int64& n, int k);
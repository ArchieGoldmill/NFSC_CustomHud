#pragma once
#include "GameApi.h"

namespace Game
{
	class MostWanted : public Api
	{
	public:
		MostWanted();

		int Device();
		float GetBoost();
		bool IsBoostInstalled();
		float GetNos();
		bool IsNosInstalled();
		float GetSpeedBreaker();
		int GetSpeed();
		bool IsHudVisible();
		float GetRPM();
		float GetRedline();
		int GetGear();
		bool GetUnits();
		bool IsInPerfectLaunchRange();
	};
}